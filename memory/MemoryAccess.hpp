/** License
*
* Copyright (c) 2014 Adam Œmigielski
*
*
*  Permission is hereby granted, free of charge, to any person obtaining a
*      copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions: The above copyright notice and this permission
*      notice shall be included in all copies or substantial portions of the
*      Software.
*
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
*      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*      CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*      TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
**/

/**
* @author Adam Œmigielski
* @file MemoryAccess.hpp
**/

#ifndef O8_TEMPLATES_MEMORY_ACCESS_HPP
#define O8_TEMPLATES_MEMORY_ACCESS_HPP

namespace O8
{
    namespace MemoryAccess
    {
        template <typename T>
        class Wrapper
        {
        public:
            Wrapper(T & t)
                : m_t(t)
            {
                /* Nothing to be done here */
            }

            int32 Read(
                size_t offset,
                void * buffer,
                size_t size)
            {
                uint8 * ptr = (uint8 *) &m_t[offset];

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Failure;
                }

                memcpy(buffer, ptr, size);

                return Success;
            }

            int32 Write(
                size_t offset,
                const void * buffer,
                size_t size)
            {
                uint8 * ptr = (uint8 *) &m_t[offset];

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Failure;
                }

                memcpy(ptr, buffer, size);

                return Success;
            }

        private:
            T & m_t;
        };

        template <>
        class Wrapper < Utility::Binary_data >
        {
        public:
            Wrapper(Utility::Binary_data & t)
                : m_t(t)
            {
                /* Nothing to be done here */
            }

            int32 Read(
                size_t offset,
                void * buffer,
                size_t size)
            {
                if (m_t.Size() < offset + size)
                {
                    ASSERT(0);
                    return Failure;
                }

                uint8 * ptr = (uint8 *) m_t.Data() + offset;

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Failure;
                }

                memcpy(buffer, ptr, size);

                return Success;
            }

            int32 Write(
                size_t offset,
                const void * buffer,
                size_t size)
            {
                if (m_t.Size() < offset + size)
                {
                    ASSERT(0);
                    return Failure;
                }

                uint8 * ptr = (uint8 *) m_t.Data() + offset;

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Failure;
                }

                memcpy(ptr, buffer, size);

                return Success;
            }

        private:
            Utility::Binary_data & m_t;
        };

        template <>
        class Wrapper < std::fstream >
        {
        public:
            Wrapper(std::fstream & t)
                : m_t(t)
            {
                /* Nothing to be done here */
            }

            int32 Read(
                size_t offset,
                void * buffer,
                size_t size)
            {
                m_t.seekg(offset, std::fstream::beg);

                m_t.read((char *) buffer, size);

                if (false == m_t.good())
                {
                    ASSERT(0);
                    return Failure;
                }

                return Success;
            }

            int32 Write(
                size_t offset,
                const void * buffer,
                size_t size)
            {
                m_t.seekp(offset, std::fstream::beg);

                m_t.write((char *) buffer, size);

                if (false == m_t.good())
                {
                    ASSERT(0);
                    return Failure;
                }

                return Success;
            }

        private:
            std::fstream & m_t;
        };

        template <typename T>
        void Swap_endianess(T & t)
        {
            static const uint size = sizeof(T);
            static const uint n_iter = size / 2;
            static const uint last = size - 1;

            char * ptr = (char *) &t;

            for (uint i = 0; i < n_iter; ++i)
            {
                const uint beg = i;
                const uint end = last - i;

                char temp = ptr[end];
                ptr[end] = ptr[beg];
                ptr[beg] = temp;
            }
        }

        template <>
        void Swap_endianess(int8 & c)
        {
            /* Nothing to be done here */
        }

        template <>
        void Swap_endianess(uint8 & c)
        {
            /* Nothing to be done here */
        }

        template <typename B, typename T>
        int32 Read(
            B & b,
            size_t offset,
            T & out_t)
        {
            Wrapper<B> w(b);

            return w.Read(
                offset,
                &out_t,
                sizeof(T));
        }

        template <typename B, typename T>
        int32 Read(
            B & b,
            size_t offset,
            bool swap_endianess,
            T & out_t)
        {
            auto ret = Read(
                b,
                offset,
                out_t);
            if (Success != ret)
            {
                return ret;
            }

            if (true == swap_endianess)
            {
                Swap_endianess(out_t);
            }

            return Success;
        }

        template <typename B, typename T>
        int32 Read(
            B & b,
            size_t offset,
            size_t size,
            T * t)
        {
            Wrapper<B> w(b);

            return w.Read(
                offset,
                t,
                size);
        }

        template <typename B>
        int32 Read(
            B & b,
            size_t offset,
            bool swap_endianess,
            std::string & out_string)
        {
            Wrapper<B> w(b);
            uint32 length;

            if (Success != w.Read(
                offset,
                &length,
                sizeof(uint32)))
            {
                ASSERT(0);
                return Failure;
            }

            if (true == swap_endianess)
            {
                Swap_endianess(length);
            }

            out_string.resize(length);
            if (length > out_string.length())
            {
                ASSERT(0);
                return Failure;
            }

            if (Success != w.Read(
                offset + sizeof(length),
                &out_string.front(),
                length))
            {
                ASSERT(0);
                return Failure;
            }

            return Success;
        }

        template <typename B, typename T>
        int32 Write(
            B & b,
            size_t offset,
            const T & t)
        {
            Wrapper<B> w(b);
            return w.Write(offset, &t, sizeof(T));
        }

        template <typename B>
        int32 Write(
            Wrapper<B> & w,
            size_t offset,
            const std::string & string)
        {
            Wrapper<B> w(b);
            const uint32 length = (uint32)string.length();

            if (Success != w.Write(offset, &length, sizeof(uint32)))
            {
                ASSERT(0);
                return Failure;
            }

            return w.Write(
                offset + sizeof(length),
                string.data(),
                length);
        }

        template <typename B, typename T>
        int32 Write(
            B & b,
            size_t offset,
            const T * t,
            size_t size)
        {
            Wrapper<B> w(b);
            return w.Write(offset, t, size);
        }
    }
}

#endif /* O8_TEMPLATES_MEMORY_ACCESS_HPP */

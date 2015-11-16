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

#ifndef UTILITIES_MEMORY_MEMORYACCESS_HPP
#define UTILITIES_MEMORY_MEMORYACCESS_HPP

namespace Memory
{
    namespace Access
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

            Platform::int32 Read(
                size_t offset,
                void * buffer,
                size_t size)
            {
                uint8 * ptr = (uint8 *) &m_t[offset];

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                memcpy(buffer, ptr, size);

                return Utilities::Success;
            }

            Platform::int32 Write(
                size_t offset,
                const void * buffer,
                size_t size)
            {
                uint8 * ptr = (uint8 *) &m_t[offset];

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                memcpy(ptr, buffer, size);

                return Utilities::Success;
            }

        private:
            T & m_t;
        };

        template <>
        class Wrapper < Memory::Binary_data >
        {
        public:
            Wrapper(Memory::Binary_data & t)
                : m_t(t)
            {
                /* Nothing to be done here */
            }

            Platform::int32 Read(
                size_t offset,
                void * buffer,
                size_t size)
            {
                if (m_t.Size() < offset + size)
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                auto ptr = (Platform::uint8 *)m_t.Data() + offset;

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                memcpy(buffer, ptr, size);

                return Utilities::Success;
            }

            Platform::int32 Write(
                size_t offset,
                const void * buffer,
                size_t size)
            {
                if (m_t.Size() < offset + size)
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                auto ptr = (Platform::uint8 *) m_t.Data() + offset;

                if (nullptr == ptr)
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                memcpy(ptr, buffer, size);

                return Utilities::Success;
            }

        private:
            Memory::Binary_data & m_t;
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

            Platform::int32 Read(
                size_t offset,
                void * buffer,
                size_t size)
            {
                m_t.seekg(offset, std::fstream::beg);

                m_t.read((char *) buffer, size);

                if (false == m_t.good())
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                return Utilities::Success;
            }

            Platform::int32 Write(
                size_t offset,
                const void * buffer,
                size_t size)
            {
                m_t.seekp(offset, std::fstream::beg);

                m_t.write((char *) buffer, size);

                if (false == m_t.good())
                {
                    ASSERT(0);
                    return Utilities::Failure;
                }

                return Utilities::Success;
            }

        private:
            std::fstream & m_t;
        };

        template <typename T>
        void Swap_endianess(T & t)
        {
            static const Platform::uint size = sizeof(T);
            static const Platform::uint n_iter = size / 2;
            static const Platform::uint last = size - 1;

            char * ptr = (char *) &t;

            for (Platform::uint i = 0; i < n_iter; ++i)
            {
                const Platform::uint beg = i;
                const Platform::uint end = last - i;

                char temp = ptr[end];
                ptr[end] = ptr[beg];
                ptr[beg] = temp;
            }
        }

        template <>
        void Swap_endianess(Platform::int8 & c)
        {
            /* Nothing to be done here */
        }

        template <>
        void Swap_endianess(Platform::uint8 & c)
        {
            /* Nothing to be done here */
        }

        template <typename B, typename T>
        Platform::int32 Read(
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
        Platform::int32 Read(
            B & b,
            size_t offset,
            bool swap_endianess,
            T & out_t)
        {
            auto ret = Read(
                b,
                offset,
                out_t);
            if (Utilities::Success != ret)
            {
                return ret;
            }

            if (true == swap_endianess)
            {
                Swap_endianess(out_t);
            }

            return Utilities::Success;
        }

        template <typename B, typename T>
        Platform::int32 Read(
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
        Platform::int32 Read(
            B & b,
            size_t offset,
            bool swap_endianess,
            std::string & out_string)
        {
            Wrapper<B> w(b);
            Platform::uint32 length;

            if (Utilities::Success != w.Read(
                offset,
                &length,
                sizeof(Platform::uint32)))
            {
                ASSERT(0);
                return Utilities::Failure;
            }

            if (true == swap_endianess)
            {
                Swap_endianess(length);
            }

            out_string.resize(length);
            if (length > out_string.length())
            {
                ASSERT(0);
                return Utilities::Failure;
            }

            if (Utilities::Success != w.Read(
                offset + sizeof(length),
                &out_string.front(),
                length))
            {
                ASSERT(0);
                return Utilities::Failure;
            }

            return Utilities::Success;
        }

        template <typename B, typename T>
        Platform::int32 Write(
            B & b,
            size_t offset,
            const T & t)
        {
            Wrapper<B> w(b);
            return w.Write(offset, &t, sizeof(T));
        }

        template <typename B>
        Platform::int32 Write(
            Wrapper<B> & w,
            size_t offset,
            const std::string & string)
        {
            Wrapper<B> w(b);
            const uint32 length = (uint32)string.length();

            if (Utilities::Success != w.Write(offset, &length, sizeof(uint32)))
            {
                ASSERT(0);
                return Utilities::Failure;
            }

            return w.Write(
                offset + sizeof(length),
                string.data(),
                length);
        }

        template <typename B, typename T>
        Platform::int32 Write(
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

#endif /* UTILITIES_MEMORY_MEMORYACCESS_HPP */

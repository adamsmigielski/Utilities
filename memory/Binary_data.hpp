/** License
*
* Copyright(c) 2014 Adam Œmigielski
*
*
*  Permission is hereby granted, free of charge, to any person obtaining a
*      copy of this software and associated documentation files(the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*distribute, sublicense, and / or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions : The above copyright notice and this permission
*      notice shall be included in all copies or substantial portions of the
*      Software.
*
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
*      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*      CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
** /

/**
* @author Adam Œmigielski
* @file Binary_data.hpp
**/

#ifndef UTILITIES_MEMORY_BINARYDATA_HPP
#define UTILITIES_MEMORY_BINARYDATA_HPP

namespace Memory
{
    class Binary_data
    {
    public:
        using size_type = Platform::uint64;

        Binary_data();
        Binary_data(Platform::uint8 * data, size_type size);
        Binary_data(const Binary_data & data);
        Binary_data(Binary_data && data);
        Binary_data & operator = (const Binary_data & data);
        Binary_data & operator = (Binary_data && data);
        ~Binary_data();


        Platform::uint8 * Data() const;
        size_type Size() const;
        Platform::uint8 & operator [] (size_type offset) const;

        Platform::int32 Copy_range(
            const Binary_data & data,
            size_type offset,
            size_type size);
        void Release();
        void Reset(Platform::uint8 * data, size_type size);

        bool Is_null() const;

    private:
        Platform::int32 copy(Platform::uint8 * data, size_type size);
        void move(Binary_data & data);
        void set(Platform::uint8 * data, size_type size);

        Platform::uint8 * m_data;
        size_type m_size;
    };

} /* namespace Memory */

#endif /* UTILITIES_MEMORY_BINARYDATA_HPP */

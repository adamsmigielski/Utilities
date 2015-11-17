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
* @file Binary_data.cpp
**/

#include "PCH.hpp"
#include "Binary_data.hpp"

namespace Memory
{
    Binary_data::Binary_data()
        : Binary_data(nullptr, 0)
    {
        /* Nothing to be done here */
    }

    Binary_data::Binary_data(Platform::uint8 * data, Platform::uint64 size)
        : m_data(data)
        , m_size(size)
    {
        /* Nothing to be done here */
    }

    Binary_data::Binary_data(const Binary_data & data)
        : Binary_data()
    {
        if (Utilities::Success != copy(data.m_data, data.m_size))
        {
            Release();
        }
    }

    Binary_data::Binary_data(Binary_data && data)
        : Binary_data()
    {
        move(data);
    }

    Binary_data & Binary_data::operator = (const Binary_data & data)
    {
        Release();

        copy(data.m_data, data.m_size);

        return *this;
    }

    Binary_data & Binary_data::operator = (Binary_data && data)
    {
        Release();

        move(data);

        return *this;
    }

    Binary_data::~Binary_data()
    {
        Release();
    }

    Platform::uint8 * Binary_data::Data() const
    {
        return m_data;
    }

    auto Binary_data::Size() const -> size_type
    {
        return m_size;
    }

    Platform::uint8 & Binary_data::operator [] (size_type offset) const
    {
        return *(m_data + offset);
    }

    void Binary_data::Release()
    {
        if (nullptr != m_data)
        {
            delete (char *) m_data;
            m_data = nullptr;
        }

        m_size = 0;
    }

    Platform::int32 Binary_data::Copy_range(
        const Binary_data & data,
        Binary_data::size_type offset,
        Binary_data::size_type size)
    {
        if (true == data.Is_null())
        {
            return Utilities::Invalid_parameter;
        }

        Release();

        return copy(data.m_data + offset, size);
    }

    void Binary_data::Reset(Platform::uint8 * data, size_type size)
    {
        Release();

        m_data = data;
        m_size = size;
    }

    bool Binary_data::Is_null() const
    {
        return (nullptr == m_data);
    }

    Platform::int32 Binary_data::copy(Platform::uint8 * data, size_type size)
    {
        auto ptr = new Platform::uint8[size_t(size)];

        if (nullptr == ptr)
        {
            DEBUGLOG("Memory allocation failed");
            ASSERT(0);
            return Utilities::Failed_to_allocate_memory;
        }

        memcpy(ptr, data, size_t(size));

        set(ptr, size);

        return Utilities::Success;
    }

    void Binary_data::move(Binary_data & data)
    {
        set(data.m_data, data.m_size);
        data.set(nullptr, 0);
    }

    void Binary_data::set(Platform::uint8 * data, size_type size)
    {
        m_data = data;
        m_size = size;
    }

} /* namespace Memory */


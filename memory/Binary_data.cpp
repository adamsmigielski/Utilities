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
        : m_data(nullptr)
        , m_size(0)
    {
        /* Nothing to be done here */
    }

    Binary_data::Binary_data(Platform::uint8 * data, size_t size)
        : m_data(data)
        , m_size(size)
    {
        /* Nothing to be done here */
    }

    Binary_data::Binary_data(const Binary_data & data)
        : m_data(nullptr)
        , m_size(0)
    {
        copy(data.m_data, data.m_size);
    }

    Binary_data::Binary_data(Binary_data && data)
        : m_data(std::move(data.m_data))
        , m_size(std::move(data.m_size))
    {
        /* Nothing to be done here */
    }

    Binary_data & Binary_data::operator = (const Binary_data & data)
    {
        copy(data.m_data, data.m_size);

        return *this;
    }

    Binary_data & Binary_data::operator = (Binary_data && data)
    {
        m_data = std::move(data.m_data);
        m_size = std::move(data.m_size);

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

    size_t Binary_data::Size() const
    {
        return m_size;
    }

    Platform::uint8 & Binary_data::operator [] (size_t offset) const
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

    void Binary_data::Reset(Platform::uint8 * data, size_t size)
    {
        Release();

        m_data = data;
        m_size = size;
    }

    bool Binary_data::Is_null() const
    {
        return (nullptr == m_data);
    }

    void Binary_data::copy(Platform::uint8 * data, size_t size)
    {
        auto ptr = new Platform::uint8[size];

        if (nullptr == ptr)
        {
            ERRLOG("Memory allocation failure");
            ASSERT(0);
            return;
        }

        memcpy(ptr, data, size);

        set(ptr, size);
    }

    void Binary_data::set(Platform::uint8 * data, size_t size)
    {
        m_data = data;
        m_size = size;
    }

} /* namespace Memory */


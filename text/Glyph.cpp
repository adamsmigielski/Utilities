/** License
*
* Copyright (c) 2015 Adam Œmigielski
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
* @file Glyph.cpp
**/

#include "PCH.hpp"
#include "Glyph.hpp"

namespace Text
{
    Glyph::Glyph()
    {
        m_descriptor = Descriptor{ 0, 0, 0, 0, 0, 0, 0, 0 };
    }

    Glyph::~Glyph()
    {
        Release();
    }

    Glyph::Glyph(Glyph && glyph)
    {
        m_data = std::move(glyph.m_data);
        m_descriptor = glyph.m_descriptor;

        glyph.Release();
    }

    Glyph & Glyph::operator = (Glyph && glyph)
    {
        Release();

        m_data = std::move(glyph.m_data);
        m_descriptor = glyph.m_descriptor;

        glyph.Release();

        return *this;
    }

    Platform::int32 Glyph::Init(
        Memory::Binary_data && data,
        const Descriptor & descriptor)
    {
        Release();

        m_data = std::move(data);
        m_descriptor = descriptor;

        return Utilities::Success;
    }

    void Glyph::Release()
    {
        m_data.Release();

        m_descriptor.m_width = 0;
        m_descriptor.m_height = 0;
        m_descriptor.m_left = 0;
        m_descriptor.m_top = 0;
        m_descriptor.m_right = 0;
        m_descriptor.m_bottom = 0;
        m_descriptor.m_horizontal_advance = 0;
        m_descriptor.m_vertical_advance = 0;
    }

    const Memory::Binary_data & Glyph::Get_data() const
    {
        return m_data;
    }

    auto Glyph::Get_descriptor() const -> const Descriptor &
    {
        return m_descriptor;
    }
}

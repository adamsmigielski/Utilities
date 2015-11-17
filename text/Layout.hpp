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
* @file Layout.hpp
**/

#ifndef TEXT_LAYOUT_HPP
#define TEXT_LAYOUT_HPP

#include "Font.hpp"

namespace Text
{
    class Box
    {
    public:
        /* Ctr & Dtr */
        Box() = default;
        ~Box() = default;

        /* Copy */
        Box(const Box & box) = default;
        Box & operator = (const Box & box) = default;

        /* Members */
        Platform::int32 m_Left;
        Platform::int32 m_Top;
        Platform::int32 m_Right;
        Platform::int32 m_Bottom;
    };

    struct Glyph_position
    {
        const Glyph * m_Glyph;
        const Box * m_Box;
        Platform::int32 m_X;
        Platform::int32 m_Y;
    };

    struct Layout
    {     
        Glyph_position ** m_Positions;
        Platform::uint32 m_Count;
    };

    enum Layout_errors
    {
        Not_enough_space = -1024,
    };


    Platform::int32 Init_layout(
        const Font & font,
        const Box * boxes,
        const Platform::uint32 boxes_count,
        const Font::character_t * text,
        const Platform::uint32 characters_count,
        Layout & layout);
    void Release_layout(Layout & layout);
}

#endif /* TEXT_LAYOUT_HPP */

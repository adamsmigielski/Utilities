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
* @file Glyph.hpp
**/

#ifndef TEXT_GLYPH_HPP
#define TEXT_GLYPH_HPP

#include <Utilities\memory\Binary_data.hpp>

namespace Text
{
    class Font;

    /** \brief Information and image of single character
     *
     * Data is an image A8 width x height. Data is owned by Glyph instance.
     **/
    class Glyph
    {
        friend class Font;

    public:
        /* Types */
        struct Descriptor
        {
            Platform::uint32 m_width;
            Platform::uint32 m_height;
            Platform::int32  m_left;
            Platform::int32  m_top;
            Platform::int32  m_right;
            Platform::int32  m_bottom;
            Platform::int32  m_horizontal_advance;
            Platform::int32  m_vertical_advance;
        };

        /* Ctr & dtr */
        Glyph();
        ~Glyph();

        /* Copy */
        Glyph(const Glyph & g) = default;
        Glyph & operator = (const Glyph & g) = default;

        /* Move */
        Glyph(Glyph && glyph);
        Glyph & operator = (Glyph && glyph);

        /* Initialization and release */
        Platform::int32 Init(
            Memory::Binary_data && data,
            const Descriptor & descriptor);
        void Release();

        /* Access */
        const Memory::Binary_data & Get_data() const;
        const Descriptor & Get_descriptor() const;

    private:
        Memory::Binary_data m_data;

        Descriptor m_descriptor;
    };
}

#endif /* TEXT_GLYPH_HPP */

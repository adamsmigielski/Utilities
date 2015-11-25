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
* @file Font.hpp
**/

#ifndef TEXT_FONT_HPP
#define TEXT_FONT_HPP

#include "Glyph.hpp"

#include <Utilities\memory\Binary_data.hpp>

/* Defines number of lut elements used by text module */
#define TEXT_FONT_CHAR_LUT_SIZE 128


namespace Text
{
    class Font_pimpl;
    class Glyph;

    /** \brief Represents single font face.
     **/
    class Font
    {
    public:
        /* Types */
        using character_t = Platform::uint32;

        /* Ctr & dtr */
        Font();
        ~Font();

        /* No copying */
        Font(const Font & font) = delete;
        Font & operator =(const Font & font) = delete;

        /* Move */
        Font(Font && font);
        Font & operator = (Font && font);

        /* Init & release */
        Platform::int32 Init();
        Platform::int32 Init(
            Memory::Binary_data && data,
            bool is_endianess_swapped);
        Platform::int32 Font::Store(Memory::Binary_data & out_result) const;
        void Release();

        /* Access */
        Platform::int32 Add_glyph(
            const character_t character,
            const Glyph::Descriptor & decriptor,
            Memory::Binary_data && image);
        const Glyph * Get_glyph(character_t character) const;
        const Glyph * Get_glyph_raw(character_t character) const;
        const Glyph::Descriptor * Get_max() const;

    private:
        Font_pimpl * m_pimpl;
    };
}

#endif /* TEXT_FONT_HPP */

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
* @file Layout.cpp
**/

#include "PCH.hpp"
#include "Layout.hpp"
#include "Glyph.hpp"

#include <Utilities\containers\PointerContainer.hpp>

#include <list>

namespace Text
{
    using position_list = std::list < Glyph_position * >;

    static void return_cursor_in_line(
        const Box & box,
        Platform::int32 & x)
    {
        x = box.m_Left;
    }

    static void return_cursor_in_row(
        const Box & box,
        const Glyph::Descriptor & max,
        Platform::int32 & y)
    {
        y = box.m_Top - max.m_top;
    }

    static void start_new_box(
        const Box & box,
        const Glyph::Descriptor & max,
        Platform::int32 & x,
        Platform::int32 & y)
    {
        return_cursor_in_line(box, x);
        return_cursor_in_row(box, max, y);
    }

    static void advance_cursor_in_line(
        const Glyph::Descriptor & descriptor,
        Platform::int32 & x)
    {
        x += descriptor.m_horizontal_advance;
    }

    static void advance_cursor_in_row(
        const Glyph::Descriptor & descriptor,
        Platform::int32 & y)
    {
        y += descriptor.m_vertical_advance;
    }

    static bool does_glyph_fit_horizontally(
        const Box & box,
        const Glyph::Descriptor & descriptor,
        Platform::int32 & x)
    {
        const auto left = box.m_Right - x;

        return left >= descriptor.m_right;
    }

    static bool does_glyph_fit_vertically(
        const Box & box,
        const Glyph::Descriptor & descriptor,
        Platform::int32 & y)
    {
        const auto left = y - box.m_Bottom;

        return left >= -descriptor.m_bottom;
    }

    static Glyph_position * add_glyph_position(
        const Box * box,
        const Platform::int32 x,
        const Platform::int32 y,
        const Glyph * glyph,
        position_list & list)
    {
        /* Allocate and init */
        auto ptr = new Glyph_position;
        if (nullptr == ptr)
        {
            ASSERT(0);
            return nullptr;
        }

        ptr->m_Box = box;
        ptr->m_X = x;
        ptr->m_Y = y;
        ptr->m_Glyph = glyph;

        /* Store ptr on list */
        list.push_back(ptr);

        /* Done */
        return ptr;
    }

    static void release_resources(
        position_list & positions)
    {
        Containers::PointerContainer::Remove_all(positions);
    }

    Platform::int32 Init_layout(
        const Font & font,
        const Box * boxes,
        const Platform::uint32 boxes_count,
        const Font::character_t * text,
        const Platform::uint32 characters_count,
        Layout & layout)
    {
        if ((nullptr == boxes) ||
            (nullptr == text)  ||
            (0 == boxes_count) ||
            (0 == characters_count))
        {
            return Utilities::Invalid_parameter;
        }

        auto max = font.Get_max();
        if (nullptr == max)
        {
            return Utilities::Invalid_object;
        }

        position_list positions;

        Platform::uint32 current_box_index = 0;
        Platform::uint32 current_char_index = 0;

        do
        {
            auto current_box = boxes + current_box_index;

            Platform::int32 cursor_x = 0;
            Platform::int32 cursor_y = 0;

            start_new_box(*current_box, *max, cursor_x, cursor_y);
            
            for (; current_char_index < characters_count; ++current_char_index)
            {
                const auto character = text[current_char_index];

                const Glyph * glyph = font.Get_glyph(character);
                const auto& descriptor = glyph->Get_descriptor();

                /* Fit glyph */
                if (false == does_glyph_fit_vertically(*current_box, descriptor, cursor_y))
                {
                    break /* Go to another box */;
                }

                if (false == does_glyph_fit_horizontally(*current_box, descriptor, cursor_x))
                {
                    /* Go to next line */
                    return_cursor_in_line(*current_box, cursor_x);
                    advance_cursor_in_row(*max, cursor_y);

                    if (false == does_glyph_fit_horizontally(*current_box, descriptor, cursor_x))
                    {
                        break; /* Go to another box */
                    }
                }

                /* Save glyph_usage and position */
                auto ret = add_glyph_position(                    
                    current_box,
                    cursor_x,
                    cursor_y,
                    glyph,
                    positions);
                if (nullptr == ret)
                {
                    ASSERT(0);
                    release_resources(positions);
                    return Utilities::Failed_to_allocate_memory;
                }
            }

            /* All characters done */
            if (current_char_index != characters_count)
            {
                ++current_box_index;
            }
            else
            {
                break;
            }
        } while (current_box_index < boxes_count);

        /* Store results */
        {
            auto ptr = new Glyph_position *[current_char_index];
            if (nullptr == ptr)
            {
                ASSERT(0);
                release_resources(positions);
                return Utilities::Failed_to_allocate_memory;
            }

            Release_layout(layout);

            layout.m_Positions = ptr;
            layout.m_Count = current_char_index;
            Platform::uint32 index = 0;

            for (auto it : positions)
            {
                ptr[index] = it;
            }
        }

        if (current_char_index != characters_count)
        {
            return Not_enough_space;
        }

        return Utilities::Success;
    }

    void Release_layout(Layout & layout)
    {
        for (size_t i = 0; i < layout.m_Count; ++i)
        {
            delete layout.m_Positions[i];
            layout.m_Positions[i] = nullptr;
        }

        delete[] layout.m_Positions;
        layout.m_Positions = nullptr;
        layout.m_Count = 0;
    }
}

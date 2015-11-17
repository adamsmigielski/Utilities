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
* @file test.cpp
**/

#include "PCH.hpp"

#include <Unit_Tests\UnitTests.hpp>
#include <Utilities\memory\Binary_data.hpp>
#include <Utilities\memory\MemoryAccess.hpp>

#include "Glyph.hpp"
#include "Font.hpp"
#include "Layout.hpp"

UNIT_TEST(Text_glyph_initial_state)
{
    Text::Glyph glyph;

    auto& data = glyph.Get_data();
    TEST_ASSERT(true, data.Is_null());

    auto& desc = glyph.Get_descriptor();
    TEST_ASSERT(0, desc.m_width);
    TEST_ASSERT(0, desc.m_height);
    TEST_ASSERT(0, desc.m_left);
    TEST_ASSERT(0, desc.m_top);
    TEST_ASSERT(0, desc.m_right);
    TEST_ASSERT(0, desc.m_bottom);
    TEST_ASSERT(0, desc.m_horizontal_advance);
    TEST_ASSERT(0, desc.m_vertical_advance);

    return Passed;
}

UNIT_TEST(Text_glyph_init)
{
    Text::Glyph glyph;
    const Text::Glyph::Descriptor init_desc = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const size_t size = 32;
    const auto ptr = new Platform::uint8[size];

    if (nullptr == ptr)
    {
        return NotAvailable;
    }
    for (size_t i = 0; i < size; ++i)
    {
        ptr[i] = Platform::uint8(i);
    }

    Memory::Binary_data init_data(ptr, size);

    glyph.Init(std::move(init_data), init_desc);

    {
        auto& data = glyph.Get_data();
        TEST_ASSERT(true, init_data.Is_null());
        TEST_ASSERT(false, data.Is_null());
        for (size_t i = 0; i < size; ++i)
        {
            TEST_ASSERT(Platform::uint8(i), data.Data()[i]);
        }

        auto& desc = glyph.Get_descriptor();
        TEST_ASSERT(init_desc.m_width, desc.m_width);
        TEST_ASSERT(init_desc.m_height, desc.m_height);
        TEST_ASSERT(init_desc.m_left, desc.m_left);
        TEST_ASSERT(init_desc.m_top, desc.m_top);
        TEST_ASSERT(init_desc.m_right, desc.m_right);
        TEST_ASSERT(init_desc.m_bottom, desc.m_bottom);
        TEST_ASSERT(init_desc.m_horizontal_advance, desc.m_horizontal_advance);
        TEST_ASSERT(init_desc.m_vertical_advance, desc.m_vertical_advance);
    }

    /* Copy */
    {
        auto glyph_b(glyph);

        Text::Glyph glyph_c;
        glyph_c = glyph_b;

        auto& data = glyph.Get_data();
        TEST_ASSERT(false, data.Is_null());
        for (size_t i = 0; i < size; ++i)
        {
            TEST_ASSERT(Platform::uint8(i), data.Data()[i]);
        }

        auto& desc = glyph.Get_descriptor();
        TEST_ASSERT(init_desc.m_width, desc.m_width);
        TEST_ASSERT(init_desc.m_height, desc.m_height);
        TEST_ASSERT(init_desc.m_left, desc.m_left);
        TEST_ASSERT(init_desc.m_top, desc.m_top);
        TEST_ASSERT(init_desc.m_right, desc.m_right);
        TEST_ASSERT(init_desc.m_bottom, desc.m_bottom);
        TEST_ASSERT(init_desc.m_horizontal_advance, desc.m_horizontal_advance);
        TEST_ASSERT(init_desc.m_vertical_advance, desc.m_vertical_advance);

        auto& data_b = glyph_b.Get_data();
        TEST_ASSERT(false, data_b.Is_null());
        for (size_t i = 0; i < size; ++i)
        {
            TEST_ASSERT(Platform::uint8(i), data_b.Data()[i]);
        }

        auto& desc_b = glyph_b.Get_descriptor();
        TEST_ASSERT(init_desc.m_width, desc_b.m_width);
        TEST_ASSERT(init_desc.m_height, desc_b.m_height);
        TEST_ASSERT(init_desc.m_left, desc_b.m_left);
        TEST_ASSERT(init_desc.m_top, desc_b.m_top);
        TEST_ASSERT(init_desc.m_right, desc_b.m_right);
        TEST_ASSERT(init_desc.m_bottom, desc_b.m_bottom);
        TEST_ASSERT(init_desc.m_horizontal_advance, desc_b.m_horizontal_advance);
        TEST_ASSERT(init_desc.m_vertical_advance, desc_b.m_vertical_advance);

        auto& data_c = glyph_c.Get_data();
        TEST_ASSERT(false, data_c.Is_null());
        for (size_t i = 0; i < size; ++i)
        {
            TEST_ASSERT(Platform::uint8(i), data_c.Data()[i]);
        }

        auto& desc_c = glyph_c.Get_descriptor();
        TEST_ASSERT(init_desc.m_width, desc_c.m_width);
        TEST_ASSERT(init_desc.m_height, desc_c.m_height);
        TEST_ASSERT(init_desc.m_left, desc_c.m_left);
        TEST_ASSERT(init_desc.m_top, desc_c.m_top);
        TEST_ASSERT(init_desc.m_right, desc_c.m_right);
        TEST_ASSERT(init_desc.m_bottom, desc_c.m_bottom);
        TEST_ASSERT(init_desc.m_horizontal_advance, desc_c.m_horizontal_advance);
        TEST_ASSERT(init_desc.m_vertical_advance, desc_c.m_vertical_advance);
    }

    /* Move */
    {
        auto glyph_b(glyph);

        auto glyph_c(std::move(glyph_b));

        auto& data_b = glyph_b.Get_data();
        TEST_ASSERT(true, data_b.Is_null());

        auto& desc_b = glyph_b.Get_descriptor();
        TEST_ASSERT(0, desc_b.m_width);
        TEST_ASSERT(0, desc_b.m_height);
        TEST_ASSERT(0, desc_b.m_left);
        TEST_ASSERT(0, desc_b.m_top);
        TEST_ASSERT(0, desc_b.m_right);
        TEST_ASSERT(0, desc_b.m_bottom);
        TEST_ASSERT(0, desc_b.m_horizontal_advance);
        TEST_ASSERT(0, desc_b.m_vertical_advance);

        auto& data_c = glyph_c.Get_data();
        TEST_ASSERT(false, data_c.Is_null());
        for (size_t i = 0; i < size; ++i)
        {
            TEST_ASSERT(Platform::uint8(i), data_c.Data()[i]);
        }

        auto& desc_c = glyph_c.Get_descriptor();
        TEST_ASSERT(init_desc.m_width, desc_c.m_width);
        TEST_ASSERT(init_desc.m_height, desc_c.m_height);
        TEST_ASSERT(init_desc.m_left, desc_c.m_left);
        TEST_ASSERT(init_desc.m_top, desc_c.m_top);
        TEST_ASSERT(init_desc.m_right, desc_c.m_right);
        TEST_ASSERT(init_desc.m_bottom, desc_c.m_bottom);
        TEST_ASSERT(init_desc.m_horizontal_advance, desc_c.m_horizontal_advance);
        TEST_ASSERT(init_desc.m_vertical_advance, desc_c.m_vertical_advance);
    }
    
    glyph.Release();
    {
        auto& data = glyph.Get_data();
        TEST_ASSERT(true, data.Is_null());

        auto& desc = glyph.Get_descriptor();
        TEST_ASSERT(0, desc.m_width);
        TEST_ASSERT(0, desc.m_height);
        TEST_ASSERT(0, desc.m_left);
        TEST_ASSERT(0, desc.m_top);
        TEST_ASSERT(0, desc.m_right);
        TEST_ASSERT(0, desc.m_bottom);
        TEST_ASSERT(0, desc.m_horizontal_advance);
        TEST_ASSERT(0, desc.m_vertical_advance);
    }

    return Passed;
}

UNIT_TEST(Text_font_initial_state)
{
    Text::Font font;

    TEST_ASSERT((Text::Glyph *) 0, font.Get_glyph(0));

    TEST_ASSERT((Text::Glyph::Descriptor *) 0, font.Get_max());

    return Passed;
}

UNIT_TEST(Text_font_init)
{
    const Text::Glyph::Descriptor desc_0 = { 4, 4, 0, 2, 4, -2, 4, -6 };
    const Text::Glyph::Descriptor desc_1 = { 6, 4, 2, 4, 8, 0, 8, -6 };

    Text::Font font;
    Text::Font font_b;

    {
        const size_t desc_size = sizeof(Text::Glyph::Descriptor);
        const size_t nog = 2;
        const size_t char_size = nog * sizeof(Platform::uint32);
        const size_t descs_size = nog * desc_size;
        const size_t img_offs_size = nog * sizeof(Platform::uint64);

        const size_t img_0_size = desc_0.m_width * desc_0.m_height;
        const size_t img_1_size = desc_1.m_width * desc_1.m_height;
        const size_t imgs_size = img_0_size + img_1_size;

        const size_t total_size =
            sizeof(Platform::uint32) /* nog */
            + char_size
            + descs_size
            + img_offs_size
            + imgs_size;

        auto ptr = new Platform::uint8[total_size];

        /* NOG */
        Memory::Access::Write(ptr, 0, nog);

        /* Characters */
        const size_t off_char = sizeof(Platform::uint32);
        Memory::Access::Write(ptr, off_char, Platform::uint32(0));
        Memory::Access::Write(ptr, off_char + sizeof(Platform::uint32), Platform::uint32(1));

        /* Descriptors */
        const size_t off_descs = off_char + nog * sizeof(Platform::uint32);
        Memory::Access::Write(ptr, off_descs, desc_0);
        Memory::Access::Write(ptr, off_descs + desc_size, desc_0);

        /* Img offsets */
        const size_t off_img_offs = off_descs + nog * desc_size;
        Memory::Access::Write(ptr, off_img_offs, Platform::uint64(total_size - imgs_size));
        Memory::Access::Write(ptr, off_img_offs + sizeof(Platform::uint64), Platform::uint64(total_size - img_1_size));

        /* Img data */
        for (size_t i = 0; i < desc_0.m_width * desc_0.m_height; ++i)
        {
            ptr[off_img_offs + i] = 0;
        }
        for (size_t i = 0; i < desc_1.m_width * desc_1.m_height; ++i)
        {
            ptr[off_img_offs + img_0_size + i] = 1;
        }

        Memory::Binary_data data(ptr, total_size);
        if (Utilities::Success != font.Init(std::move(data), false))
        {
            return NotAvailable;
        }
    }

    {
        auto max = font.Get_max();
        TEST_ASSERT_NOT_EQUAL((Text::Glyph::Descriptor *) 0, max);

        TEST_ASSERT(6, max->m_width);
        TEST_ASSERT(4, max->m_height);
        TEST_ASSERT(0, max->m_left);
        TEST_ASSERT(4, max->m_top);
        TEST_ASSERT(8, max->m_right);
        TEST_ASSERT(-2, max->m_bottom);
        TEST_ASSERT(8, max->m_horizontal_advance);
        TEST_ASSERT(-6, max->m_vertical_advance);

        {
            auto glyph = font.Get_glyph(0);
            TEST_ASSERT_NOT_EQUAL((Text::Glyph *) 0, glyph);

            auto& desc = glyph->Get_descriptor();
            TEST_ASSERT(desc_0.m_width, desc.m_width);
            TEST_ASSERT(desc_0.m_height, desc.m_height);
            TEST_ASSERT(desc_0.m_left, desc.m_left);
            TEST_ASSERT(desc_0.m_top, desc.m_top);
            TEST_ASSERT(desc_0.m_right, desc.m_right);
            TEST_ASSERT(desc_0.m_bottom, desc.m_bottom);
            TEST_ASSERT(desc_0.m_horizontal_advance, desc.m_horizontal_advance);
            TEST_ASSERT(desc_0.m_vertical_advance, desc.m_vertical_advance);

            auto& data = glyph->Get_data();
            TEST_ASSERT(false, data.Is_null());

            for (size_t i = 0; i < desc.m_width * desc.m_height; ++i)
            {
                TEST_ASSERT(Platform::uint8(0), data.Data()[i]);
            }
        }

        {
            auto glyph = font.Get_glyph(1);
            TEST_ASSERT_NOT_EQUAL((Text::Glyph *) 0, glyph);

            auto& desc = glyph->Get_descriptor();
            TEST_ASSERT(desc_1.m_width, desc.m_width);
            TEST_ASSERT(desc_1.m_height, desc.m_height);
            TEST_ASSERT(desc_1.m_left, desc.m_left);
            TEST_ASSERT(desc_1.m_top, desc.m_top);
            TEST_ASSERT(desc_1.m_right, desc.m_right);
            TEST_ASSERT(desc_1.m_bottom, desc.m_bottom);
            TEST_ASSERT(desc_1.m_horizontal_advance, desc.m_horizontal_advance);
            TEST_ASSERT(desc_1.m_vertical_advance, desc.m_vertical_advance);

            auto& data = glyph->Get_data();
            TEST_ASSERT(false, data.Is_null());

            for (size_t i = 0; i < desc.m_width * desc.m_height; ++i)
            {
                TEST_ASSERT(Platform::uint8(1), data.Data()[i]);
            }
        }

        for (size_t i = 0; i < TEXT_FONT_CHAR_LUT_SIZE; ++i)
        {
            auto glyph = font.Get_glyph(2);
            TEST_ASSERT((Text::Glyph *) 0, glyph);
        }
    }

    font_b = std::move(font);
    {
        auto max = font.Get_max();
        auto max_b = font_b.Get_max();
        TEST_ASSERT((Text::Glyph::Descriptor *) 0, max);
        TEST_ASSERT_NOT_EQUAL((Text::Glyph::Descriptor *) 0, max_b);
    }

    static const Text::Box boxes[] = {
        { 0, 0, 2, -2 },
        { 0, 0, 4, -4 },
        { 0, 0, 4, -4 },
        { 0, 0, 6, -4 },
        { 0, 0, 6, -12 },
        { 0, 0, 12, -4 },
    };
    static const size_t boxes_count = sizeof(boxes) / sizeof(boxes[0]);
    Text::Font::character_t text[] = { 0, 1, 0, 1, 0, 1, 0 };
    static const size_t text_count = sizeof(text) / sizeof(text[0]);
    Text::Layout layout = { 0 };
    auto ret = Text::Init_layout(font_b, boxes, boxes_count, text, text_count, layout);

    return Passed;
}

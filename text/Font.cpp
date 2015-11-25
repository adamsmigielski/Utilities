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
* @file Font.cpp
**/

#include "PCH.hpp"
#include "Font.hpp"

#include <Utilities\memory\MemoryAccess.hpp>

#include <algorithm>
#include <map>

namespace Text
{
    using Map = std::map < Font::character_t, Glyph > ;


    class Font_pimpl
    {
    public:
        /* Ctr & dtr */
        Font_pimpl();
        ~Font_pimpl() = default;

        /* Copy */
        Font_pimpl(const Font_pimpl&) = delete;
        Font_pimpl & operator = (const Font_pimpl &) = delete;

        /* Members */
        Glyph m_ascii[TEXT_FONT_CHAR_LUT_SIZE];
        Map m_map;

        Glyph::Descriptor m_max;
    };

    Font_pimpl::Font_pimpl()
    {
        m_max = Glyph::Descriptor{ 0, 0, 0, 0, 0, 0, 0, 0 };
    }

    Font::Font()
        : m_pimpl(nullptr)
    {
        /* Nothing to be done */
    }

    Font::~Font()
    {
        Release();
    }

    Font::Font(Font && font)
    {
        m_pimpl = font.m_pimpl;
        font.m_pimpl = nullptr;
    }

    Font & Font::operator = (Font && font)
    {
        m_pimpl = font.m_pimpl;
        font.m_pimpl = nullptr;

        return *this;
    }

    Platform::int32 Font::Init()
    {
        auto ptr = new Font_pimpl;

        if (nullptr == ptr)
        {
            ERRLOG("Memory allocation failure");
            ASSERT(0);
            return Utilities::Failed_to_allocate_memory;
        }

        m_pimpl = ptr;

        return Utilities::Success;
    }

    /** \brief Unpacks glyphs from memory
     *
     * The memory layout is as follows:
     * Font::character_t - Number of glyphs - NOG
     * NOG * uint32 - characters
     * NOG * sizeof(Glyph::Descriptor) - descriptors
     * NOG * uint64 - image offsets
     * NOG * desc.width * desc.height - image data
     **/
    Platform::int32 Font::Init(
        Memory::Binary_data && data,
        bool is_endianess_swapped)
    {
        /* Clean up */
        Release();

        {
            auto ptr = new Font_pimpl;

            if (nullptr == ptr)
            {
                ERRLOG("Memory allocation failure");
                ASSERT(0);
                return Utilities::Failed_to_allocate_memory;
            }

            m_pimpl = ptr;
        }

        /* Get NOG */
        Platform::uint32 nog = 0;
        auto ret = Memory::Access::Read(data, 0, is_endianess_swapped, nog);
        if (Utilities::Success != ret)
        {
            ERRLOG("Corrupted resource");
            Release();
            return ret;
        }

        /* Calculate constants */
        const Platform::uint64 off_chars = sizeof(Platform::uint32);
        const Platform::uint64 size_chars = nog * sizeof(Font::character_t);
        const Platform::uint64 off_descs = off_chars + size_chars;
        const Platform::uint64 size_descs = nog * sizeof(Glyph::Descriptor);
        const Platform::uint64 off_img_offs = off_descs + size_descs;
        const Platform::uint64 size_img_offs = nog * sizeof(Platform::uint64);
        const Platform::uint64 off_imgs = off_img_offs + size_img_offs;

        /* Read each glyph */
        for (Platform::uint32 i = 0; i < nog; ++i)
        {
            /* Calculate constants */
            const Platform::uint64 off_char = off_chars + i * sizeof(Font::character_t);
            const Platform::uint64 off_desc = off_descs + i * sizeof(Glyph::Descriptor);
            const Platform::uint64 off_img_off = off_img_offs + i * sizeof(Platform::uint64);

            /* Get data */
            Font::character_t character = 0;
            Glyph::Descriptor descriptor = { 0 };
            Platform::uint64 off_img = 0;

            ret = Memory::Access::Read(data, off_char, is_endianess_swapped, character);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                Release();
                return ret;
            }

            ret = Memory::Access::Read(data, off_desc, descriptor);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                Release();
                return ret;
            }

            ret = Memory::Access::Read(data, off_img_off, is_endianess_swapped, off_img);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                Release();
                return ret;
            }

            if (true == is_endianess_swapped)
            {
                Memory::Access::Swap_endianess(descriptor.m_width);
                Memory::Access::Swap_endianess(descriptor.m_height);
                Memory::Access::Swap_endianess(descriptor.m_left);
                Memory::Access::Swap_endianess(descriptor.m_top);
                Memory::Access::Swap_endianess(descriptor.m_right);
                Memory::Access::Swap_endianess(descriptor.m_bottom);
                Memory::Access::Swap_endianess(descriptor.m_horizontal_advance);
                Memory::Access::Swap_endianess(descriptor.m_vertical_advance);
            }

            /* Get image data */
            const auto size = descriptor.m_width * descriptor.m_height;

            Memory::Binary_data img_data;
            img_data.Copy_range(data, off_img, size);

            ret = Add_glyph(character, descriptor, std::move(img_data));
            if (Utilities::Success != ret)
            {
                Release();
                return ret;
            }

            /* Select max */
            if (0 != i)
            {
                m_pimpl->m_max.m_width = std::max(m_pimpl->m_max.m_width, descriptor.m_width);
                m_pimpl->m_max.m_height = std::max(m_pimpl->m_max.m_height, descriptor.m_height);
                m_pimpl->m_max.m_left = std::min(m_pimpl->m_max.m_left, descriptor.m_left);
                m_pimpl->m_max.m_top = std::max(m_pimpl->m_max.m_top, descriptor.m_top);
                m_pimpl->m_max.m_right = std::max(m_pimpl->m_max.m_right, descriptor.m_right);
                m_pimpl->m_max.m_bottom = std::min(m_pimpl->m_max.m_bottom, descriptor.m_bottom);
                m_pimpl->m_max.m_horizontal_advance = std::max(m_pimpl->m_max.m_horizontal_advance, descriptor.m_horizontal_advance);
                m_pimpl->m_max.m_vertical_advance = std::min(m_pimpl->m_max.m_vertical_advance, descriptor.m_vertical_advance);
            }
            else
            {
                m_pimpl->m_max = descriptor;
            }
        }

        return Utilities::Success;
    }

    Platform::int32 Font::Store(Memory::Binary_data & out_result) const
    {
        if (nullptr == m_pimpl)
        {
            return Utilities::Invalid_object;
        }

        /* Get NOG & image data size */
        Platform::uint32 nog = m_pimpl->m_map.size();
        Platform::uint64 image_data_size = 0;

        for (Platform::uint32 i = 0; i < TEXT_FONT_CHAR_LUT_SIZE; ++i)
        {
            auto glyph = Get_glyph_raw(i);

            if (nullptr != glyph)
            {
                auto size = glyph->Get_data().Size();

                ++nog;
                image_data_size += size;
            }
        }

        for (auto it : m_pimpl->m_map)
        {
            auto size = it.second.Get_data().Size();
            image_data_size += size;
        }


        /* Calculate constants */
        const Platform::uint64 off_chars = sizeof(Platform::uint32);
        const Platform::uint64 size_chars = nog * sizeof(Font::character_t);
        const Platform::uint64 off_descs = off_chars + size_chars;
        const Platform::uint64 size_descs = nog * sizeof(Glyph::Descriptor);
        const Platform::uint64 off_img_offs = off_descs + size_descs;
        const Platform::uint64 size_img_offs = nog * sizeof(Platform::uint64);
        const Platform::uint64 off_imgs = off_img_offs + size_img_offs;
        const Platform::uint64 memory_req = off_imgs + image_data_size;

        /* Allocate memory */
        auto ptr = new Platform::uint8[size_t(memory_req)];
        if (nullptr == ptr)
        {
            ASSERT(0);
            return Utilities::Failed_to_allocate_memory;
        }
        out_result.Reset(ptr, memory_req);

        /* Store NOG */
        auto ret = Memory::Access::Write(out_result, 0, nog);
        if (Utilities::Success != ret)
        {
            ERRLOG("Corrupted resource");
            return ret;
        }

        /* Store each glyph */
        Platform::uint64 off_img = off_imgs;
        Platform::uint32 index = 0;

        auto write_glyph = [&](
            character_t character,
            const Glyph * glyph,
            Platform::uint32 index,
            Platform::uint64 & off_img)
            -> Platform::int32
        {
            /* Calculate constants */
            const Platform::uint64 off_char = off_chars + index * sizeof(Font::character_t);
            const Platform::uint64 off_desc = off_descs + index * sizeof(Glyph::Descriptor);
            const Platform::uint64 off_img_off = off_img_offs + index * sizeof(Platform::uint64);

            /* Get size */
            auto size = glyph->Get_data().Size();

            /* Store data */
            auto ret = Memory::Access::Write(out_result, off_char, character);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            ret = Memory::Access::Write(out_result, off_desc, glyph->Get_descriptor());
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            ret = Memory::Access::Write(out_result, off_img_off, off_img);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            /* Glyph may be empty, skip images with size 0 */
            if (0 != size)
            {
                ret = Memory::Access::Write(out_result, off_img, glyph->Get_data().Data(), size);
                if (Utilities::Success != ret)
                {
                    ERRLOG("Corrupted resource");
                    return ret;
                }
            }

            off_img += size;

            return Utilities::Success;
        };


        for (Platform::uint32 i = 0; i < TEXT_FONT_CHAR_LUT_SIZE; ++i)
        {
            const Glyph * glyph = Get_glyph_raw(i);
            
            if (nullptr == glyph)
            {
                continue;
            }

            auto ret = write_glyph(i, glyph, index, off_img);
            if (Utilities::Success != ret)
            {
                ASSERT(0);
                return ret;
            }

            ++index;
        }

        for (auto it : m_pimpl->m_map)
        {
            const Glyph * glyph = &it.second;

            auto ret = write_glyph(it.first, glyph, index, off_img);
            if (Utilities::Success != ret)
            {
                ASSERT(0);
                return ret;
            }

            ++index;
        }

        return Utilities::Success;
    }

    void Font::Release()
    {
        if (nullptr != m_pimpl)
        {
            delete m_pimpl;
            m_pimpl = nullptr;
        }
    }

    Platform::int32 Font::Add_glyph(
        const Font::character_t character,
        const Glyph::Descriptor & descriptor,
        Memory::Binary_data && image)
    {
        if (nullptr == m_pimpl)
        {
            ASSERT(0);
            return Utilities::Invalid_object;
        }

        /* Store glyph */
        if (TEXT_FONT_CHAR_LUT_SIZE > character)
        {
            m_pimpl->m_ascii[character].Init(std::move(image), descriptor);
        }
        else
        {
            m_pimpl->m_map[character].Init(std::move(image), descriptor);
        }

        return Utilities::Success;
    }

    const Glyph * Font::Get_glyph(character_t character) const
    {
        if (nullptr == m_pimpl)
        {
            ASSERT(0);
            return nullptr;
        }

        const Glyph * ptr = Get_glyph_raw(character);

        if (nullptr != ptr)
        {
            return ptr;
        }
        else
        {
            return &m_pimpl->m_ascii[0];
        }
    }

    const Glyph * Font::Get_glyph_raw(character_t character) const
    {
        if (nullptr == m_pimpl)
        {
            ASSERT(0);
            return nullptr;
        }

        const Glyph * ptr = nullptr;

        if (TEXT_FONT_CHAR_LUT_SIZE > character)
        {
            auto tmp = &m_pimpl->m_ascii[character];

            if (false == tmp->Get_data().Is_null())
            {
                ptr = tmp;
            }
        }
        else
        {
            auto it = m_pimpl->m_map.find(character);

            if (m_pimpl->m_map.end() != it)
            {
                ptr = &it->second;
            }
        }
            
        return ptr;
    }

    const Glyph::Descriptor * Font::Get_max() const
    {
        if (nullptr == m_pimpl)
        {
            return nullptr;
        }

        return &m_pimpl->m_max;
    }
}

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
* @file Float.hpp
**/

#include "PCH.hpp"
#include "Font.hpp"
#include <map>
#include "Glyph.hpp"

#include <Utilities\memory\MemoryAccess.hpp>

namespace Text
{
    using Map = std::map < Font::character_t, Glyph > ;

#define TEXT_FONT_CHAR_LUT_SIZE 128

    class Font_pimpl
    {
    public:
        Glyph m_ascii[TEXT_FONT_CHAR_LUT_SIZE];
        Map m_map;
    };

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

            m_pimpl.reset(ptr);
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
        const size_t off_chars = sizeof(Platform::uint32);
        const size_t size_chars = nog * sizeof(Font::character_t);
        const size_t off_descs = off_chars + size_chars;
        const size_t size_descs = nog * sizeof(Glyph::Descriptor);
        const size_t off_img_offs = off_descs + size_descs;
        const size_t size_img_offs = nog * sizeof(Platform::uint64);
        const size_t off_imgs = off_img_offs + size_img_offs;

        /* Read each glyph */
        for (Platform::uint32 i = 0; i < nog; ++i)
        {
            /* Calculate constants */
            const size_t off_char = off_chars + i * sizeof(Font::character_t);
            const size_t off_desc = off_descs + i * sizeof(Glyph::Descriptor);
            const size_t off_img_off = off_img_offs + i * sizeof(Platform::uint64);

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

            /* Store glyph */
            if (TEXT_FONT_CHAR_LUT_SIZE > character)
            {
                m_pimpl->m_ascii[character].Init(std::move(img_data), descriptor);
            }
            else
            {
                m_pimpl->m_map[character].Init(std::move(img_data), descriptor);
            }
        }

        return Utilities::Success;
    }

    Memory::Binary_data Font::Store() const
    {
        Memory::Binary_data result;

        if (nullptr == m_pimpl.get())
        {
            return result;
        }

        /* Get NOG & image data size */
        Platform::uint32 nog = m_pimpl->m_map.size();
        Platform::uint64 image_data_size = 0;

        for (Platform::uint32 i = 0; i < TEXT_FONT_CHAR_LUT_SIZE; ++i)
        {
            auto glyph = Get_glyph(i);

            auto size = glyph->Get_data().Size();

            if (0 != size)
            {
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
        const size_t off_chars = sizeof(Platform::uint32);
        const size_t size_chars = nog * sizeof(Font::character_t);
        const size_t off_descs = off_chars + size_chars;
        const size_t size_descs = nog * sizeof(Glyph::Descriptor);
        const size_t off_img_offs = off_descs + size_descs;
        const size_t size_img_offs = nog * sizeof(Platform::uint64);
        const size_t off_imgs = off_img_offs + size_img_offs;
        const size_t memory_req = off_imgs + image_data_size;

        /* Allocate memory */
        auto ptr = new Platform::uint8[memory_req];
        result.Reset(ptr, memory_req);

        /* Store each glyph */
        size_t off_img = off_imgs;
        Platform::uint32 index = 0;

        auto write_glyph = [=](
            character_t character,
            const Glyph * glyph,
            Platform::uint32 index,
            size_t & off_img)
            -> Platform::int32
        {
            /* Calculate constants */
            const size_t off_char = off_chars + index * sizeof(Font::character_t);
            const size_t off_desc = off_descs + index * sizeof(Glyph::Descriptor);
            const size_t off_img_off = off_img_offs + index * sizeof(Platform::uint64);

            /* Get size */
            auto size = glyph->Get_data().Size();

            /* Skip glyph if there is no image */
            if (0 == size)
            {
                return Utilities::Success;
            }

            /* Store data */
            auto ret = Memory::Access::Write(result, off_char, character);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            ret = Memory::Access::Write(result, off_desc, glyph->Get_descriptor());
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            ret = Memory::Access::Write(result, off_img_off, off_img);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            ret = Memory::Access::Write(result, off_img, glyph->Get_data().Data(), size);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                return ret;
            }

            off_img += size;

            return Utilities::Success;
        };


        for (Platform::uint32 i = 0; i < TEXT_FONT_CHAR_LUT_SIZE; ++i)
        {
            const Glyph * glyph = m_pimpl->m_ascii + i;

            auto ret = write_glyph(i, glyph, index, off_img);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                ASSERT(0);
                return result;
            }

            ++index;
        }

        for (auto it : m_pimpl->m_map)
        {
            const Glyph * glyph = &it.second;

            auto ret = write_glyph(it.first, glyph, index, off_img);
            if (Utilities::Success != ret)
            {
                ERRLOG("Corrupted resource");
                ASSERT(0);
                return result;
            }

            ++index;
        }

        return result;
    }

    void Font::Release()
    {
        if (nullptr != m_pimpl.get())
        {
            m_pimpl.release();
        }
    }

    const Glyph * Font::Get_glyph(character_t character) const
    {
        if (nullptr == m_pimpl.get())
        {
            return nullptr;
        }

        if (TEXT_FONT_CHAR_LUT_SIZE > character)
        {
            return &m_pimpl->m_ascii[character];
        }
        else
        {
            auto it = m_pimpl->m_map.find(character);

            if (m_pimpl->m_map.end() != it)
            {
                return &it->second;
            }
            else
            {
                return nullptr;
            }
        }
    }
}

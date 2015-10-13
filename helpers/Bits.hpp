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
* @file Bits.hpp
**/

namespace Helpers
{
    template < typename T >
    struct Binary_operations
    {
        typedef T Base_type;

        static Base_type And(
            const Base_type & l,
            const Base_type & r)
        {
            return (l & r);
        }

        static Base_type Or(
            const Base_type & l,
            const Base_type & r)
        {
            return (l | r);
        }

        static Base_type Xor(
            const Base_type & l,
            const Base_type & r)
        {
            return (l ^ r);
        }
    };

    template < typename T >
    struct Masks
    {
        typedef T Base_type;

        static const Base_type m_None = Base_type(0);
        static const Base_type m_One  = Base_type(1);
        static const Base_type m_All  = ~(m_None);
    };

    template <
        typename T,
        unsigned int bit>
    struct Bit
    {
        typedef T Base_type;
        typedef Masks< Base_type > Masks;

        static Base_type Get(const Base_type & raw)
        {
            return (raw & m_Bit) >> m_Offset;
        }

        static Base_type Set(
            const Base_type & value,
            const Base_type & raw)
        {
            const Base_type unset_raw = raw & m_Bit_inv;
            const Base_type selected_bit = (value << m_Offset) & m_Bit;
            const Base_type result = unset_raw | selected_bit;

            return result;
        }

        static const unsigned int m_Offset    = bit;
        static const Base_type m_Bit          = Base_type(Masks::m_One << bit);
        static const Base_type m_Bit_inv      = Base_type(~m_Bit);
        static const Base_type m_MSE_bits     = Base_type(Masks::m_All << bit);
        static const Base_type m_MSE_inv_bits = Base_type(~m_MSE_bits);
        static const Base_type m_LSE_bits     = Base_type(m_MSE_inv_bits | m_Bit);
        static const Base_type m_LSE_inv_bits = Base_type(~m_LSE_bits);
    };

    template <
        typename T,
        unsigned int ms,
        unsigned int ls >
    struct Mask
    {
        typedef T Base_type;
        typedef Binary_operations< Base_type > Binary_operations;
        typedef Masks< Base_type > Masks;
        typedef Bit< T, ms > MS_bit;
        typedef Bit< T, ls > LS_bit;

        static const Base_type    m_Mask        = MS_bit::m_LSE_bits & LS_bit::m_MSE_bits;
        static const Base_type    m_Mask_inv    = ~m_Mask;
        static const unsigned int m_Mask_length = ms - ls;
        static const Base_type    m_Mask_value  = m_Mask >> LS_bit::m_Offset;

        static Base_type Get(const Base_type & raw)
        {
            const Base_type masked_raw = Binary_operations::And(raw, m_Mask);
            const Base_type result = (masked_raw >> LS_bit::m_Offset);

            return result;
        }

        static Base_type Set(
            const Base_type & value,
            const Base_type & raw)
        {
            const Base_type unset_raw = raw & m_Mask_inv;
            const Base_type selected_bits = (value << LS_bit::m_Offset) & m_Mask;
            const Base_type result = unset_raw | selected_bits;

            return result;
        }
    };
}
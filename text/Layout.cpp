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

#ifndef MATH_FLOAT_HPP
#define MATH_FLOAT_HPP

#include <Utilities\basic\ErrorCodes.hpp>
#include <Utilities\helpers\Bits.hpp>
#include <Utilities\helpers\Type.hpp>

namespace Math
{
    template <
        typename T,
        unsigned int sign_bits,
        unsigned int exponent_bits,
        unsigned int exponent_bias,
        unsigned int significand_bits >
    class Float
    {
    public:
        typedef T Base_type;
        typedef Helpers::Masks < T > Masks;
        typedef Helpers::Type < T > Type;
        typedef Helpers::Bit <
            Base_type,
            exponent_bits + significand_bits >
            Sign_bit;
        typedef Helpers::Mask <
            Base_type,
            exponent_bits + significand_bits - 1,
            significand_bits>
            Exponent_mask;
        typedef Helpers::Mask <
            Base_type,
            significand_bits - 1,
            0>
            Significand_mask;


        Float()
            : m_Raw(0)
        {
        }
        Float(const Float & _float)
            : m_Raw(_float.m_Raw)
        {
        }
        Float(const Base_type & raw)
            : m_Raw(raw)
        {
        }
        Float(const double & value)
        {
            Value(value);
        }

        static typename Type::difference_type Get_exponent(const Base_type & raw)
        {
            const typename Type::difference_type raw_exponent( Exponent_mask::Get(raw));
            const typename Type::difference_type biased_exponent(raw_exponent - m_Exponent_bias);

            return biased_exponent;
        }

        static Base_type Set_exponent(
            const typename Type::difference_type & exponent,
            const Base_type & raw)
        {
            const Base_type biased_exponent(exponent + m_Exponent_bias);

            return Exponent_mask::Set(biased_exponent, raw);
        }

        static Base_type Get_significand(const Base_type & raw)
        {
            return Significand_mask::Get(raw);
        }

        static Base_type Set_significand(
            const Base_type & significand,
            const Base_type & raw)
        {
            return Significand_mask::Set(significand, raw);
        }

        static Base_type Get_sign(const Base_type & raw)
        {
            Base_type sign = 0;

            if (true == m_Has_sign)
            {
                sign = Sign_bit::Get(raw);
            }

            return sign;
        }

        static Base_type Set_sign(
            const Base_type & sign,
            const Base_type & raw)
        {
            Base_type result = 0;

            if (true == m_Has_sign)
            {
                result = Sign_bit::Set(sign, raw);
            }

            return result;
        }

        Platform::int32 Value(const double & value)
        {
            auto base_64     = (typename float64::Base_type *) &value;
            auto exponent_64 = float64::Get_exponent(*base_64);

            if (m_Exponent_max < exponent_64)
            {
                return Utilities::Invalid_parameter;
            }

            auto sign_64        = float64::Get_sign       (*base_64);
            auto significand_64 = float64::Get_significand(*base_64);

            auto exponent    = exponent_64;
            auto sign        = Float::Base_type(sign_64);
            auto significand = Float::Base_type(significand_64 >> m_Significand_shift);
            auto point       = significand_64 & ((float64::Masks::m_One << m_Significand_shift) >> 1);

            if (-Type::difference_type(m_Exponent_bias) > exponent_64)
            {
                exponent = -Type::difference_type(m_Exponent_bias);
            }

            if (0 != point)
            {
                auto half_bits = significand_64 & ( (~ (float64::Masks::m_All << m_Significand_shift) ) >> 1);

                if (0 != half_bits)
                {
                    significand += 1;
                }
            }


            m_Raw = 
                Set_significand(
                    significand,
                    Set_exponent(
                        exponent,
                        Set_sign(
                            sign,
                            Float::Base_type(0)
                        )
                    )
                );

            return Utilities::Success;
        }

        double Value() const
        {
            auto exponent    = Float::Get_exponent(m_Raw);
            auto sign        = Float::Get_sign(m_Raw);
            auto significand = Float::Get_significand(m_Raw);

            float64::Type::difference_type exponent_64(exponent);
            float64::Base_type             sign_64(sign);
            float64::Base_type             significand_64 =
                float64::Base_type(significand) << m_Significand_shift;

            if (exponent == -Float::Type::difference_type(m_Exponent_bias))
            {
                exponent_64 = -Float::Type::difference_type(float64::m_Exponent_bias);
            }

            float64::Base_type base_64 =
                float64::Set_significand(
                    significand_64,
                    float64::Set_exponent(
                        exponent_64,
                        float64::Set_sign(
                            sign_64,
                            float64::Base_type(0)
                        )
                    )
                );

            double result = *(double *) &base_64;

            return result;
        }

        Base_type m_Raw;

        /* Constants */
        static const bool m_Has_sign = (0 == sign_bits)
                                     ? false
                                     : true;;
        static const Base_type          m_Exponent_bias           = Base_type(exponent_bias);
        static const Base_type          m_Exponent_max            = Exponent_mask::m_Mask_value - m_Exponent_bias - 1;
        static const Base_type          m_Significand_max         = Significand_mask::m_Mask_value;
        static const Base_type          m_Significand_shift       = float64::Significand_mask::m_Mask_length - Significand_mask::m_Mask_length;
    };



    typedef Float <
        Platform::uint16 /* base */,
        1                /* sign */,
        5                /* exponent bits */,
        15               /* exponent bias */,
        10               /* significand bits */
    > float16;
    typedef float16 half;
    typedef Float <
        Platform::uint32 /* base */,
        1                /* sign */,
        8                /* exponent bits */,
        127              /* exponent bias */,
        23               /* significand bits */
    > float32;
    typedef Float <
        Platform::uint64 /* base */,
        1                /* sign */,
        11               /* exponent bits */,
        1023            /* exponent bias */,
        52               /* significand bits */
    > float64;
}

#endif /* MATH_FLOAT_HPP */

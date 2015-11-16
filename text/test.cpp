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

#include "Float.hpp"

#include <cstring>

UNIT_TEST(Float16_constants)
{
    TEST_ASSERT(true, Math::float16::m_Has_sign);

    /* Sign */
    TEST_ASSERT(0x8000, Math::float16::Sign_bit::m_Bit);
    TEST_ASSERT(0x7fff, Math::float16::Sign_bit::m_Bit_inv);
    TEST_ASSERT(0x8000, Math::float16::Sign_bit::m_MSE_bits);
    TEST_ASSERT(0x7fff, Math::float16::Sign_bit::m_MSE_inv_bits);
    TEST_ASSERT(0xffff, Math::float16::Sign_bit::m_LSE_bits);
    TEST_ASSERT(0x0000, Math::float16::Sign_bit::m_LSE_inv_bits);

    /* Exponent */
    TEST_ASSERT(0xffff, Math::float16::Exponent_mask::Masks::m_All);
    TEST_ASSERT(0x0001, Math::float16::Exponent_mask::Masks::m_One);
    TEST_ASSERT(0x0000, Math::float16::Exponent_mask::Masks::m_None);

    TEST_ASSERT(0x4000, Math::float16::Exponent_mask::MS_bit::m_Bit);
    TEST_ASSERT(0xbfff, Math::float16::Exponent_mask::MS_bit::m_Bit_inv);
    TEST_ASSERT(0xc000, Math::float16::Exponent_mask::MS_bit::m_MSE_bits);
    TEST_ASSERT(0x3fff, Math::float16::Exponent_mask::MS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x7fff, Math::float16::Exponent_mask::MS_bit::m_LSE_bits);
    TEST_ASSERT(0x8000, Math::float16::Exponent_mask::MS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x0400, Math::float16::Exponent_mask::LS_bit::m_Bit);
    TEST_ASSERT(0xfbff, Math::float16::Exponent_mask::LS_bit::m_Bit_inv);
    TEST_ASSERT(0xfc00, Math::float16::Exponent_mask::LS_bit::m_MSE_bits);
    TEST_ASSERT(0x03ff, Math::float16::Exponent_mask::LS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x07ff, Math::float16::Exponent_mask::LS_bit::m_LSE_bits);
    TEST_ASSERT(0xf800, Math::float16::Exponent_mask::LS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x7c00, Math::float16::Exponent_mask::m_Mask);
    TEST_ASSERT(0x001f, Math::float16::Exponent_mask::m_Mask_value);

    /* Significand */
    TEST_ASSERT(0xffff, Math::float16::Significand_mask::Masks::m_All);
    TEST_ASSERT(0x0001, Math::float16::Significand_mask::Masks::m_One);
    TEST_ASSERT(0x0000, Math::float16::Significand_mask::Masks::m_None);

    TEST_ASSERT(0x0200, Math::float16::Significand_mask::MS_bit::m_Bit);
    TEST_ASSERT(0xfdff, Math::float16::Significand_mask::MS_bit::m_Bit_inv);
    TEST_ASSERT(0xfe00, Math::float16::Significand_mask::MS_bit::m_MSE_bits);
    TEST_ASSERT(0x01ff, Math::float16::Significand_mask::MS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x03ff, Math::float16::Significand_mask::MS_bit::m_LSE_bits);
    TEST_ASSERT(0xfc00, Math::float16::Significand_mask::MS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x0001, Math::float16::Significand_mask::LS_bit::m_Bit);
    TEST_ASSERT(0xfffe, Math::float16::Significand_mask::LS_bit::m_Bit_inv);
    TEST_ASSERT(0xffff, Math::float16::Significand_mask::LS_bit::m_MSE_bits);
    TEST_ASSERT(0x0000, Math::float16::Significand_mask::LS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x0001, Math::float16::Significand_mask::LS_bit::m_LSE_bits);
    TEST_ASSERT(0xfffe, Math::float16::Significand_mask::LS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x03ff, Math::float16::Significand_mask::m_Mask);
    TEST_ASSERT(0x03ff, Math::float16::Significand_mask::m_Mask_value);

    return Passed;
}

UNIT_TEST(Float32_constants)
{
    TEST_ASSERT(true, Math::float32::m_Has_sign);

    /* Sign */
    TEST_ASSERT(0x80000000, Math::float32::Sign_bit::m_Bit);
    TEST_ASSERT(0x7fffffff, Math::float32::Sign_bit::m_Bit_inv);
    TEST_ASSERT(0x80000000, Math::float32::Sign_bit::m_MSE_bits);
    TEST_ASSERT(0x7fffffff, Math::float32::Sign_bit::m_MSE_inv_bits);
    TEST_ASSERT(0xffffffff, Math::float32::Sign_bit::m_LSE_bits);
    TEST_ASSERT(0x00000000, Math::float32::Sign_bit::m_LSE_inv_bits);

    /* Exponent */
    TEST_ASSERT(0xffffffff, Math::float32::Exponent_mask::Masks::m_All);
    TEST_ASSERT(0x00000001, Math::float32::Exponent_mask::Masks::m_One);
    TEST_ASSERT(0x00000000, Math::float32::Exponent_mask::Masks::m_None);

    TEST_ASSERT(0x40000000, Math::float32::Exponent_mask::MS_bit::m_Bit);
    TEST_ASSERT(0xbfffffff, Math::float32::Exponent_mask::MS_bit::m_Bit_inv);
    TEST_ASSERT(0xc0000000, Math::float32::Exponent_mask::MS_bit::m_MSE_bits);
    TEST_ASSERT(0x3fffffff, Math::float32::Exponent_mask::MS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x7fffffff, Math::float32::Exponent_mask::MS_bit::m_LSE_bits);
    TEST_ASSERT(0x80000000, Math::float32::Exponent_mask::MS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x00800000, Math::float32::Exponent_mask::LS_bit::m_Bit);
    TEST_ASSERT(0xff7fffff, Math::float32::Exponent_mask::LS_bit::m_Bit_inv);
    TEST_ASSERT(0xff800000, Math::float32::Exponent_mask::LS_bit::m_MSE_bits);
    TEST_ASSERT(0x007fffff, Math::float32::Exponent_mask::LS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x00ffffff, Math::float32::Exponent_mask::LS_bit::m_LSE_bits);
    TEST_ASSERT(0xff000000, Math::float32::Exponent_mask::LS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x7f800000, Math::float32::Exponent_mask::m_Mask);
    TEST_ASSERT(0x000000ff, Math::float32::Exponent_mask::m_Mask_value);

    /* Significand */
    TEST_ASSERT(0xffffffff, Math::float32::Significand_mask::Masks::m_All);
    TEST_ASSERT(0x00000001, Math::float32::Significand_mask::Masks::m_One);
    TEST_ASSERT(0x00000000, Math::float32::Significand_mask::Masks::m_None);

    TEST_ASSERT(0x00400000, Math::float32::Significand_mask::MS_bit::m_Bit);
    TEST_ASSERT(0xffbfffff, Math::float32::Significand_mask::MS_bit::m_Bit_inv);
    TEST_ASSERT(0xffc00000, Math::float32::Significand_mask::MS_bit::m_MSE_bits);
    TEST_ASSERT(0x003fffff, Math::float32::Significand_mask::MS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x007fffff, Math::float32::Significand_mask::MS_bit::m_LSE_bits);
    TEST_ASSERT(0xff800000, Math::float32::Significand_mask::MS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x00000001, Math::float32::Significand_mask::LS_bit::m_Bit);
    TEST_ASSERT(0xfffffffe, Math::float32::Significand_mask::LS_bit::m_Bit_inv);
    TEST_ASSERT(0xffffffff, Math::float32::Significand_mask::LS_bit::m_MSE_bits);
    TEST_ASSERT(0x00000000, Math::float32::Significand_mask::LS_bit::m_MSE_inv_bits);
    TEST_ASSERT(0x00000001, Math::float32::Significand_mask::LS_bit::m_LSE_bits);
    TEST_ASSERT(0xfffffffe, Math::float32::Significand_mask::LS_bit::m_LSE_inv_bits);

    TEST_ASSERT(0x007fffff, Math::float32::Significand_mask::m_Mask);
    TEST_ASSERT(0x007fffff, Math::float32::Significand_mask::m_Mask_value);

    return Passed;
}

UNIT_TEST(Float64_constants)
{
    TEST_ASSERT(true, Math::float64::m_Has_sign);

    /* Sign */
    TEST_ASSERT(0x8000000000000000, Math::float64::Sign_bit::m_Bit);                          
    TEST_ASSERT(0x7fffffffffffffff, Math::float64::Sign_bit::m_Bit_inv);                      
    TEST_ASSERT(0x8000000000000000, Math::float64::Sign_bit::m_MSE_bits);                     
    TEST_ASSERT(0x7fffffffffffffff, Math::float64::Sign_bit::m_MSE_inv_bits);                 
    TEST_ASSERT(0xffffffffffffffff, Math::float64::Sign_bit::m_LSE_bits);                     
    TEST_ASSERT(0x0000000000000000, Math::float64::Sign_bit::m_LSE_inv_bits);                 

    /* Exponent */
    TEST_ASSERT(0xffffffffffffffff, Math::float64::Exponent_mask::Masks::m_All);              
    TEST_ASSERT(0x0000000000000001, Math::float64::Exponent_mask::Masks::m_One);
    TEST_ASSERT(0x0000000000000000, Math::float64::Exponent_mask::Masks::m_None);

    TEST_ASSERT(0x4000000000000000, Math::float64::Exponent_mask::MS_bit::m_Bit);             
    TEST_ASSERT(0xbfffffffffffffff, Math::float64::Exponent_mask::MS_bit::m_Bit_inv);         
    TEST_ASSERT(0xc000000000000000, Math::float64::Exponent_mask::MS_bit::m_MSE_bits);        
    TEST_ASSERT(0x3fffffffffffffff, Math::float64::Exponent_mask::MS_bit::m_MSE_inv_bits);    
    TEST_ASSERT(0x7fffffffffffffff, Math::float64::Exponent_mask::MS_bit::m_LSE_bits);        
    TEST_ASSERT(0x8000000000000000, Math::float64::Exponent_mask::MS_bit::m_LSE_inv_bits);    

    TEST_ASSERT(0x0010000000000000, Math::float64::Exponent_mask::LS_bit::m_Bit);             
    TEST_ASSERT(0xffefffffffffffff, Math::float64::Exponent_mask::LS_bit::m_Bit_inv);         
    TEST_ASSERT(0xfff0000000000000, Math::float64::Exponent_mask::LS_bit::m_MSE_bits);        
    TEST_ASSERT(0x000fffffffffffff, Math::float64::Exponent_mask::LS_bit::m_MSE_inv_bits);    
    TEST_ASSERT(0x001fffffffffffff, Math::float64::Exponent_mask::LS_bit::m_LSE_bits);        
    TEST_ASSERT(0xffe0000000000000, Math::float64::Exponent_mask::LS_bit::m_LSE_inv_bits);    

    TEST_ASSERT(0x7ff0000000000000, Math::float64::Exponent_mask::m_Mask);
    TEST_ASSERT(0x00000000000007ff, Math::float64::Exponent_mask::m_Mask_value);

    /* Significand */
    TEST_ASSERT(0xffffffffffffffff, Math::float64::Significand_mask::Masks::m_All);           
    TEST_ASSERT(0x0000000000000001, Math::float64::Significand_mask::Masks::m_One);
    TEST_ASSERT(0x0000000000000000, Math::float64::Significand_mask::Masks::m_None);

    TEST_ASSERT(0x0008000000000000, Math::float64::Significand_mask::MS_bit::m_Bit);          
    TEST_ASSERT(0xfff7ffffffffffff, Math::float64::Significand_mask::MS_bit::m_Bit_inv);      
    TEST_ASSERT(0xfff8000000000000, Math::float64::Significand_mask::MS_bit::m_MSE_bits);     
    TEST_ASSERT(0x0007ffffffffffff, Math::float64::Significand_mask::MS_bit::m_MSE_inv_bits); 
    TEST_ASSERT(0x000fffffffffffff, Math::float64::Significand_mask::MS_bit::m_LSE_bits);     
    TEST_ASSERT(0xfff0000000000000, Math::float64::Significand_mask::MS_bit::m_LSE_inv_bits); 

    TEST_ASSERT(0x0000000000000001, Math::float64::Significand_mask::LS_bit::m_Bit);          
    TEST_ASSERT(0xfffffffffffffffe, Math::float64::Significand_mask::LS_bit::m_Bit_inv);      
    TEST_ASSERT(0xffffffffffffffff, Math::float64::Significand_mask::LS_bit::m_MSE_bits);     
    TEST_ASSERT(0x0000000000000000, Math::float64::Significand_mask::LS_bit::m_MSE_inv_bits); 
    TEST_ASSERT(0x0000000000000001, Math::float64::Significand_mask::LS_bit::m_LSE_bits);     
    TEST_ASSERT(0xfffffffffffffffe, Math::float64::Significand_mask::LS_bit::m_LSE_inv_bits); 

    TEST_ASSERT(0x000fffffffffffff, Math::float64::Significand_mask::m_Mask);
    TEST_ASSERT(0x000fffffffffffff, Math::float64::Significand_mask::m_Mask_value);

    return Passed;
}

UNIT_TEST(Float_usage)
{
    static const double values[] =
    {
        123.456,
        0.0,
        1.0,
        -0.0,
        -1.0
    };
    static const size_t n_values = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < n_values; ++i)
    {
        const float val = float(values[i]);
        const Math::float32 f(values[i]);

        TEST_ASSERT(0, memcmp(&val, &(f.m_Raw), sizeof(float)));

        const double d_val(val);
        const double d_f(f.Value());

        TEST_ASSERT(d_val, d_f);
    }

    return Passed;
}

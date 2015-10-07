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
 * @file Float4.hpp
 **/

#ifndef O8_MATH_FLOAT4_HPP
#define O8_MATH_FLOAT4_HPP

#include "FloatTypes.hpp"

namespace O8
{
	namespace Math
	{
		//Creation
		namespace Float4
		{
			static const float4 one = {{1.0f, 1.0f, 1.0f, 1.0f}};

			inline float4 Zero()
			{
				float4 res;

				res.m128 = _mm_setzero_ps();

				return res;
			}

			inline float4 One()
			{
				return one;
			}

			inline float4 Set(float val)
			{
				float4 res;

				res.m128 = _mm_set1_ps(val);

				return res;
			}

			inline float4 Set(float x, float y, float z, float w)
			{
				float4 res;

				res.m128 = _mm_set_ps(w, z, y, x);

				return res;
			}
		}
		
		inline float4 operator + (const float4 & a, const float4 & b)
		{
			float4 res;

			res.m128 = _mm_add_ps(a.m128, b.m128);

			return res;
		}
		
		inline float4 operator - (const float4 & a, const float4 & b)
		{
			float4 res;

			res.m128 = _mm_sub_ps(a.m128, b.m128);

			return res;
		}
		
		inline float4 operator * (const float4 & a, const float4 & b)
		{
			float4 res;

			res.m128 = _mm_mul_ps(a.m128, b.m128);

			return res;
		}
		
		inline float4 operator * (const float4 & a, const float b)
		{
			float4 res, scale;

			scale.m128 = _mm_set1_ps(b);
			res.m128 = _mm_mul_ps(a.m128, scale.m128);

			return res;
		}
		
		inline float4 operator / (const float4 & a, const float4 & b)
		{
			float4 res;

			res.m128 = _mm_div_ps(a.m128, b.m128);

			return res;
		}
		
		inline float4 operator / (const float4 & a, const float b)
		{
			float4 res, scale;

			scale.m128 = _mm_set1_ps(b);
			scale.m128 = _mm_rcp_ps(scale.m128);
			res.m128 = _mm_mul_ps(a.m128, scale.m128);

			return res;
		}

		inline float Sum(const float4 & a)
		{
			float4 temp1, temp2;
			
			temp1.m128 = _mm_shuffle_ps(a.m128, a.m128, _MM_SHUFFLE(3, 3, 1, 1));
			temp1.m128 = _mm_add_ps(a.m128, temp1.m128);

			temp2.m128 = _mm_shuffle_ps(temp1.m128, temp1.m128, _MM_SHUFFLE(3, 3, 3, 2));
			temp1.m128 = _mm_add_ps(temp2.m128, temp1.m128);

			return temp1.x;
		}

		inline float SquareLength(const float4 & a)
		{
			float4 temp = a * a;

			return Sum(temp);
		}

		inline float4 Normalise(const float4 & a)
		{
			float4 res, temp;

			res = a * a;
			
			temp.m128 = _mm_shuffle_ps(res.m128, res.m128, _MM_SHUFFLE(3, 3, 1, 1));
			res.m128 = _mm_add_ps(res.m128, temp.m128);

			temp.m128 = _mm_shuffle_ps(res.m128, res.m128, _MM_SHUFFLE(3, 3, 3, 2));
			res.m128 = _mm_add_ps(res.m128, temp.m128);

			temp.m128 = _mm_shuffle_ps(res.m128, res.m128, _MM_SHUFFLE(0, 0, 0, 0));
			temp.m128 = _mm_rsqrt_ps(temp.m128);
			res.m128 = _mm_mul_ps(a.m128, temp.m128);

			return res;
		}

		inline float4 Lerp(const float4 & a, const float4 & b, const float4 & percent)
		{
			float4 res;

			res = a + ((b - a) * percent);

			return res;
		}
	}
}

#endif /* O8_MATH_FLOAT4_HPP */

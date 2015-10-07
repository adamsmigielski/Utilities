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
 * @file Float12.hpp
 **/

#ifndef O8_MATH_FLOAT12_HPP
#define O8_MATH_FLOAT12_HPP

#include "FloatTypes.hpp"
#include "Float4.hpp"

namespace O8
{
	namespace Math
	{
		//Creation
		namespace Float12
		{
			static float12 one = {{1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f}};

			inline float12 Zero()
			{
				float12 res;

				res.x.m128 = _mm_setzero_ps();
				res.y.m128 = _mm_setzero_ps();
				res.z.m128 = _mm_setzero_ps();

				return res;
			}

			inline float12 & Zero(float12 & a)
			{
				a.x.m128 = _mm_setzero_ps();
				a.y.m128 = _mm_setzero_ps();
				a.z.m128 = _mm_setzero_ps();

				return a;
			}

			inline float12 One()
			{
				return one;
			}

			inline float12 Set(float val)
			{
				float12 res = one;

				res._11 = val;
				res._22 = val;
				res._33 = val;

				return res;
			}

			inline float12 Set(float x, float y, float z, float w)
			{
				float12 res = one;

				res._11 = x;
				res._22 = y;
				res._33 = z;

				return res;
			}

			inline float12 Set(float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34)
			{
				float12 res;

				res.x = Float4::Set(_11, _12, _13, _14);
				res.y = Float4::Set(_21, _22, _23, _24);
				res.z = Float4::Set(_31, _32, _33, _34);

				return res;
			}
		
			inline float12 Set(const float16 & a)
			{
				float12 res;

				res._1 = a._1;
				res._2 = a._2;
				res._3 = a._3;

				return res;
			}
		}

		inline float12 operator + (const float12 & a, const float12 & b)
		{
			float12 res;
			
			res.x.m128 = _mm_add_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_add_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_add_ps(a.z.m128, b.z.m128);

			return res;
		}
		
		inline float12 operator - (const float12 & a, const float12 & b)
		{
			float12 res;
			
			res.x.m128 = _mm_sub_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_sub_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_sub_ps(a.z.m128, b.z.m128);

			return res;
		}
		
		inline float12 operator * (const float12 & a, const float12 & b)
		{
			float12 res;
			
			res.x.m128 = _mm_mul_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_mul_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_mul_ps(a.z.m128, b.z.m128);

			return res;
		}
		
		inline float12 operator * (const float12 & a, const float b)
		{
			float12 res;
			float4 scale;

			scale.m128 = _mm_set1_ps(b);
			
			
			res.x.m128 = _mm_mul_ps(a.x.m128, scale.m128);
			res.y.m128 = _mm_mul_ps(a.y.m128, scale.m128);
			res.z.m128 = _mm_mul_ps(a.z.m128, scale.m128);

			return res;
		}
		
		inline float12 operator / (const float12 & a, const float12 & b)
		{
			float12 res;
			
			res.x.m128 = _mm_div_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_div_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_div_ps(a.z.m128, b.z.m128);
			
			return res;
		}
	}
}

#endif /* O8_MATH_FLOAT12_HPP */

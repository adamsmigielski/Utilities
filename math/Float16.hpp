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
 * @file Float16.hpp
 **/

#ifndef O8_MATH_FLOAT16_HPP
#define O8_MATH_FLOAT16_HPP

#include "FloatTypes.hpp"
#include "Float4.hpp"

namespace O8
{
	namespace Math
	{
		//Creation
		namespace Float16
		{
			static const float16 one = {{1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f}};

			inline float16 Zero()
			{
				float16 res;

				res.x.m128 = _mm_setzero_ps();
				res.y.m128 = _mm_setzero_ps();
				res.z.m128 = _mm_setzero_ps();
				res.w.m128 = _mm_setzero_ps();

				return res;
			}

			inline float16 One()
			{
				return one;
			}

			inline float16 Set(float val)
			{
				float16 res = one;

				res._11 = val;
				res._22 = val;
				res._33 = val;
				res._44 = val;

				return res;
			}

			inline float16 Set(float x, float y, float z, float w)
			{
				float16 res = one;

				res._11 = x;
				res._22 = y;
				res._33 = z;
				res._44 = w;

				return res;
			}

			inline float16 Set(float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44)
			{
				float16 res;

				res.x = Float4::Set(_11, _12, _13, _14);
				res.y = Float4::Set(_21, _22, _23, _24);
				res.z = Float4::Set(_31, _32, _33, _34);
				res.w = Float4::Set(_41, _42, _43, _44);

				return res;
			}


			static const float4 w = {{0.0f, 0.0f, 0.0f, 1.0f}};
			inline float16 Set(const float12 & a)
			{
				float16 res;

				res._1 = a._1;
				res._2 = a._2;
				res._3 = a._3;
				res._4 = w;

				return res;
			}

			inline float16 Transpose(const float16 & a)
			{
				float16 temp, res;

				temp.x.m128 = _mm_unpacklo_ps(a.x.m128, a.z.m128);
				temp.y.m128 = _mm_unpacklo_ps(a.y.m128, a.w.m128);
				temp.z.m128 = _mm_unpackhi_ps(a.x.m128, a.z.m128);
				temp.w.m128 = _mm_unpackhi_ps(a.y.m128, a.w.m128);

				res.x.m128 = _mm_unpacklo_ps(temp.x.m128, temp.y.m128);
				res.y.m128 = _mm_unpackhi_ps(temp.x.m128, temp.y.m128);
				res.z.m128 = _mm_unpacklo_ps(temp.z.m128, temp.w.m128);
				res.w.m128 = _mm_unpackhi_ps(temp.z.m128, temp.w.m128);

				return res;
			}
		}
		


		inline float16 operator + (const float16 & a, const float16 & b)
		{
			float16 res;
			
			res.x.m128 = _mm_add_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_add_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_add_ps(a.z.m128, b.z.m128);
			res.w.m128 = _mm_add_ps(a.w.m128, b.w.m128);

			return res;
		}
		
		inline float16 operator - (const float16 & a, const float16 & b)
		{
			float16 res;
			
			res.x.m128 = _mm_sub_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_sub_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_sub_ps(a.z.m128, b.z.m128);
			res.w.m128 = _mm_sub_ps(a.w.m128, b.w.m128);

			return res;
		}
		
		inline float16 operator * (const float16 & a, const float16 & b)
		{
			float16 res;
			
			res.x.m128 = _mm_mul_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_mul_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_mul_ps(a.z.m128, b.z.m128);
			res.w.m128 = _mm_mul_ps(a.w.m128, b.w.m128);

			return res;
		}
		
		inline float16 operator * (const float16 & a, const float b)
		{
			float16 res;
			float4 scale;

			scale.m128 = _mm_set1_ps(b);
			
			
			res.x.m128 = _mm_mul_ps(a.x.m128, scale.m128);
			res.y.m128 = _mm_mul_ps(a.y.m128, scale.m128);
			res.z.m128 = _mm_mul_ps(a.z.m128, scale.m128);
			res.w.m128 = _mm_mul_ps(a.w.m128, scale.m128);

			return res;
		}
		
		inline float16 operator / (const float16 & a, const float16 & b)
		{
			float16 res;
			
			res.x.m128 = _mm_div_ps(a.x.m128, b.x.m128);
			res.y.m128 = _mm_div_ps(a.y.m128, b.y.m128);
			res.z.m128 = _mm_div_ps(a.z.m128, b.z.m128);
			res.w.m128 = _mm_div_ps(a.w.m128, b.w.m128);
			
			return res;
		}
	}
}

#endif /* O8_MATH_FLOAT16_HPP */

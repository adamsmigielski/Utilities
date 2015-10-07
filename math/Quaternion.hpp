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
 * @file Quaternion.hpp
 **/

#ifndef UTILITIES_MATH_QUATERNION_HPP
#define UTILITIES_MATH_QUATERNION_HPP

#include "Float4.hpp"
#include <math.h>

namespace Math
{
	namespace Quaternion
	{
		inline float4 Make(float j, float x, float y, float z)
		{
			float4 res;

			float rad = j / 2.0f;
			float sinVal = sin(rad);

			res = Float4::Set(x, y, z, 0.0f);
				
			res = res * sinVal;
			res.w = cos(rad);

			res = Normalise(res);

			return res;
		}

		inline float4 Make(float j, const float4 & axis)
		{
			float4 res;

			float rad = j / 2.0f;
			float sinVal = sin(rad);
				
			res = axis * sinVal;
			res.w = cos(rad);

			res = Normalise(res);

			return res;
		}

		inline float4 MakeFromPoint(const float4 & point)
		{
			float4 res;

			res.m128 = point.m128;
			res.w = 0.0f;

			return res;
		}
			
		static const float4 i = {{1.0f, 0.0f, 0.0f, 0.0f}};
		static const float4 j = {{0.0f, 1.0f, 0.0f, 0.0f}};
		static const float4 k = {{0.0f, 0.0f, 1.0f, 0.0f}};
		static const float4 one = {{0.0f, 0.0f, 0.0f, 1.0f}};

		inline float4 I()
		{
			return i;
		}

		inline float4 J()
		{
			return j;
		}

		inline float4 K()
		{
			return k;
		}

		inline float4 One()
		{
			return one;
		}

		/*
		xx, yx, zx, jx
		xy, yy, zy, jy
		xz, yz, zz, jz
		xj, yj, zj, jj
		*/
		static const float4 multiplyHelperX = {{-1.0f, -1.0f, 1.0f, 1.0f}};
		static const float4 multiplyHelperY = {{1.0f, -1.0f, -1.0f, 1.0f}};
		static const float4 multiplyHelperZ = {{-1.0f, 1.0f, -1.0f, 1.0f}};

		inline float4 Multiply(const float4 & a, const float4 & b)
		{
			float4 res, temp1, temp2, temp3, temp4;
				
			temp1.m128 = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(0, 0, 0, 0));
			temp1.m128 = _mm_mul_ps(a.m128, temp1.m128);
			temp1.m128 = _mm_mul_ps(multiplyHelperX.m128, temp1.m128);
			temp1.m128 = _mm_shuffle_ps(temp1.m128, temp1.m128, _MM_SHUFFLE(0, 1, 2, 3));

			temp2.m128 = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(1, 1, 1, 1));
			temp2.m128 = _mm_mul_ps(a.m128, temp2.m128);
			temp2.m128 = _mm_mul_ps(multiplyHelperY.m128, temp2.m128);
			temp2.m128 = _mm_shuffle_ps(temp2.m128, temp2.m128, _MM_SHUFFLE(1, 0, 3, 2));
			res.m128 = _mm_add_ps(temp1.m128, temp2.m128);

			temp3.m128 = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(2, 2, 2, 2));
			temp3.m128 = _mm_mul_ps(a.m128, temp3.m128);
			temp3.m128 = _mm_mul_ps(multiplyHelperZ.m128, temp3.m128);
			temp3.m128 = _mm_shuffle_ps(temp3.m128, temp3.m128, _MM_SHUFFLE(2, 3, 0, 1));
			res.m128 = _mm_add_ps(res.m128, temp3.m128);

			temp4.m128 = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(3, 3, 3, 3));
			temp4.m128 = _mm_mul_ps(a.m128, temp4.m128);
			res.m128 = _mm_add_ps(res.m128, temp4.m128);				
								

			return res;
		}

		static const float4 inverseHelper = {{-1.0f, -1.0f, -1.0f, 1.0f}};

		inline float4 Inverse(const float4 & a)
		{
			float4 res;

			res.m128 = _mm_mul_ps(a.m128, inverseHelper.m128);

			return res;
		}

		inline float4 Rotate(const float4 & point, const float4 & quaternion)
		{
			float4 res, inv, temp;

			inv = Inverse(quaternion);
			temp = MakeFromPoint(point);
				
			res = Multiply(Multiply(quaternion, temp), inv);
				
			return res;
		}

		inline float4 NLerp(const float4 & a, const float4 & b, const float4 & percent)
		{
			float4 res, temp;

			temp = Lerp(a, b, percent);

			res = Normalise(temp);

			return res;
		}
	}
}

#endif /* UTILITIES_MATH_QUATERNION_HPP */

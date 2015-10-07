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
 * @file Matrix.hpp
 **/

#ifndef O8_MATH_MATRIX_HPP
#define O8_MATH_MATRIX_HPP

#include "FloatTypes.hpp"
#include "Float12.hpp"
#include "Float16.hpp"

namespace O8
{
	namespace Math
	{
		namespace Matrix
		{
			inline float16 Multiply(const float16 & a, const float16 & b)
			{
				float16 res, temp;

				temp.x.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, b.w.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.x.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);
			

				temp.x.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, b.w.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.y.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);
			

				temp.x.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, b.w.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.z.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);
			

				temp.x.m128 = _mm_shuffle_ps(a.w.m128, a.w.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.w.m128, a.w.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.w.m128, a.w.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.w.m128, a.w.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, b.w.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.w.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);

				return res;
			}

			inline float12 Multiply (const float12 & a, const float12 & b)
			{
				float16 temp;
				float12 res;
				float4 last = Float4::Set(0.0f, 0.0f, 0.0f, 1.0f);

				temp.x.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.x.m128, a.x.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, last.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.x.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);


				temp.x.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.y.m128, a.y.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, last.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.y.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);


				temp.x.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(0,0,0,0));
				temp.y.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(1,1,1,1));
				temp.z.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(2,2,2,2));
				temp.w.m128 = _mm_shuffle_ps(a.z.m128, a.z.m128, _MM_SHUFFLE(3,3,3,3));

				temp.x.m128 = _mm_mul_ps(temp.x.m128, b.x.m128);
				temp.y.m128 = _mm_mul_ps(temp.y.m128, b.y.m128);
				temp.z.m128 = _mm_mul_ps(temp.z.m128, b.z.m128);
				temp.w.m128 = _mm_mul_ps(temp.w.m128, last.m128);

				temp.x.m128 = _mm_add_ps(temp.x.m128, temp.y.m128);
				temp.z.m128 = _mm_add_ps(temp.z.m128, temp.w.m128);
				res.z.m128 = _mm_add_ps(temp.x.m128, temp.z.m128);

				return res;
			}

			inline float12 Multiply (const float16 & a, const float12 & b)
			{
				float12 temp = Float12::Set(a);

				return Multiply(temp, b);
			}

			inline float12 Multiply (const float12 & a, const float16 & b)
			{
				float12 temp = Float12::Set(b);

				return Multiply(a, temp);
			}

			inline float12 RotationFromQuaternion(const float4 & quat)
			{
				float12 matrix;

				const float ww = quat.w * quat.w;
				const float xx = quat.x * quat.x;
				const float yy = quat.y * quat.y;
				const float zz = quat.z * quat.z;

				const float xy2 = quat.x * quat.y * 2.0f;
				const float xz2 = quat.x * quat.z * 2.0f;
				const float xw2 = quat.x * quat.w * 2.0f;
				const float yz2 = quat.y * quat.z * 2.0f;
				const float yw2 = quat.y * quat.w * 2.0f;
				const float zw2 = quat.z * quat.w * 2.0f;

				//x
				matrix.xx = ww + xx - yy - zz;
				matrix.xy = xy2 + zw2;
				matrix.xz = xz2 - yw2;

				//y
				matrix.yx = xy2 - zw2;
				matrix.yy = ww - xx + yy - zz;
				matrix.yz = yz2 + xw2;

				//z
				matrix.zx = xz2 + yw2;
				matrix.zy = yz2 - xw2;
				matrix.zz = ww - xx - yy + zz;


				matrix.xw = 0.0f;
				matrix.yw = 0.0f;
				matrix.zw = 0.0f;

				return matrix;
			}

			inline float12 TranslationFromVector(const float4 & position)
			{
				float12 matrix;
				Float12::Zero(matrix);

				matrix.xw = position.x;
				matrix.yw = position.y;
				matrix.zw = position.z;

				return matrix;
			}

			inline float12 TransformationFromQuaternionAndVector(const float4 & quat,
					const float4 & position)
			{
				return RotationFromQuaternion(quat) + TranslationFromVector(position);
			}
		}
	}
}

#endif /* O8_MATH_MATRIX_HPP */

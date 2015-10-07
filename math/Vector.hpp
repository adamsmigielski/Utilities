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
 * @file Vector.hpp
 **/

#ifndef UTILITIES_MATH_VECTOR_HPP
#define UTILITIES_MATH_VECTOR_HPP

#include "Float4.hpp"

namespace Math
{
	namespace Vector
	{
		inline float Dot(const float4 & a, const float4 & b)
		{
			float4 res1, res2;

			res1.m128 = _mm_mul_ps(a.m128, b.m128);
			
			//res1 = x, y, z, w
			//res2 = z, w, #, #
			res2.m128 = _mm_shuffle_ps(res1.m128, b.m128, _MM_SHUFFLE(0, 0, 3, 2));

			//res1 = x + z, y + w, z + #, w + #
			//res2 = z, w, #, #
			res1.m128 = _mm_add_ps(res1.m128, res2.m128);

			//res1 = x + z, y + w, z + #, w + #
 			//res2 = y + w, #, #, #
			res2.m128 = _mm_shuffle_ps(res1.m128, b.m128, _MM_SHUFFLE(0, 0, 0, 1));

			//res1 = x + z + y + w, #, #, #
			res1.m128 = _mm_add_ps(res1.m128, res2.m128);

			return res1.x;
		}

		inline float4 Cross(const float4 & a, const float4 & b)
		{
				
			float4 temp1, temp2, res;
			
			temp1.m128 = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(3, 0, 2, 1)); //y, z, x
			temp2.m128 = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(3, 1, 0, 2)); //z, x, y
			temp1.m128 = _mm_mul_ps(a.m128, temp1.m128);
			temp2.m128 = _mm_mul_ps(b.m128, temp2.m128);
			

			temp1.m128 = _mm_shuffle_ps(temp1.m128, temp1.m128, _MM_SHUFFLE(3, 0, 2, 1)); //y, z, x
			temp2.m128 = _mm_shuffle_ps(temp2.m128, temp2.m128, _MM_SHUFFLE(3, 1, 0, 2)); //z, x, y



			res.m128 = _mm_sub_ps(temp1.m128, temp2.m128);

			//#, ay * bz

			return res;
		}
	}
}

#endif /* UTILITIES_MATH_VECTOR_HPP */

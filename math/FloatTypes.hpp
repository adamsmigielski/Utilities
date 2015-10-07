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
 * @file FloatTypes.hpp
 **/

#ifndef O8_MATH_FLOATTYPES_HPP
#define O8_MATH_FLOATTYPES_HPP

#include <O8\Common\Align.hpp>
#include <xmmintrin.h>

namespace O8
{
	namespace Math
	{
		union ALIGN16 float4
		{
			float f[4];

			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			struct
			{
				float _1;
				float _2;
				float _3;
				float _4;
			};

			__m128 m128;
		};


		union ALIGN16 float12
		{
			float f[12];

			struct
			{
				float xx;
				float xy;
				float xz;
				float xw;
				float yx;
				float yy;
				float yz;
				float yw;
				float zx;
				float zy;
				float zz;
				float zw;
			};

			struct
			{
				float _11;
				float _12;
				float _13;
				float _14;
				float _21;
				float _22;
				float _23;
				float _24;
				float _31;
				float _32;
				float _33;
				float _34;
			};

			struct
			{
				float4 x;
				float4 y;
				float4 z;
			};

			struct
			{
				float4 _1;
				float4 _2;
				float4 _3;
			};
		};


		union ALIGN16 float16
		{
			float f[16];

			struct
			{
				float xx;
				float xy;
				float xz;
				float xw;
				float yx;
				float yy;
				float yz;
				float yw;
				float zx;
				float zy;
				float zz;
				float zw;
				float wx;
				float wy;
				float wz;
				float ww;
			};

			struct
			{
				float _11;
				float _12;
				float _13;
				float _14;
				float _21;
				float _22;
				float _23;
				float _24;
				float _31;
				float _32;
				float _33;
				float _34;
				float _41;
				float _42;
				float _43;
				float _44;
			};

			struct
			{
				float4 x;
				float4 y;
				float4 z;
				float4 w;
			};

			struct
			{
				float4 _1;
				float4 _2;
				float4 _3;
				float4 _4;
			};
		};
	}
}

#endif /* O8_MATH_FLOATTYPES_HPP */

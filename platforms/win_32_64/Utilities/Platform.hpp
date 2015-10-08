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
* @file Platform.hpp
**/

#ifndef UTILITIES_PLATFORMS_WIN_32_64_PLATFORM_HPP
#define UTILITIES_PLATFORMS_WIN_32_64_PLATFORM_HPP

#include <stdint.h>

/* DLL */
#if O8_IS_MINGW

#define DLL_EXPORT __declspec(dllexport)

#define DLL_IMPORT __declspec(dllimport)

#else /* O8_IS_MINGW */

#define DLL_EXPORT __declspec(dllexport)

#define DLL_IMPORT __declspec(dllimport)

#endif /* O8_IS_MINGW */

/* Typedefs */
namespace Platform
{
    typedef int8_t       int8;
    typedef uint8_t      uint8;
    typedef int16_t      int16;
    typedef uint16_t     uint16;
    typedef int32_t      int32;
    typedef uint32_t     uint32;
    typedef unsigned int uint;
    typedef int64_t      int64;
    typedef uint64_t     uint64;
}

#endif /* UTILITIES_PLATFORMS_WIN_32_64_PLATFORM_HPP */

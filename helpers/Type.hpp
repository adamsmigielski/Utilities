/** License
*
* Copyright (c) 2015 Adam Śmigielski
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
* @author Adam Śmigielski
* @file Type.hpp
**/

#ifndef UTILITIES_TEMPLATES_TYPE_HPP
#define UTILITIES_TEMPLATES_TYPE_HPP

namespace Helpers
{
    template <typename T>
    class Difference_type
    {
    public:
        typedef T difference_type;
    };

    template <>
    class Difference_type < Platform::uint8 >
    {
    public:
        typedef Platform::int16 difference_type;
    };

    template <>
    class Difference_type < Platform::uint16 >
    {
    public:
        typedef Platform::int32 difference_type;
    };

    template <>
    class Difference_type < Platform::uint32 >
    {
    public:
        typedef Platform::int64 difference_type;
    };

    template <>
    class Difference_type < Platform::uint64 >
    {
    public:
        typedef Platform::int64 difference_type;
    };

    template <class T>
    class Type
    {
    public:
        typedef T value_type;
        typedef typename Difference_type<T>::difference_type difference_type;
        typedef value_type * pointer;
        typedef value_type & reference;
        typedef value_type && rvalue_reference;
        typedef const pointer const_pointer;
		typedef const reference & const_reference;
		typedef pointer & pointer_reference;
		typedef const_pointer & const_pointer_reference;
    };

    template <class T>
    class Type<T*>
    {
    public:
        typedef T value_type;
        typedef typename Difference_type<T>::difference_type difference_type;
        typedef value_type * pointer;
        typedef value_type & reference;
        typedef value_type && rvalue_reference;
        typedef const pointer const_pointer;
		typedef const reference & const_reference;
		typedef pointer & pointer_reference;
		typedef const_pointer & const_pointer_reference;
    };

    template <class T>
    class Type<T&>
    {
    public:
        typedef T value_type;
        typedef typename Difference_type<T>::difference_type difference_type;
        typedef value_type * pointer;
        typedef value_type & reference;
        typedef value_type && rvalue_reference;
        typedef const pointer const_pointer;
		typedef const reference & const_reference;
		typedef pointer & pointer_reference;
		typedef const_pointer & const_pointer_reference;
    };
}

#endif UTILITIES_TEMPLATES_TYPE_HPP
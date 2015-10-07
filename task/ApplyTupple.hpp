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
* @file ApplyTupple.hpp
**/

#ifndef O8_TEMPLATES_APPLYTUPPLE_HPP
#define O8_TEMPLATES_APPLYTUPPLE_HPP

#include <tuple>

namespace O8
{
	namespace Templates
	{
		template<size_t N>
		class ApplyTupleToFunction
		{
		public:
			template<typename ...ArgF, typename ...ArgT, typename ...Args>
			static void Call(void (*f)(ArgF...),
			    std::tuple<ArgT...> & tuple,
			    Args ... args)
			{
				ApplyTupleToFunction<N - 1>::Call(f, tuple,
				    std::get < N - 1 > (tuple), args...);
			}
		};

		template<>
		class ApplyTupleToFunction<0>
		{
		public:
			template<typename ...ArgF, typename ...ArgT, typename ...Args>
			static void Call(void (*f)(ArgF...),
			    std::tuple<ArgT...> & tuple,
			    Args ... args)
			{
				f(args...);
			}
		};

		template<size_t N>
		class ApplyTupleToMethod
		{
		public:
			template<typename Class, typename ...ArgF, typename ...ArgT,
			    typename ...Args>
			static void Call(Class * object,
			    void (Class::*f)(ArgF...),
			    std::tuple<ArgT...> & tuple,
			    Args ... args)
			{
				ApplyTupleToMethod<N - 1>::Call(object, f, tuple,
				    std::get < N - 1 > (tuple), args...);
			}
		};

		template<>
		class ApplyTupleToMethod<0>
		{
		public:
			template<typename Class, typename ...ArgF, typename ...ArgT,
			    typename ...Args>
			static void Call(Class * object,
			    void (Class::*f)(ArgF...),
			    std::tuple<ArgT...> & tuple,
			    Args ... args)
			{
				(object->*f)(args...);
			}
		};

	}
}

#endif /* O8_TEMPLATES_APPLYTUPPLE_HPP */

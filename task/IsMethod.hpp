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
* @file IsMethod.hpp
**/

#ifndef UTILITIES_TASK_ISMETHOD_HPP
#define UTILITIES_TASK_ISMETHOD_HPP

namespace Task
{
	template<typename O, typename F, typename ...TT>
	class IsMethod
	{
	private:
		typedef char positive_type;
		typedef long negative_type;

		template<typename C, typename M, typename ...Args>
		static positive_type f(void (C::*method)(Args...));

		template<typename R, typename S, typename ...Args>
		static negative_type f(...);

	public:
		enum
		{
			result = (
				sizeof(f<O, F, TT...>((F) 0)) == sizeof(positive_type) ? 1 :
				                                                            0)
		};
	};
}

#endif /* UTILITIES_TASK_ISMETHOD_HPP */

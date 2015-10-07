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
* @file Task.hpp
**/

#ifndef TASK_HPP_
#define TASK_HPP_

#include "ApplyTupple.hpp"
#include "IsMethod.hpp"

namespace O8
{
	namespace Templates
	{

		template<typename T>
		class Dereference
		{
		public:
			typedef T Type;
		};

		template<typename T>
		class Dereference<T*>
		{
		public:
			typedef T Type;
		};

//Implementation
		template<size_t, typename ...TT>
		class Task;

		template<typename Function, typename ...TT>
		class Task<0, Function, TT...>
		{
		public:
			Task(void (*function)(TT...), TT ... args)
				: m_function(function),
				  m_data(args...)
			{
			}

			void Run()
			{
				ApplyTupleToFunction<sizeof...(TT)>::Call(m_function, m_data);
			}

		private:
			void (*m_function)(TT...);
			std::tuple<TT...> m_data;
		};

		template<typename Object, typename F, typename ...TT>
		class Task<1, Object, F, TT...>
		{
		public:
			Task(Object * object, void (Object::*method)(TT...), TT ... args)
				: m_object(object),
				  m_method(method),
				  m_data(args...)
			{
			}

			void Run()
			{
				ApplyTupleToMethod<sizeof...(TT)>::Call(m_object, m_method, m_data);
			}

		private:
			Object * m_object;
			void (Object::*m_method)(TT...);
			std::tuple<TT...> m_data;
		};

		template<typename F, typename ...TT>
		Task<IsMethod<F, TT...>::result, F, TT...> * CreateTask(F * f,
		    TT ... args)
		{
			Task<IsMethod<F, TT...>::result, F, TT...> * t = new Task<
			    IsMethod<F, TT...>::result, F, TT...>(f, args...);

			return t;
		}

		template<typename FP, typename ...TT>
		class TaskFactory
		{
		private:
			typedef typename Dereference<FP>::Type F;

			enum
			{
				is_method = IsMethod<F, TT...>::result
			};

		public:
			typedef Task<is_method, F, TT...> Type;

			static Type * Create(F * f, TT ... args)
			{
				Type * t = new Type(f, args...);

				return t;
			}
		};

	}
}

#endif /* TASK_HPP_ */

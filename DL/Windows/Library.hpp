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
 * @file Library.hpp
 **/


#ifndef UTILITIES_DL_WINDOWS_LIBRARY_HPP
#define UTILITIES_DL_WINDOWS_LIBRARY_HPP

#include <Utilities\DL\Library.hpp>  /* Library */
#include <Windows.h>                 /* HMODULE */

namespace Dynamic_library
{
	namespace Windows
	{
		class Library : public ::Dynamic_library::Library
		{
		public:
			Library();
			virtual ~Library();

			void Init(HMODULE module, const char * name);
			virtual Platform::proc_t GetFunctionAddress(const char * function_name);


		private:
			HMODULE m_module;
			std::string m_name;
		};
	}
}

#endif /* UTILITIES_DL_WINDOWS_LIBRARY_HPP */

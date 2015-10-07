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
* @file PointerContainer.hpp
**/

#ifndef UTILITIES_CONTAINERS_POINTER_CONTAINER_HPP
#define UTILITIES_CONTAINERS_POINTER_CONTAINER_HPP

#include <algorithm>

#include <Utilities\helpers\Type.hpp>

namespace Containers
{
    namespace PointerContainer
    {
        template <class T>
        class Operations
        {
        public:
			typedef Helpers::Type<T> type;

            static void Delete_entry(typename type::pointer_reference entry);

            
        };
        
        template <class C>
        void Remove_all(C & container);



        template <class T>
        void Operations<T>::Delete_entry(typename Operations<T>::pointer_reference entry)
        {
            pointer ptr = entry;

            entry = nullptr;


            if (nullptr != ptr)
            {
                delete ptr;
            }
        }

        template <class C>
        void Remove_all(C & container)
        {
            std::for_each(container.begin(), container.end(), Operations<typename Helpers::Type<typename C::value_type>::value_type>::Delete_entry);
        }
    }
}

#endif /* UTILITIES_CONTAINERS_POINTER_CONTAINER_HPP */


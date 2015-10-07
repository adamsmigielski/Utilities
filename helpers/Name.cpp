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
* @file Name.cpp
**/

#include "PCH.hpp"

#include "Name.hpp"
#include "Hash_string.hpp"

namespace O8
{
    namespace Utility
    {
        Name::Name()
            : m_hash(0)
        {
            /* Nothing to be done here */
        }

        Name::~Name()
        {
            m_hash = 0;
        }

        void Name::Clear()
        {
            m_name.clear();
            m_hash = 0;
        }

        void Name::operator()(const std::string & name)
        {
            if (true == name.empty())
            {
                Clear();
            }
            else
            {
                m_name = name;
                m_hash = Hash_string(m_name);
            }
        }

        const std::string & Name::operator()(void) const
        {
            return m_name;
        }

        size_t Name::Hash() const
        {
            return m_hash;
        }
    }
}

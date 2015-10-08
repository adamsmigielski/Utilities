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
* @file Name.hpp
**/

#ifndef UTILITIES_HELPERS_NAME_HPP
#define UTILITIES_HELPERS_NAME_HPP

namespace Helpers
{
    class Name
    {
    public:
        Name();
        ~Name();

        void Clear();

        void operator()(const std::string & name);
        const std::string & operator() (void) const;
        size_t Hash() const;

    private:
        std::string m_name;
        size_t m_hash;
    };

    template <typename T>
    class Name_predicate
    {
    public:
        Name_predicate(const std::string & name)
            : m_hash(Hash_string(name))
        {
            /* Nothing to be done here */
        }

        Name_predicate(size_t hash)
            : m_hash(hash)
        {
            /* Nothing to be done here */
        }

        bool operator()(const T& t) const
        {
            return (t.m_Name.Hash() == m_hash);
        }

    private:
        size_t m_hash;
    };

    template <typename T>
    class Name_ascend_predicate
    {
    public:
        int operator()(const T& left, const T& right) const
        {
            return left.m_Name().compare(right.m_Name());
        }
    };
}

#endif UTILITIES_HELPERS_NAME_HPP

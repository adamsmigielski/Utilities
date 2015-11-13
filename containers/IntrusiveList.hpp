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
* @file IntrusiveList.hpp
**/

#ifndef UTILITIES_CONTAINERS_INTRUSIVE_LIST_HPP
#define UTILITIES_CONTAINERS_INTRUSIVE_LIST_HPP

namespace Containers
{
    namespace IntrusiveList
    {
        template <typename T>
        class List;

        template <typename T>
        class Node
        {
        public:
            using List = Containers::IntrusiveList::List< T >;

            friend class List;

        public:
            Node();
            virtual ~Node();

            T * Get();
            const T * Get() const;

            T * Next();
            const T * Next() const;
            T * Previous();
            const T * Previous() const;

            List * Parent();
            const List * Parent() const;

        private:
            T * m_next;
            T * m_prev;

            List * m_parent;
        };

        template <typename T>
        class List
        {
        public:
            List();
            List(List && list);
            virtual ~List();

            void Attach(T * node);
#if 0 /* TBD */
            void Attach_after(Node<T> * node, Node<T> * after);
            void Attach_before(Node<T> * node, Node<T> * before);
#endif
            void Clear();
            void Detach(T * node);

            void Exchange(T * left, T * right);

            T * First();
            const T * First() const;
            T * Last();
            const T * Last() const;

            template <typename P>
            T * Search(const P & p);

            template <typename P, typename V>
            T * Search(const P & p, const V & v);

            template <typename P>
            const T * Search(const P & p) const;

            template <typename P, typename V>
            const T * Search(const P & p, const V & v) const;

            template <typename P>
            void Sort(const P & p);

            Platform::uint32 Size() const;

        private:
            T * m_first;
            T * m_last;
        };

        template <typename T>
        Node<T>::Node()
            : m_next(nullptr)
            , m_prev(nullptr)
            , m_parent(nullptr)
        {

        }

        template <typename T>
        Node<T>::~Node()
        {
            if (nullptr != m_parent)
            {
                m_parent->Detach(Get());
            }
        }

        template <typename T>
        T * Node<T>::Get()
        {
            return (T*) this;
        }

        template <typename T>
        const T * Node<T>::Get() const
        {
            return (T*) this;
        }

        template <typename T>
        T * Node<T>::Next()
        {
            return m_next;
        }

        template <typename T>
        const T * Node<T>::Next() const
        {
            return m_next;
        }

        template <typename T>
        T * Node<T>::Previous()
        {
            return m_prev;
        }

        template <typename T>
        const T * Node<T>::Previous() const
        {
            return m_prev;
        }

        template <typename T>
        List<T> * Node<T>::Parent()
        {
            return m_parent;
        }

        template <typename T>
        const List<T> * Node<T>::Parent() const
        {
            return m_parent;
        }

        template <typename T>
        List<T>::List()
            : m_first(nullptr)
            , m_last(nullptr)
        {
            /* Nothing to be done */
        }

        template <typename T>
        List<T>::List(List && list)
            : m_first(std::move(list.m_first))
            , m_last(std::move(list.m_last))
        {
            /* Nothing to be done */
        }

        template <typename T>
        List<T>::~List()
        {
            Clear();
        }

        template <typename T>
        void List<T>::Attach(T * node)
        {
            if (nullptr == node)
            {
                ASSERT(0);
                return;
            }

            /* Detach from previous list */
            if (nullptr != node->m_parent)
            {
                node->m_parent->Detach(node);
            }

            if (nullptr == m_last) /* List is empty */
            {
                m_first = m_last = node;
            }
            else
            {
                /* Make connection */
                m_last->m_next = node;
                node->m_prev = m_last;

                /* Set last pointer */
                m_last = node;
            }

            /* Set parent */
            node->m_parent = this;
        }

        template <typename T>
        void List<T>::Clear()
        {
            T * next = nullptr;

            for (T * current = m_first;
                current != nullptr;
                current = next)
            {
                next = current->Next();

                delete current;
            }
        }

        template <typename T>
        void List<T>::Detach(T * node)
        {
            if (nullptr == node)
            {
                ASSERT(0);
                return;
            }

            if (nullptr == m_last) /* List is empty - something is not right */
            {
                ASSERT(0);
            }
            else if (m_last == node) /* End of the list */
            {
                if (m_first == m_last) /* Last element left */
                {
                    m_first = nullptr;
                    m_last = nullptr;
                }
                else
                {
                    auto prev = node->m_prev;

                    /* Break connection prev >> node */
                    prev->m_next = nullptr;

                    /* Store last pointer */
                    m_last = prev;
                }
            }
            else if (m_first == node) /* Begining of the list */
            {
                auto next = node->m_next;

                /* Break connection next >> node */
                next->m_prev = nullptr;

                /* Store first pointer */
                m_first = next;
            }
            else /* In the middle */
            {
                auto next = node->m_next;
                auto prev = node->m_prev;

                /* Make connection prev <> next */
                next->m_prev = prev;
                prev->m_next = next;
            }

            /* Null all pointers in node */
            node->m_next = nullptr;
            node->m_prev = nullptr;
            node->m_parent = nullptr;
        }

        template <typename T>
        void List<T>::Exchange(T * left, T * right)
        {
            T * const left_next = left->m_next;
            T * const left_prev = left->m_prev;
            T * const right_next = right->m_next;
            T * const right_prev = right->m_prev;

            /* Are they different */
            if (left == right)
            {
                return;
            }
            /* Are they neighbours */
            else if (left_next == right)
            {
                left->m_next = right_next;
                left->m_prev = right;
                right->m_next = left;
                right->m_prev = left_prev;

                /* Is left first? */
                if (nullptr != left_prev)
                {
                    left_prev->m_next = right;
                }
                else
                {
                    m_first = right;
                }

                /* Is right last? */
                if (nullptr != right_next)
                {
                    right_next->m_prev = left;
                }
                else
                {
                    m_last = left;
                }
            }
            /* Are they neighbours */
            else if (left_prev == right)
            {
                right->m_next = left_next;
                right->m_prev = left;
                left->m_next = right;
                left->m_prev = right_prev;

                /* Is right first? */
                if (nullptr != right_prev)
                {
                    right_prev->m_next = left;
                }
                else
                {
                    m_first = left;
                }

                /* Is left last? */
                if (nullptr != left_next)
                {
                    left_next->m_prev = right;
                }
                else
                {
                    m_last = right;
                }
            }
            /* They are away */
            else
            {
                left->m_next = right_next;
                left->m_prev = right_prev;
                right->m_next = left_next;
                right->m_prev = left_prev;

                /* Is left first? */
                if (nullptr != left_prev)
                {
                    left_prev->m_next = right;
                }
                else
                {
                    m_first = right;
                }

                /* Is left last? */
                if (nullptr != left_next)
                {
                    left_next->m_prev = right;
                }
                else
                {
                    m_last = right;
                }

                /* Is right first? */
                if (nullptr != right_prev)
                {
                    right_prev->m_next = left;
                }
                else
                {
                    m_first = left;
                }

                /* Is right last? */
                if (nullptr != right_next)
                {
                    right_next->m_prev = left;
                }
                else
                {
                    m_last = left;
                }
            }
        }

        template <typename T>
        T * List<T>::First()
        {
            return m_first;
        }

        template <typename T>
        const T * List<T>::First() const
        {
            return m_first;
        }

        template <typename T>
        T * List<T>::Last()
        {
            return m_last;
        }

        template <typename T>
        const T * List<T>::Last() const
        {
            return m_last;
        }

        template <typename T>
        template <typename P>
        T * List<T>::Search(const P & p)
        {
            T * result = nullptr;

            for (T * t = First(); nullptr != t; t = t->Next())
            {
                if (true == p(*t))
                {
                    result = t;
                    break;
                }
            }

            return result;
        }

        template <typename T>
        template <typename P, typename V>
        T * List<T>::Search(const P & p, const V & v)
        {
            T * result = nullptr;

            for (T * t = First(); nullptr != t; t = t->Next())
            {
                if (v == p(*t))
                {
                    result = t;
                    break;
                }
            }

            return result;
        }

        template <typename T>
        template <typename P>
        const T * List<T>::Search(const P & p) const
        {
            const T * result = nullptr;

            for (const T * t = First(); nullptr != t; t = t->Next())
            {
                if (true == p(*t))
                {
                    result = t;
                    break;
                }
            }

            return result;
        }

        template <typename T>
        template <typename P, typename V>
        const T * List<T>::Search(const P & p, const V & v) const
        {
            const T * result = nullptr;

            for (const T * t = First(); nullptr != t; t = t->Next())
            {
                if (v == p(*t))
                {
                    result = t;
                    break;
                }
            }

            return result;
        }

        template <typename T>
        template <typename P>
        void List<T>::Sort(const P & p)
        {
            T * last = Last();

            while (nullptr != last)
            {
                T * it = First();
                
                while (last != it)
                {
                    T * left = it;
                    T * right = it->Next();

                    if (0 < p(*left, *right))
                    {
                        Exchange(left, right);

                        /* Last was just moved before it, end inner loop */
                        if (right == last)
                        {
                            break;
                        }
                    }
                    else
                    {
                        it = it->Next();
                    }
                }

                last = it->Previous();
            }
        }

        template <typename T>
        Platform::uint32 List<T>::Size() const
        {
            GLuint size = 0;

            for (T * it = First();
                nullptr != it;
                it = it->Next())
            {
                size += 1;
            }

            return size;
        }
    }
}

#endif /* UTILITIES_CONTAINERS_INTRUSIVE_LIST_HPP */

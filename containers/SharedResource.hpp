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
* @file SharedResource.hpp
**/

#ifndef UTILITIES_CONTAINERS_SHAREDRESOURCE_HPP
#define UTILITIES_CONTAINERS_SHAREDRESOURCE_HPP

namespace Containers
{
    namespace SharedResource
    {
        template <typename T>
        class Reference<T>;

        template <typename T>
        class Event_handler
        {
        public:
            virtual ~Event_handler() {}

            virtual void On_owner_destruction(T * resource);
            virtual void On_last_reference_gone(
                T * resource,
                bool & should_owner_be_destoyed);
        };

        template <typename T>
        class Owner
        {
            friend class Reference<T>
        public:
            static Owner * Create();
            static Owner * Create(T * resource);
            ~Owner();

            void Set_event_handler(Event_handler * handler);

        private:
            Owner();
            Owner(T * resource);

            void increase_reference_count();
            void decrease_reference_count();

            Event_handler * m_event_handler;
            T * m_resource;
            uint32 m_reference_counter;
        };

        template <typename T>
        class Reference
        {
        public:
            Reference();
            Reference(T * resource);
            Reference(const Reference & reference);
            Reference(Reference && reference);
            Reference & operator = (const Reference & reference);
            Reference & operator = (Reference && reference);
            ~Reference();

            void Reset(T * resource);
            void Release();

            Owner * Get_owner();

            T * operator * ();
            const T * operator * () const;

            T * operator -> ();
            const T * operator -> () const;

            T * Get();
            const T * Get() const;

        private:
            void set(T * resource);

            Owner * m_owner;
            T * m_resource;
        };

        template <typename T>
        void Event_handler<T>::On_owner_destruction(T * resource)
        {
            if (nullptr != resource)
            {
                delete resource;
            }
        }

        template <typename T>
        void Event_handler<T>::On_last_reference_gone(
            T * resource,
            bool & should_owner_be_destoyed)
        {
            should_owner_be_destoyed = true;
        }

        template <typename T>
        Owner<T> * Owner<T>::Create()
        {
            return new Owner;
        }

        template <typename T>
        Owner<T> * Owner<T>::Create(T * resource)
        {
            return new Owner(resource);
        }

        template <typename T>
        Owner<T>::Owner()
            : m_event_handler(nullptr)
            , m_resource(nullptr)
            , m_reference_counter(0)
        {
            /* Nothing to be done here */
        }

        template <typename T>
        Owner<T>::Owner(T * resource)
            : m_event_handler(nullptr)
            , m_resource(resource)
            , m_reference_counter(0)
        {
            /* Nothing to be done here */
        }

        template <typename T>
        Owner<T>::~Owner()
        {
            if (nullptr != m_resource)
            {
                if (nullptr == m_handler)
                {
                    delete m_resource;
                }
                else
                {
                    m_handler->On_owner_destruction(m_resource);
                }

                m_resource = nullptr;
            }
        }

        template <typename T>
        void Owner<T>::Set_event_handler(Event_handler * handler)
        {
            m_handler = handler;
        }

        template <typename T>
        void Owner<T>::increase_reference_count()
        {
            m_reference_counter += 1;
        }

        template <typename T>
        void Owner<T>::decrease_reference_count()
        {
            if (1 == m_reference_counter)
            {
                m_reference_counter = 0;

                if (nullptr == m_handler)
                {
                    delete this;
                }
                else
                {
                    bool should_owner_be_destroyed = true;

                    m_handler->On_last_reference_gone(
                        m_resource,
                        should_owner_be_destroyed);

                    if (true == should_owner_be_destroyed)
                    {
                        delete this;
                    }
                }
            }
            else if (0 == m_reference_counter)
            {
                DEBUGLOG("Something is seriously wrong with shared resource management");
                ASSERT(0);
            }
            else
            {
                m_reference_counter -= 1;
            }
        }

        template <typename T>
        Reference<T>::Reference()
            : m_owner(nullptr)
            , m_resource(nullptr)
        {
            /* Nothing to be done here */
        }

        template <typename T>
        Reference<T>::Reference(T * resource)
            : m_owner(nullptr)
            , m_resource(nullptr)
        {
            set(resource);
        }

        template <typename T>
        Reference<T>::Reference(const Reference & reference)
            : m_owner(reference.m_owner)
            , m_resource(reference.m_resource)
        {
            if (nullptr != m_owner)
            {
                m_owner->increase_reference_count();
            }
        }

        template <typename T>
        Reference<T>::Reference(Reference && reference)
            : m_owner(std::move(reference.m_owner))
            , m_resource(std::move(reference.m_resource))
        {
        }

        template <typename T>
        Reference<T> & Reference<T>::operator = (const Reference & reference)
        {
            Release();

            m_owner = reference.m_owner;
            m_resource = reference.m_resource;

            if (nullptr != m_owner)
            {
                m_owner->increase_reference_count();
            }
        }

        template <typename T>
        Reference<T> & Reference<T>::operator = (Reference && reference)
        {
            Release();

            m_owner = std::move(reference.m_owner);
            m_resource = std::move(reference.m_resource);
        }

        template <typename T>
        Reference<T>::~Reference()
        {
            Release();
        }

        template <typename T>
        void Reference<T>::Reset(T * resource)
        {
            Release();

            set(resource);
        }

        template <typename T>
        void Reference<T>::Release()
        {
            if (nullptr != m_owner)
            {
                m_owner->decrease_reference_count();
            }

            m_owner = nullptr;
            m_resource = nullptr;
        }

        template <typename T>
        void Reference<T>::set(T * resource)
        {
            /* Do not create infrastructure for keeping nullptr */
            if (nullptr == resource)
            {
                return;
            }

            auto owner = Owner::Create(resource);

            if (nullptr != owner)
            {
                m_owner = owner;
                m_owner->increase_reference_count();
                m_resource = resource;
            }
        }

        template <typename T>
        Owner * Reference<T>::Get_owner()
        {
            return m_owner;
        }

        template <typename T>
        T * Reference<T>::operator * ()
        {
            ASSERT(nullptr != m_resource);

            return m_resource;
        }

        template <typename T>
        const T * Reference<T>::operator * () const
        {
            ASSERT(nullptr != m_resource);

            return m_resource;
        }

        template <typename T>
        T * Reference<T>::operator -> ()
        {
            ASSERT(nullptr != m_resource);

            return m_resource;
        }

        template <typename T>
        const T * Reference<T>::operator -> () const
        {
            ASSERT(nullptr != m_resource);

            return m_resource;
        }

        template <typename T>
        T * Reference<T>::Get()
        {
            ASSERT(nullptr != m_resource);

            return m_resource;
        }

        template <typename T>
        const T * Reference<T>::Get() const
        {
            ASSERT(nullptr != m_resource);

            return m_resource;
        }
    }
}

#endif /* UTILITIES_CONTAINERS_SHAREDRESOURCE_HPP */

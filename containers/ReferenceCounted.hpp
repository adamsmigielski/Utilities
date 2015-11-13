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
* @file ReferenceCounted.hpp
**/

#ifndef UTILITIES_CONTAINERS_REFERENCECOUNTED_HPP
#define UTILITIES_CONTAINERS_REFERENCECOUNTED_HPP

namespace Containers
{
	namespace ReferenceCounted
	{
		class Base_resource;

        template <typename T>
		class Event_handler
		{
		public:
			virtual ~Event_handler()
			{
			}

			virtual void On_resource_destruction(T * resource);
			virtual void On_last_reference_gone(
				T * resource,
				bool & should_resource_be_destoyed);
		};

        template <typename T>
        class Resource
        {
            template <typename T>
            friend class Reference;

        public:
            using Event_handler = Event_handler < T >;
            using Reference = Reference < T >;
            using ref_count_t = Platform::uint32;

            Event_handler * Get_event_handler() const;
            void Set_event_handler(Event_handler * handler);

            ref_count_t Get_references_number() const;

        protected:
            Resource();
            virtual ~Resource();

        private:
			void increase_reference_count();
			void decrease_reference_count();

            Event_handler * m_event_handler = nullptr;
            ref_count_t m_reference_counter = ref_count_t(0);
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

			T & operator * ();
			const T & operator * () const;

			T * operator -> ();
			const T * operator -> () const;

			T * Get();
			const T * Get() const;

		private:
			void set(T * resource);

			T * m_resource;
		};

        /* *** Even_handler *** */
        template <typename T>
        void Event_handler<T>::On_resource_destruction(T * resource)
        {
            /* Nothing to be done here */
        }

        template <typename T>
        void Event_handler<T>::On_last_reference_gone(
            T * resource,
            bool & should_resource_be_destoyed)
        {
            should_resource_be_destoyed = true;
        }

        /* *** Resource *** */


        template <typename T>
        Resource<T>::Resource()
        {
            /* Nothing to be done here */
        }

        template <typename T>
        Resource<T>::~Resource()
        {
            if (nullptr != m_event_handler)
            {
                m_event_handler->On_resource_destruction((T *) this);
                m_event_handler = nullptr;
            }
        }

        template <typename T>
        auto Resource<T>::Get_event_handler() const -> Event_handler *
        {
            return m_event_handler;
        }

        template <typename T>
        void Resource<T>::Set_event_handler(Event_handler * handler)
        {
            m_event_handler = handler;
        }

        template <typename T>
        auto Resource<T>::Get_references_number() const -> ref_count_t
        {
            return m_reference_counter;
        }

        template <typename T>
        void Resource<T>::increase_reference_count()
        {
            m_reference_counter += ref_count_t(1);
        }

        template <typename T>
        void Resource<T>::decrease_reference_count()
        {
            if (1 == m_reference_counter)
            {
                m_reference_counter = ref_count_t(0);

                if (nullptr == m_event_handler)
                {
                    delete this;
                }
                else
                {
                    bool should_resource_be_destroyed = true;

                    m_event_handler->On_last_reference_gone(
                        (T *) this,
                        should_resource_be_destroyed);

                    if (true == should_resource_be_destroyed)
                    {
                        delete this;
                    }
                }
            }
            else if (0 == m_reference_counter)
            {
                DEBUGLOG("Something is seriously wrong with reference counted resources");
                ASSERT(0);
            }
            else
            {
                m_reference_counter -= ref_count_t(1);
            }
        }

        /* *** Reference *** */
		template <typename T>
		Reference<T>::Reference()
			: m_resource(nullptr)
		{
			/* Nothing to be done here */
		}

		template <typename T>
		Reference<T>::Reference(T * resource)
			: m_resource(nullptr)
		{
			set(resource);
		}

		template <typename T>
		Reference<T>::Reference(const Reference & reference)
            : Reference(reference.m_resource)
		{
            /* Nothing to be done here */
        }

		template <typename T>
		Reference<T>::Reference(Reference && reference)
            : Reference(reference.m_resource)
		{
            reference.Release();
		}

		template <typename T>
		Reference<T> & Reference<T>::operator = (const Reference & reference)
		{
            Reset(reference.m_resource);

			return *this;
		}

		template <typename T>
		Reference<T> & Reference<T>::operator = (Reference && reference)
		{
            Reset(reference.m_resource);
            reference.Release();

            return *this;
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
			if (nullptr != m_resource)
			{
				m_resource->decrease_reference_count();
				m_resource = nullptr;
			}
		}

		template <typename T>
		void Reference<T>::set(T * resource)
		{
			if (nullptr == resource)
			{
				return;
			}

            m_resource = resource;
            m_resource->increase_reference_count();
		}

		template <typename T>
		T & Reference<T>::operator * ()
		{
			ASSERT(nullptr != m_resource);

			return m_resource;
		}

		template <typename T>
		const T & Reference<T>::operator * () const
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
			return m_resource;
		}

		template <typename T>
		const T * Reference<T>::Get() const
		{
			return m_resource;
		}
	}
}

#endif /* UTILITIES_CONTAINERS_REFERENCECOUNTED_HPP */
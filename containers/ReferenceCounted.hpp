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
		class Resource;

		class Event_handler
		{
		public:
			virtual ~Event_handler()
			{
			}

			virtual void On_resource_destruction(Resource * resource);
			virtual void On_last_reference_gone(
				Resource * resource,
				bool & should_resource_be_destoyed);
		};

		class Resource
		{
			template <typename T>
			friend class Reference;

		public:
			virtual ~Resource();

			void Set_event_handler(Event_handler * handler);

		protected:
			Resource();

		private:
			void increase_reference_count();
			void decrease_reference_count();

			Event_handler * m_event_handler;
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

			T * operator * ();
			const T * operator * () const;

			T * operator -> ();
			const T * operator -> () const;

			T * Get();
			const T * Get() const;

		private:
			void set(T * resource);

			T * m_resource;
		};



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
			: m_resource(reference.m_resource)
		{
			if (nullptr != m_resource)
			{
				m_resource->increase_reference_count();
			}
		}

		template <typename T>
		Reference<T>::Reference(Reference && reference)
			: m_resource(std::move(reference.m_resource))
		{
		}

		template <typename T>
		Reference<T> & Reference<T>::operator = (const Reference & reference)
		{
			Release();

			m_resource = reference.m_resource;

			if (nullptr != m_resource)
			{
				m_resource->increase_reference_count();
			}

			return *this;
		}

		template <typename T>
		Reference<T> & Reference<T>::operator = (Reference && reference)
		{
			Release();

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

			m_resource->increase_reference_count();
			m_resource = resource;
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

#endif /* UTILITIES_CONTAINERS_REFERENCECOUNTED_HPP */
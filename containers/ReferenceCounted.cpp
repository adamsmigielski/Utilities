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
* @file ReferenceCounted.cpp
**/

#include "PCH.hpp"

#include "ReferenceCounted.hpp"

namespace Containers
{
    namespace ReferenceCounted
    {
        void Event_handler::On_resource_destruction(Resource * resource)
        {
            /* Nothing to be done here */
        }

        void Event_handler::On_last_reference_gone(
            Resource * resource,
            bool & should_resource_be_destoyed)
        {
            should_resource_be_destoyed = true;
        }

        Resource::Resource()
            : m_event_handler(nullptr)
            , m_reference_counter(0)
        {
            /* Nothing to be done here */
        }

        Resource::~Resource()
        {
            if (nullptr != m_event_handler)
            {
                m_event_handler->On_resource_destruction(this);
                m_event_handler = nullptr;
            }
        }

        void Resource::Set_event_handler(Event_handler * handler)
        {
            m_event_handler = handler;
        }

        void Resource::increase_reference_count()
        {
            m_reference_counter += 1;
        }

        void Resource::decrease_reference_count()
        {
            if (1 == m_reference_counter)
            {
                m_reference_counter = 0;

                if (nullptr == m_event_handler)
                {
                    delete this;
                }
                else
                {
                    bool should_resource_be_destroyed = true;

                    m_event_handler->On_last_reference_gone(
                        this,
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
                m_reference_counter -= 1;
            }
        }
    }
}
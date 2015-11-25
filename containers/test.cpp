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
* @file test.cpp
**/

#include "PCH.hpp"

#include <Unit_Tests\UnitTests.hpp>

#include "IntrusiveList.hpp"
#include "ReferenceCounted.hpp"
#include "Singleton.hpp"

#include <cstring>

/* *** Intrusive_list *** */

class Int_list_res : public Containers::IntrusiveList::Node < Int_list_res >
{
public:
    Int_list_res() = default;
    ~Int_list_res() = default;

    Platform::uint32 m_res = 0;
};

UNIT_TEST(Intrusive_list_initial_state)
{
    Int_list_res res;
    Int_list_res::List list;

    TEST_ASSERT(&res, res.Get());
    TEST_ASSERT((Int_list_res *)0, res.Next());
    TEST_ASSERT((Int_list_res::List *)0, res.Parent());
    TEST_ASSERT((Int_list_res *)0, res.Previous());

    TEST_ASSERT((Int_list_res *)0, list.First());
    TEST_ASSERT((Int_list_res *)0, list.Last());

    return Passed;
}

UNIT_TEST(Intrusive_list_attached)
{
    auto res = new Int_list_res;
    Int_list_res::List list;

    /* A */
    list.Attach(res);

    TEST_ASSERT((Int_list_res *)0, res->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res->Parent());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT(res, list.First());
    TEST_ASSERT(res, list.Last());

    /* A B */
    auto res_b = new Int_list_res;
    list.Attach(res_b);

    TEST_ASSERT(res_b, res->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res->Parent());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT((Int_list_res *)0, res_b->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_b->Parent());
    TEST_ASSERT(res, res_b->Previous());

    TEST_ASSERT(res, list.First());
    TEST_ASSERT(res_b, list.Last());

    /* A B C */
    auto res_c = new Int_list_res;
    list.Attach(res_c);

    TEST_ASSERT(res_b, res->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res->Parent());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT(res_c, res_b->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_b->Parent());
    TEST_ASSERT(res, res_b->Previous());

    TEST_ASSERT((Int_list_res *)0, res_c->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_c->Parent());
    TEST_ASSERT(res_b, res_c->Previous());

    TEST_ASSERT(res, list.First());
    TEST_ASSERT(res_c, list.Last());

    /* A C */
    list.Detach(res_b);

    TEST_ASSERT(res_c, res->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res->Parent());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT((Int_list_res *)0, res_b->Next());
    TEST_ASSERT((Int_list_res::List *)0, res_b->Parent());
    TEST_ASSERT((Int_list_res *)0, res_b->Previous());

    TEST_ASSERT((Int_list_res *)0, res_c->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_c->Parent());
    TEST_ASSERT(res, res_c->Previous());

    TEST_ASSERT(res, list.First());
    TEST_ASSERT(res_c, list.Last());

    /* A */
    list.Detach(res_c);

    TEST_ASSERT((Int_list_res *)0, res->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res->Parent());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT((Int_list_res *)0, res_b->Next());
    TEST_ASSERT((Int_list_res::List *)0, res_b->Parent());
    TEST_ASSERT((Int_list_res *)0, res_b->Previous());

    TEST_ASSERT((Int_list_res *)0, res_c->Next());
    TEST_ASSERT((Int_list_res::List *)0, res_c->Parent());
    TEST_ASSERT((Int_list_res *)0, res_c->Previous());

    TEST_ASSERT(res, list.First());
    TEST_ASSERT(res, list.Last());

    /* B */
    list.Attach(res_b);
    list.Detach(res);

    TEST_ASSERT((Int_list_res *)0, res->Next());
    TEST_ASSERT((Int_list_res::List *)0, res->Parent());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT((Int_list_res *)0, res_b->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_b->Parent());
    TEST_ASSERT((Int_list_res *)0, res_b->Previous());

    TEST_ASSERT((Int_list_res *)0, res_c->Next());
    TEST_ASSERT((Int_list_res::List *)0, res_c->Parent());
    TEST_ASSERT((Int_list_res *)0, res_c->Previous());

    TEST_ASSERT(res_b, list.First());
    TEST_ASSERT(res_b, list.Last());

    /* Exchange A B C to C B A */
    list.Detach(res_b);
    list.Attach(res);
    list.Attach(res_b);
    list.Attach(res_c);
    list.Exchange(res, res_c);

    TEST_ASSERT((Int_list_res *)0, res->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res->Parent());
    TEST_ASSERT((Int_list_res *)res_b, res->Previous());

    TEST_ASSERT((Int_list_res *)res, res_b->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_b->Parent());
    TEST_ASSERT((Int_list_res *)res_c, res_b->Previous());

    TEST_ASSERT((Int_list_res *)res_b, res_c->Next());
    TEST_ASSERT((Int_list_res::List *)&list, res_c->Parent());
    TEST_ASSERT((Int_list_res *)0, res_c->Previous());

    TEST_ASSERT(res_c, list.First());
    TEST_ASSERT(res, list.Last());

    return Passed;
}

UNIT_TEST(Intrusive_list_move)
{
    auto res = new Int_list_res;
    auto res_b = new Int_list_res;
    auto res_c = new Int_list_res;
    Int_list_res::List list;

    /* A B C */
    list.Attach(res);
    list.Attach(res_b);
    list.Attach(res_c);

    {
        Int_list_res::List list_2(std::move(list));

        TEST_ASSERT((Int_list_res *) 0, list->First());
        TEST_ASSERT(res, list_2->First());
        TEST_ASSERT(&list_2, res->Parent());
    }

    return Passed;
}



UNIT_TEST(Intrusive_list_search_and_sort)
{
    auto res = new Int_list_res;
    auto res_b = new Int_list_res;
    auto res_c = new Int_list_res;
    Int_list_res::List list;

    /* A B C */
    list.Attach(res);
    list.Attach(res_b);
    list.Attach(res_c);

    /* Search */
    res->m_res = 1;
    res_b->m_res = 2;
    res_c->m_res = 3;

    /* B */
    TEST_ASSERT(res_b, list.Search([&](Int_list_res & p) -> bool { return p.m_res == res_b->m_res; }));

    /* C */
    TEST_ASSERT(res_c, list.Search([&](Int_list_res & p) -> Platform::uint32 { return p.m_res; }, res_c->m_res));

    /* Sort C B A*/
    list.Sort([](Int_list_res & l, Int_list_res & r) -> Platform::uint32 { return r.m_res - l.m_res; });

    TEST_ASSERT((Int_list_res *)0, res->Next());
    TEST_ASSERT((Int_list_res *)res_b, res->Previous());

    TEST_ASSERT((Int_list_res *)res, res_b->Next());
    TEST_ASSERT((Int_list_res *)res_c, res_b->Previous());

    TEST_ASSERT((Int_list_res *)res_b, res_c->Next());
    TEST_ASSERT((Int_list_res *)0, res_c->Previous());

    /* Sort A B C */
    list.Sort([](Int_list_res & l, Int_list_res & r) -> Platform::uint32 { return l.m_res - r.m_res; });

    TEST_ASSERT((Int_list_res *)res_b, res->Next());
    TEST_ASSERT((Int_list_res *)0, res->Previous());

    TEST_ASSERT((Int_list_res *)res_c, res_b->Next());
    TEST_ASSERT((Int_list_res *)res, res_b->Previous());

    TEST_ASSERT((Int_list_res *)0, res_c->Next());
    TEST_ASSERT((Int_list_res *)res_b, res_c->Previous());

    return Passed;
}


/* *** Reference_counted *** */

class Ref_counted_res : public Containers::ReferenceCounted::Resource< Ref_counted_res >
{
public:
    Ref_counted_res() = default;
    virtual ~Ref_counted_res() = default;
};

class Ref_counted_event_handler : public Ref_counted_res::Event_handler
{
public:
    virtual void On_resource_destruction(Ref_counted_res * res)
    {
        m_counter = res->Get_references_number();
        m_was_called_on_resource_destruction = true;
    }

    virtual void On_last_reference_gone(
        Ref_counted_res * res,
        bool & should_resource_be_destoyed)
    {
        m_counter = res->Get_references_number();
        m_was_called_on_last_reference_gone = true;

        should_resource_be_destoyed = m_should_resource_be_destroyed;
    }

    Ref_counted_res::ref_count_t m_counter = 0;
    bool m_should_resource_be_destroyed = false;
    bool m_was_called_on_last_reference_gone = false;
    bool m_was_called_on_resource_destruction = false;
};


UNIT_TEST(Reference_counted_initial_state)
{
    Ref_counted_res res;

    TEST_ASSERT(Ref_counted_res::ref_count_t(0), res.Get_references_number());
    TEST_ASSERT((Ref_counted_res::Event_handler *) 0, res.Get_event_handler());

    return Passed;
}

UNIT_TEST(Reference_counted_basic_event_handler)
{
    auto res = new Ref_counted_res;
    if (nullptr == res)
    {
        return NotAvailable;
    }
    Ref_counted_event_handler handler;

    TEST_ASSERT((Ref_counted_res::Event_handler *) 0, res->Get_event_handler());

    res->Set_event_handler(&handler);
    TEST_ASSERT((Ref_counted_res::Event_handler *) &handler, res->Get_event_handler());

    delete res;
    TEST_ASSERT(true, handler.m_was_called_on_resource_destruction);
    TEST_ASSERT(false, handler.m_was_called_on_last_reference_gone);
    TEST_ASSERT(Ref_counted_res::ref_count_t(0), handler.m_counter);
                     
    return Passed;
}

UNIT_TEST(Reference_counted_reference)
{
    auto res = new Ref_counted_res;
    if (nullptr == res)
    {
        return NotAvailable;
    }
    Ref_counted_event_handler handler;
    res->Set_event_handler(&handler);

    Ref_counted_res::Reference ref(res);
    TEST_ASSERT(ref.Get(), res);

    TEST_ASSERT(Ref_counted_res::ref_count_t(1), res->Get_references_number());

    /* Release & Reset */
    {
        Ref_counted_res::Reference ref_b(res);
        TEST_ASSERT(ref_b.Get(), res);
        TEST_ASSERT(Ref_counted_res::ref_count_t(2), res->Get_references_number());

        ref_b.Release();
        TEST_ASSERT(Ref_counted_res::ref_count_t(1), res->Get_references_number());
        TEST_ASSERT(ref_b.Get(), (Ref_counted_res *) 0);

        ref_b.Reset(res);
        TEST_ASSERT(Ref_counted_res::ref_count_t(2), res->Get_references_number());
        TEST_ASSERT(ref_b->Get_references_number(), ref->Get_references_number());
        TEST_ASSERT(ref_b.Get(), res);
    }

    TEST_ASSERT(Ref_counted_res::ref_count_t(1), ref->Get_references_number());

    /* Copy ctr & op */
    {
        Ref_counted_res::Reference ref_a(ref);
        const Ref_counted_res::Reference ref_b(ref_a);
        Ref_counted_res::Reference ref_c(std::move(ref_b));

        TEST_ASSERT(Ref_counted_res::ref_count_t(4), res->Get_references_number());

        ref_a.Release();
        TEST_ASSERT(Ref_counted_res::ref_count_t(3), res->Get_references_number());
        ref_a = ref_b;
        TEST_ASSERT(Ref_counted_res::ref_count_t(4), res->Get_references_number());

        ref_c.Release();
        TEST_ASSERT(Ref_counted_res::ref_count_t(3), res->Get_references_number());
        ref_c = std::move(ref_b);
        TEST_ASSERT(Ref_counted_res::ref_count_t(4), res->Get_references_number());
    }

    TEST_ASSERT(Ref_counted_res::ref_count_t(1), ref->Get_references_number());

    /* Move ctr & op */
    {
        Ref_counted_res::Reference ref_a(ref);
        const Ref_counted_res::Reference ref_b(ref_a);
        TEST_ASSERT(Ref_counted_res::ref_count_t(3), res->Get_references_number());

        Ref_counted_res::Reference ref_c(std::move(ref_a));
        TEST_ASSERT(Ref_counted_res::ref_count_t(3), res->Get_references_number());
        TEST_ASSERT(ref_a.Get(), (Ref_counted_res *)0);

        ref_a = std::move(ref_c);
        TEST_ASSERT(Ref_counted_res::ref_count_t(3), res->Get_references_number());
        TEST_ASSERT(ref_c.Get(), (Ref_counted_res *)0);

        ref_c = std::move(ref_b);
        TEST_ASSERT(Ref_counted_res::ref_count_t(4), res->Get_references_number());
    }

    TEST_ASSERT(Ref_counted_res::ref_count_t(1), ref->Get_references_number());

    /* Destruction and event handling */
    {
        TEST_ASSERT(false, handler.m_was_called_on_resource_destruction);
        TEST_ASSERT(false, handler.m_was_called_on_last_reference_gone);
        TEST_ASSERT(Ref_counted_res::ref_count_t(0), handler.m_counter);

        handler.m_should_resource_be_destroyed = false;
        ref.Release();

        TEST_ASSERT(false, handler.m_was_called_on_resource_destruction);
        TEST_ASSERT(true, handler.m_was_called_on_last_reference_gone);
        TEST_ASSERT(Ref_counted_res::ref_count_t(0), handler.m_counter);

        handler.m_should_resource_be_destroyed = true;
        handler.m_was_called_on_last_reference_gone = false;
        ref.Reset(res);;
        if (nullptr == ref.Get())
        {
            return NotAvailable;
        }
        ref.Release();

        TEST_ASSERT(true, handler.m_was_called_on_resource_destruction);
        TEST_ASSERT(true, handler.m_was_called_on_last_reference_gone);
        TEST_ASSERT(Ref_counted_res::ref_count_t(0), handler.m_counter);
    }

    /* Access violation ? */
    ref.Release();

    return Passed;
}


/* *** Singleton *** */
class Single_res : public Containers::Singleton < Single_res >
{
public:
    Single_res() = default;
    virtual ~Single_res() = default;

    Platform::uint32 m_res = 0;
};

UNIT_TEST(Singleton)
{
    auto res = new Single_res;

    auto singleton = Single_res::Get_singleton();
    TEST_ASSERT(res, singleton);

    res->m_res = 1;

    Single_res::Release();

    singleton = Single_res::Get_singleton();
    TEST_ASSERT_NOT_EQUAL(1, singleton->m_res);

    delete singleton;

    return Passed;
}


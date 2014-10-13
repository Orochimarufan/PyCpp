/*****************************************************************************
 * PyC++ - C++ Python bindings
 * Copyright (C) 2014 MORI Taeyeon <orochimarufan.x3@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#pragma once

#include "Object.h"

namespace Py
{

class StopIteration;

namespace detail
{
void PYCPP_EXPORT throw_stopiter();
}

// Iterator class.
// E must be a subclass of Py::Object
template <typename E>
class PYCPP_EXPORT IteratorT : public ObjectPtr
{
    ObjectPtr stl_current;
    bool stl_end;

    void do_stl_next()
    {
        stl_current = PyIter_Next(ptr());
        if (stl_current.isNull())
        {
            auto_throw();
            // If we're still here, it ended
            stl_end = true;
        }
    }

public:
    // Construct
    IteratorT(const IteratorT<E> &o)
        : ObjectPtr(o), stl_current(o.stl_current), stl_end(o.stl_end)
    {
    }

    IteratorT(IteratorT<E> &&it)
        : ObjectPtr(it), stl_current(std::move(it.stl_current)), stl_end(it.stl_end)
    {
    }

    IteratorT() : ObjectPtr(nullptr, false), stl_end(true)
    {
    }

    IteratorT<E> &operator=(const IteratorT<E> &it)
    {
        Object::operator=(it);
        stl_current = it.stl_current;
        stl_end = it.stl_end;
    }

    IteratorT<E> &operator=(IteratorT<E> &&it)
    {
        Object::operator=(it);
        stl_current = std::move(it.stl_current);
        stl_end = it.stl_end;
    }

    PYCPP_OBJECT_DEF_POINTER_ASSIGNMENT(IteratorT<E>)

    explicit IteratorT(const Object &o)
        : ObjectPtr(PyObject_GetIter(o.ptr()), true), stl_end(false)
    {
    }

    static IteratorT<E> begin(const Object &o)
    {
        return IteratorT<E>(o);
    }

    static IteratorT<E> end(const Object &)
    {
        return IteratorT<E>();
    }

    static inline bool valid(PyObject *o)
    {
        return !o || PyIter_Check(o);
    }

    // STL interface
    typedef IteratorT<E> iterator;
    typedef E value_type;

    IteratorT<E> begin()
    {
        return *this;
    }

    IteratorT<E> end()
    {
        IteratorT<E> self(ptr()); // Don't copy stl_current
        self.stl_end = true;
        return self;
    }

    IteratorT<E> &operator++()
    {
        do_stl_next();
        return *this;
    }

    E operator*()
    {
        if (stl_current.isNull())
            do_stl_next();
        assert(!stl_end && "Must not deref a finished iterator");
        return stl_current;
    }

    bool operator==(const IteratorT<E> &it)
    {
        if (!stl_end && stl_current.isNull()) // stl_end might become true
            do_stl_next();

        return stl_end == it.stl_end;
    }

    bool operator!=(const IteratorT<E> &it)
    {
        return !operator==(it);
    }

    // Pythonic interface
    E next()
    {
        do_stl_next();
        if (stl_current.isNull())
            detail::throw_stopiter();
        return stl_current;
    }
};

PYCPP_OBJECT_IMPL_POINTER_ASSIGNMENT(IteratorT, ObjectPtr, template <typename E> inline, <E>)

typedef IteratorT<Object> Iterator;
}

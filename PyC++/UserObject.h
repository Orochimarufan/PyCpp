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
#include "Tuple.h"
#include "Dict.h"

namespace Py {

// Macro
#define PyCppUserObject(type, parent, py_type)\
public:\
    using parent::parent;\
    using parent::operator=;\
    \
    inline static PyObject *pyType()\
    {\
        return py_type;\
    }\
    \
    type() :\
        parent(PyObject_CallObject(py_type, NULL), true)\
    {}\
    \
    explicit type(const Py::Tuple &args) :\
        parent(PyObject_CallObject(py_type, args.ptr()), true)\
    {}\
    \
    explicit type(const Py::Tuple &args, const Py::Dict &kw) :\
        parent(PyObject_Call(py_type, args.ptr(), kw.ptr()), true)\
    {}\
    \
    template <typename... Args>\
    inline static type create(Args... targs)\
    {\
        return type(Py::Tuple({targs...}));\
    }
    /* ATTENTION: the Variadic constructor MUST NOT shadow the other constructors! */\
    /* This means that x(Tuple) and x(Tuple, Dict) can't be used unless cast down to Object */\
    /*template <typename... Args>\
    explicit type(Object a, Object b, Args... targs) :\
        type(Py::Tuple({a, b, targs...}))\
    {}\
    \
    explicit type(Py::Object a) :\
        type(Py::Tuple({a}))\
    {}*/


// Base class
class PYCPP_EXPORT UserObject : public Object
{
public:
    inline static PyObject *pyType()
    {
        return NULL;
    }

    UserObject() :
        Object()
    {
    }

    // Copy/Move
    UserObject(const UserObject &o) :
        Object(o)
    {
    }

    UserObject(UserObject &&o) :
        Object(o)
    {
    }

    // Ptr
    explicit UserObject(PyObject *o, bool steal=false) :
        Object(o, steal)
    {
    }

    // Assign
    inline UserObject &operator=(const UserObject &o)
    {
        Object::operator =(o);
        return *this;
    }

    inline UserObject &operator=(UserObject &&o)
    {
        Object::operator =(o);
        return *this;
    }
};

} // namespace Py

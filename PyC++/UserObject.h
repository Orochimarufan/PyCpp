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

namespace Py
{

// Macro
#define PyCppUserObject(type, parent, py_type)                                                 \
    PYCPP_OBJECT_DEF_DEFAULTS(type)                                                            \
                                                                                               \
    inline static PyObject *pyType()                                                           \
    {                                                                                          \
        return py_type;                                                                        \
    }                                                                                          \
                                                                                               \
    type() : parent(PyObject_CallObject(py_type, NULL), true)                                  \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    explicit type(const Py::Tuple &args)                                                       \
        : parent(PyObject_CallObject(py_type, args.ptr()), true)                               \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    explicit type(const Py::Tuple &args, const Py::Dict &kw)                                   \
        : parent(PyObject_Call(py_type, args.ptr(), kw.ptr()), true)                           \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    template <typename... Args>                                                                \
    inline static type create(Args... targs)                                                   \
    {                                                                                          \
        return type(Py::Tuple({ targs... }));                                                  \
    }

// Base class
class PYCPP_EXPORT UserObject : public Object
{
public:
    inline static PyObject *pyType()
    {
        return NULL;
    }

    PYCPP_OBJECT_DEF_DEFAULTS(UserObject)

    UserObject() : Object()
    {
    }
};

PYCPP_OBJECT_IMPL_DEFAULTS(UserObject, Object, inline)

} // namespace Py

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

#include "PyC++Config.h"
#include "exceptions.h"

#include <functional>
#include <unordered_map>

#include <Python.h>

#include "global.h"

namespace Py {

template <typename E>
struct ThrowException
{
    void operator()(PyObject *tp, PyObject *exc, PyObject *tb)
    {
        throw E(tp, exc, tb);
    }
};

struct PYCPP_EXPORT ExceptionRegistry
{
    typedef std::function<void(PyObject *, PyObject *, PyObject *)> raise_func;

private:
    static std::unordered_map<PyTypeObject*, raise_func> registry;

    friend void finalize();

public:
    ExceptionRegistry() = delete;

    template <typename T>
    static void registerType(PyObject *type, T raise)
    {
        registry.insert(std::make_pair((PyTypeObject*)type, raise));
    }

    template <typename T>
    static void registerType()
    {
        registry.insert(std::make_pair((PyTypeObject*)T::pyType(), ThrowException<T>()));
    }

    static void throwException(PyObject *type, PyObject *exc, PyObject *tb)
    {
        if (!PyType_Check(type))
            TypeError("ExceptionRegistry::throwException() expects type, exception, traceback.").raise();
        //print(type);
        PyTypeObject *x = (PyTypeObject*)type;
        auto it = registry.find(x);
        while (it == registry.end())
        {
            x = x->tp_base;
            it = registry.find(x);
            //print((PyObject*)x);
        }
        it->second(type, exc, tb);
    }

    static void throwCurrentException()
    {
        PyObject *type, *exc, *tb;
        PyErr_Fetch(&type, &exc, &tb);
        throwException(type, exc, tb);
    }
};

} // namespace Py

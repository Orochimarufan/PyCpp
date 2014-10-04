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

#include "Object.h"
#include "exceptions.h"

#include <Python.h>

#include <string>

#ifdef HAS_RTTI
#include <typeinfo>
#endif

namespace Py {

// Object management
void Object::set(PyObject *object, bool steal)
{
    Py_CLEAR(mp_object);
    mp_object = object;
    if (!steal)
        Py_XINCREF(object);
    validate();
}

void Object::unset()
{
    if (mp_object)
        Py_CLEAR(mp_object);
}

void Object::validate()
{
    if (!valid(mp_object))
    {
#ifdef HAS_RTTI
        std::string s("Error creating ");
        s += (typeid(*this)).name();
        s += " object from ";

        if (mp_object != nullptr)
        {
            PyObject *repr = PyObject_Repr(mp_object);
            s += PyUnicode_AsUTF8(repr);
            Py_DECREF(repr);
        }
        else
            s += "(nullptr)";
#endif
        Py_CLEAR(mp_object);
        auto_throw();
#ifdef HAS_RTTI
        throw TypeError(s);
#else
        throw TypeError();
#endif
    }
}

bool Object::valid(PyObject *o) const
{
    return o != NULL;
}

// Refcount
void Object::decref()
{
    if (refcnt() < 2)
        throw RuntimeError("Attempted decref() suicide!");
    Py_XDECREF(mp_object);
}

Py_ssize_t Object::refcnt() const
{
    return mp_object->ob_refcnt;
}

// Type Constructory
Object::Object(int val) :
    Object(PyLong_FromLong(val), true)
{
}

Object::Object(long val) :
    Object(PyLong_FromLong(val), true)
{
}

Object::Object(long long int val) :
    Object(PyLong_FromLongLong(val), true)
{
}

Object::Object(double val) :
    Object(PyFloat_FromDouble(val), true)
{
}

Object::Object(const char* val) :
    Object(PyUnicode_FromString(val), true)
{
}

Object::Object(const std::string &val) :
    Object(PyUnicode_FromStringAndSize(val.c_str(), val.size()), true)
{
}

} // namespace Py

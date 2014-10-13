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

#include "CppObject.h"

#include <PyC++/exceptions.h>

#define check_overridden(Base, method) (typeid(&Base::method) != typeid(&method))

namespace Py
{

const char *Class::__name__ = "???";

// ========== Class ==========
Class::Class(const Tuple &args, const Dict &kw)
{
}

void Class::__init__(const Tuple &, const Dict &)
{
}

Class::~Class()
{
}

Class::Class()
{
}

// Stringify
String Class::__repr__()
{
    return String("<C++ Object at 0x{:x}>").m("format", (intptr_t) this);
}

String Class::__str__()
{
    return __repr__();
}

// Hash
Py_hash_t Class::__hash__()
{
    throw TypeError("not hashable.");
}

// Call
Object Class::operator()(const Tuple &args, const Dict &kw)
{
    throw TypeError("not callable");
}

// Attributes
Object Class::__getattr__(const String &attr)
{
    return Object(PyObject_GenericGetAttr(static_cast<PyObject *>(this), attr.ptr()), true);
}

void Class::__setattr__(const String &attr, const Object &value)
{
    if (PyObject_GenericSetAttr(static_cast<PyObject *>(this), attr.ptr(), value.ptr()) < 0)
        auto_throw();
}

void Class::__delattr__(const String &attr)
{
    if (PyObject_GenericSetAttr(static_cast<PyObject *>(this), attr.ptr(), NULL) < 0)
        auto_throw();
}

// Garbage collection
int Class::__traverse__(visitproc visit, void *arg)
{
    return 1;
}

int Class::__clear__()
{
    return 1;
}

// Comparison
Object Class::__richcompare__(const Object &rhs, int op)
{
    /*if (op == Py_EQ && check_overridden(Class, operator ==))
        return operator ==(rhs);
    else if (op == Py_NE && check_overridden(Class, operator !=))
        return operator !=(rhs);
    else if (op == Py_LT && check_overridden(Class, operator <))
        return operator <(rhs);
    else if (op == Py_LE && check_overridden(Class, operator <=))
        return operator <=(rhs);
    else if (op == Py_GE && check_overridden(Class, operator >=))
        return operator >=(rhs);
    else if (op == Py_GT && check_overridden(Class, operator >))
        return operator >(rhs);
    else*/
    return Py_NotImplemented;
}

Object Class::operator==(const Object &rhs)
{
    throw NotImplementedError("operator == not implemented.");
}

Object Class::operator!=(const Object &rhs)
{
    throw NotImplementedError("operator != not implemented");
}

Object Class::operator<(const Object &rhs)
{
    throw NotImplementedError("operator < not implemented");
}

Object Class::operator<=(const Object &rhs)
{
    throw NotImplementedError("operator <= not implemented");
}

Object Class::operator>=(const Object &rhs)
{
    throw NotImplementedError("operator >= not implemented");
}

Object Class::operator>(const Object &rhs)
{
    throw NotImplementedError("operator > not implemented");
}

// Iterator Interface
Object Class::__iter__()
{
    throw TypeError("Not iterable");
}

Object Class::__next__()
{
    throw TypeError("Not iterable");
}

// Descriptor Interface
Object Class::__get__(const Object &obj, const Type &type)
{
    throw RuntimeError("Fatal: CppClass::__get__ called");
}

Object Class::__set__(const Object &obj, const Object &value)
{
    throw RuntimeError("Fatal: CppClass::__set__ called");
}

// Print
void Class::__print__(FILE *file, int flags)
{
    throw RuntimeError("Fatal: CppClass::__print__ called");
}
}

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

#include "String.h"
#include "List.h"
#include "Module.h"

namespace Py {

extern Object None;
extern Object True;
extern Object False;

void PYCPP_EXPORT auto_throw();

// str()
inline PYCPP_EXPORT String str(const Object &object)
{
    return String(PyObject_Str(object.ptr()), true);
}

inline PYCPP_EXPORT String str(const char *str)
{
    return String(str);
}

inline PYCPP_EXPORT String str(std::string str)
{
    return String(str);
}

inline PYCPP_EXPORT String str()
{
    return String();
}

// repr()
inline PYCPP_EXPORT String repr(const Object &object)
{
    return String(PyObject_Str(object.ptr()), true);
}

// dir()
inline PYCPP_EXPORT List dir(const Object &object)
{
    return List(PyObject_Dir(object.ptr()), true);
}

// len()
inline PYCPP_EXPORT long len(const Object &object)
{
    long res = PyObject_Length(object.ptr());
    if (res < 0)
        auto_throw();
    return res;
}

// type()
inline PYCPP_EXPORT Type type(const Object &object)
{
    return Type(PyObject_Type(object.ptr()), true);
}

// import_module()
inline PYCPP_EXPORT Module import_module(const char *module)
{
    return Module(PyImport_ImportModule(module), true);
}

// print()
template <typename... Args>
inline PYCPP_EXPORT void print(Args... targs)
{
    import_module("builtins").m("print", targs...);
    auto_throw();
}

} // namespace Py

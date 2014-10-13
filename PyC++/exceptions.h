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

#include "Exception.h"
#include "String.h"

#define PYEXC(name, base, py_tp)                                                               \
    PyCppUserObject(name, base, py_tp) explicit name(const char *reason)                       \
        : name(Py::Tuple({ Py::Object(reason) }))                                              \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    explicit name(PyObject *type, PyObject *value, PyObject *tb) : base(type, value, tb)       \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    explicit name(const std::string &reason) : name(Py::Tuple({ Py::Object(reason) }))         \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    explicit name(const Py::String &reason) : name(Py::Tuple({(Py::Object)reason }))           \
    {                                                                                          \
    }                                                                                          \
                                                                                               \
    template <typename... Args>                                                                \
    static inline name fromFormat(const char *fmt, Args... targs)                              \
    {                                                                                          \
        return name(Py::Tuple({ Py::String::fromFormat(fmt, targs...) }));                     \
    }

namespace Py
{

class PYCPP_EXPORT SystemExit : public BaseException
{
    PYEXC(SystemExit, BaseException, PyExc_SystemExit)
};

class PYCPP_EXPORT Exception : public BaseException
{
    PYEXC(Exception, BaseException, PyExc_Exception)
};

class PYCPP_EXPORT AttributeError : public Exception
{
    PYEXC(AttributeError, Exception, PyExc_AttributeError)
};

class PYCPP_EXPORT IndexError : public Exception
{
    PYEXC(IndexError, Exception, PyExc_IndexError)
};

class PYCPP_EXPORT TypeError : public Exception
{
    PYEXC(TypeError, Exception, PyExc_TypeError)
};

class PYCPP_EXPORT RuntimeError : public Exception
{
    PYEXC(RuntimeError, Exception, PyExc_RuntimeError)
};

class PYCPP_EXPORT NotImplementedError : public Exception
{
    PYEXC(NotImplementedError, Exception, PyExc_NotImplementedError)
};

class PYCPP_EXPORT StopIteration : public Exception
{
    PYEXC(StopIteration, Exception, PyExc_StopIteration)
};

} // namespace Py

#undef PYEXC

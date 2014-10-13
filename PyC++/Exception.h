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

#include "UserObject.h"
#include "Type.h"
#include "Traceback.h"

namespace Py
{

void PYCPP_EXPORT auto_throw();

class PYCPP_EXPORT BaseException : public UserObject, public std::exception
{
    PyObject *mp_traceback;

public:
    // PyC++ User Object
    inline static PyObject *pyType()
    {
        return PyExc_BaseException;
    }

    PYCPP_OBJECT_INLINE_VALID(PyExceptionInstance_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(BaseException)

    // ===== Construct =====
    // Need new constructors because mp_traceback
    BaseException()
        : mp_traceback(NULL), UserObject(PyObject_CallObject(PyExc_BaseException, NULL), true)
    {
        PYCPP_OBJECT_VALIDATE();
    }

    explicit BaseException(const Tuple &args)
        : mp_traceback(NULL),
          UserObject(PyObject_CallObject(PyExc_BaseException, args.ptr()), true)
    {
        PYCPP_OBJECT_VALIDATE();
    }

    explicit BaseException(const Tuple &args, const Dict &kw)
        : mp_traceback(NULL),
          UserObject(PyObject_Call(PyExc_BaseException, args.ptr(), kw.ptr()), true)
    {
        PYCPP_OBJECT_VALIDATE();
    }

    // Restore
    explicit BaseException(PyObject *type, PyObject *exc, PyObject *tb);

    // Destruct
    ~BaseException();

    // ===== Throwing =====
    // Raise/Re-raise Python Exception.
    void raise() const;

    // ===== Catching =====
    Traceback traceback() const;
    void print() const;

    // ===== STL Interaction =====
    const char *what() const noexcept;

    // ===== Boundaries =====
    // Python->C++ Transition
    static bool hasOccured();
    static void autoThrow();

    /**
     * @brief Exception guard.
     * Usage:
     *  return Exception::throwGuard(python_api_call(...));
     */
    template <typename T>
    inline static T throwGuard(T v)
    {
        autoThrow();
        return v;
    }
};

PYCPP_OBJECT_IMPL_DEFAULTS(BaseException, UserObject, inline)

} // namespace Py

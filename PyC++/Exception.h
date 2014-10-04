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

namespace Py {

class PYCPP_EXPORT BaseException : public UserObject, public std::exception
{
    PyObject *mp_traceback;

public:
    // PyC++ User Object
    inline static PyObject *pyType()
    {
        return PyExc_BaseException;
    }

    // Type check
    bool valid(PyObject *o) const
    {
        return o && PyExceptionInstance_Check(o);
    }

    // ===== Construct =====
    // Need new constructors because mp_traceback
    BaseException() :
        mp_traceback(NULL), UserObject(PyObject_CallObject(PyExc_BaseException, NULL), true)
    {
    }

    explicit BaseException(const Tuple &args) :
        mp_traceback(NULL), UserObject(PyObject_CallObject(PyExc_BaseException, args.ptr()), true)
    {
    }

    explicit BaseException(const Tuple &args, const Dict &kw) :
        mp_traceback(NULL), UserObject(PyObject_Call(PyExc_BaseException, args.ptr(), kw.ptr()), true)
    {
    }

    // Forward PyObject* constructor
    explicit BaseException(PyObject *object, bool steal=false) :
        mp_traceback(NULL), UserObject(object, steal)
    {
    }

    // Copy & Move
    BaseException(const BaseException &e) :
        mp_traceback(e.mp_traceback), UserObject(e)
    {
        Py_XINCREF(mp_traceback);
    }

    BaseException(BaseException &&e) :
        mp_traceback(e.mp_traceback), UserObject(e)
    {
        e.mp_traceback = NULL;
    }

    // Restore
    explicit BaseException(PyObject *type, PyObject *exc, PyObject *tb);

    // Destruct
    ~BaseException();

    // Assign
    inline BaseException &operator=(const BaseException &e)
    {
        Py_XDECREF(mp_traceback);
        mp_traceback = e.mp_traceback;
        Py_XINCREF(mp_traceback);
        UserObject::operator=(e);
        return *this;
    }

    inline BaseException &operator=(BaseException &&e)
    {
        mp_traceback = e.mp_traceback;
        e.mp_traceback = NULL;
        UserObject::operator=(e);
        return *this;
    }

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

class PYCPP_EXPORT Exception : public BaseException
{
    PyCppUserObject(Exception, BaseException, PyExc_Exception)
};

} // namespace Py

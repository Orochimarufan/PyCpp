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

#include "Exception.h"
#include "ExceptionRegistry.h"
#include "Object.h"
#include "global.h"

#include <Python.h>

namespace Py
{

struct exc_guard
{
    bool occured;
    PyObject *type;
    PyObject *value;
    PyObject *traceback;

    exc_guard() : type(NULL), value(NULL), traceback(NULL)
    {
        if (occured = (PyErr_Occurred() != NULL))
        {
            PyErr_Fetch(&type, &value, &traceback);
            PyErr_Clear();
            PyErr_SetExcInfo(type, value, traceback);
        }
    }

    ~exc_guard()
    {
        if (occured)
        {
            if (PyErr_Occurred())
            // Exception while handling one
            {
                PyObject *new_type, *new_value, *new_traceback;
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Fetch(&new_type, &new_value, &new_traceback);
                PyErr_NormalizeException(&new_type, &new_value, &new_traceback);
                PyException_SetContext(new_value, value);
            }
            else
            {
                PyErr_SetExcInfo(NULL, NULL, NULL);
                PyErr_Restore(type, value, traceback);
            }
        }
        if (std::uncaught_exception())
            PyErr_SetString(PyExc_RuntimeError, "Uncaught C++ exception");
    }
};

BaseException::BaseException(PyObject *type, PyObject *exc, PyObject *tb)
{
    PyErr_NormalizeException(&type, &exc, &tb);

    mp_traceback = tb;

    {
        exc_guard g;
        set(exc);
    }
}

BaseException::~BaseException()
{
    if (mp_traceback)
        Py_DECREF(mp_traceback);
}

// ===== Throwing =====
void BaseException::raise() const
{
    if (mp_traceback)
        PyErr_Restore(PyObject_Type(ptr()), ptr(), mp_traceback);
    else
        PyErr_SetObject(pyType(), ptr());
}

// ===== Catching =====
Traceback BaseException::traceback() const
{
    if (mp_traceback)
        return mp_traceback;
    else
        return Traceback(PyException_GetTraceback(ptr()), true);
}

void BaseException::print() const
{
    PyObject *tb;
    if (mp_traceback)
        tb = mp_traceback;
    else
        tb = PyException_GetTraceback(ptr());
    if (!tb)
        tb = Py_None;
    PyObject *traceback = PyImport_ImportModule("traceback");
    if (hasOccured() && !mp_traceback && tb != Py_None)
        Py_XDECREF(tb);
    PyObject_CallMethod(traceback, "print_exception", "OOO", PyObject_Type(ptr()), ptr(), tb);
    if (!mp_traceback && tb != Py_None)
        Py_XDECREF(tb);
    autoThrow();
}

// ===== STL Interaction =====
const char *BaseException::what() const noexcept
{
    return str(*this).asUTF8();
}

// ===== Boundaries =====
// Python->C++ Transition
bool BaseException::hasOccured()
{
    return PyErr_Occurred() != NULL;
}

void BaseException::autoThrow()
{
    PyObject *type, *exc, *tb;
    PyErr_Fetch(&type, &exc, &tb);
    if (type != NULL)
        ExceptionRegistry::throwException(type, exc, tb);
}

void auto_throw()
{
    PyObject *type, *exc, *tb;
    PyErr_Fetch(&type, &exc, &tb);
    if (type != NULL)
        ExceptionRegistry::throwException(type, exc, tb);
}

} // namespace PyCpp

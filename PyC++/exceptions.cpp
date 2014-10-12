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

#include "ExceptionRegistry.h"
#include "exceptions.h"

namespace Py
{

// Add Python type info
/*#define PYEXC(cpp,py) PyObject *cpp::pyType() const { return py; } \
                      PyObject *cpp::_pyType() { return py; }

PYEXC(BaseException,    PyExc_BaseException)
PYEXC(SystemExit,       PyExc_SystemExit)

PYEXC(Exception,        PyExc_Exception)
PYEXC(AttributeError,   PyExc_AttributeError)
PYEXC(IndexError,       PyExc_IndexError)
PYEXC(TypeError,        PyExc_TypeError)
PYEXC(RuntimeError,     PyExc_RuntimeError)

#undef PYEXC*/

// Initialize Exception Registry
std::unordered_map<PyTypeObject *, ExceptionRegistry::raise_func> ExceptionRegistry::registry;

PYCPP_HIDDEN void __init_exceptions()
{
    ExceptionRegistry::registerType<BaseException>();
    ExceptionRegistry::registerType<SystemExit>();

    ExceptionRegistry::registerType<Exception>();
    ExceptionRegistry::registerType<AttributeError>();
    ExceptionRegistry::registerType<IndexError>();
    ExceptionRegistry::registerType<TypeError>();
    ExceptionRegistry::registerType<RuntimeError>();
}

} // namespace Py

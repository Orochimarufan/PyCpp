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

#include "PyC++.h"
#include "Exception.h"
#include "ExceptionRegistry.h"

#include "exceptions.h"

#include <Python.h>

namespace Py {

// Global Objects
Object PYCPP_EXPORT None(Py_None, true);
Object PYCPP_EXPORT True(Py_True, true);
Object PYCPP_EXPORT False(Py_False, true);

// Empty objects
static Nullable<Tuple> empty_tuple;
static Nullable<Dict> empty_dict;

const Tuple &Tuple::empty()
{
    return empty_tuple;
}

const Dict &Dict::empty()
{
    return empty_dict;
}

// Initialization & Finalization
void __init_exceptions();

bool pycpp_initialized = false;

void initialize(bool sigs)
{
    if (pycpp_initialized)
        return;

    // Initialize Python
    Py_InitializeEx(sigs ? 1 : 0);

    __init_exceptions();

    // Empty Objects
    empty_tuple = Tuple();
    empty_dict = Dict();

    Run::MainModule = import_module("__main__");

    pycpp_initialized = true;
}

void finalize(void)
{
    if (!pycpp_initialized)
        return;

    Run::MainModule = nullptr;

    // Empty Objects
    empty_tuple = nullptr;
    empty_dict = nullptr;

    ExceptionRegistry::registry.clear();

    pycpp_initialized = false;

    // Finalize Python
    Py_Finalize();
}

namespace Run {

Nullable<Module> PYCPP_EXPORT MainModule;

int SimpleString(const char *code, PyCompilerFlags *flags)
{
    return PyRun_SimpleStringFlags(code, flags);
}

Object String(const char *code, Dict globals, Dict locals, int type, PyCompilerFlags *flags)
{
    return Object(PyRun_StringFlags(code, type, globals.ptr(), locals.ptr(), flags), true);
}

}

}

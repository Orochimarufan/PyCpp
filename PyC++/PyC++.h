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

#include "types.h"
#include "exceptions.h"
#include "global.h"

#include "Nullable.h"

namespace Py {

PYCPP_EXPORT void initialize(bool py_sigs=false);
PYCPP_EXPORT void finalize(void);

namespace Run {

extern Nullable<Module> MainModule;

PYCPP_EXPORT int SimpleString(const char *code, PyCompilerFlags *flags=NULL);
PYCPP_EXPORT Object String(const char *code, Dict globals=(Dict)MainModule, Dict locals=Dict(), int type=Py_file_input, PyCompilerFlags *flags=NULL);

}

} // namespace Py

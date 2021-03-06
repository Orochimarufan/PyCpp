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

#include "Sequence.h"

namespace Py
{

class PYCPP_EXPORT List : public Sequence
{
public:
    PYCPP_OBJECT_INLINE_VALID(PyList_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(List)

    List() = delete;
};

PYCPP_OBJECT_IMPL_DEFAULTS(List, Sequence, inline)

} // namespace Py

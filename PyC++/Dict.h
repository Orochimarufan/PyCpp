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

#include "PyC++Config.h"
#include "Object.h"
#include "Module.h"

namespace Py {

class PYCPP_EXPORT Dict : public CObject
{
public:
    using CObject::CObject;
    using CObject::operator =;

    Dict(const Module &m) :
        CObject(m)
    {
    }

    Dict() :
        CObject(PyDict_New(), true)
    {
    }

    bool valid(PyObject *o) const override
    {
        return o && PyDict_Check(o);
    }

    static const Dict &empty();
};

} // namespace Py

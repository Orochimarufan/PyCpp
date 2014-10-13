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

namespace Py
{

class PYCPP_EXPORT Bytes : public Object
{
public:
    PYCPP_OBJECT_INLINE_VALID(PyBytes_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(Bytes)

    Bytes() : Object(PyBytes_FromStringAndSize("", 0), true)
    {
    }

    Bytes(const char *txt) : Object(PyBytes_FromString(txt), true)
    {
    }

    Bytes(const std::string &txt) : Object(PyBytes_FromStringAndSize(txt.c_str(), txt.size()))
    {
    }

    inline std::string asStdString() const
    {
        return std::string(PyBytes_AsString(ptr()));
    }

    inline char *asString() const
    {
        return PyBytes_AsString(ptr());
    }
};

PYCPP_OBJECT_IMPL_DEFAULTS(Bytes, Object, inline)

} // namespace Py

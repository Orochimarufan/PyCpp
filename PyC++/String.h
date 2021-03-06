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

class PYCPP_EXPORT String : public Object
{
public:
    PYCPP_OBJECT_INLINE_VALID(PyUnicode_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(String)

    String() : Object(PyUnicode_New(0, 0), true)
    {
    }

    String(const char *txt) : Object(PyUnicode_FromString(txt), true)
    {
    }

    String(const std::string &txt)
        : Object(PyUnicode_FromStringAndSize(txt.c_str(), txt.size()))
    {
    }

    inline std::string asStdString() const
    {
        return std::string(PyUnicode_AsUTF8(ptr()));
    }

    inline char *asUTF8() const
    {
        return PyUnicode_AsUTF8(ptr());
    }

    template <typename... Args>
    static inline String fromFormat(const char *fmt, Args... targs)
    {
        return String(PyUnicode_Format(fmt, targs...), true);
    }
};

PYCPP_OBJECT_IMPL_DEFAULTS(String, Object, inline)

} // namespace Py

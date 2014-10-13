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

#include "Object.h"

namespace Py
{

class PYCPP_EXPORT Slice : public Object
{
public:
    PYCPP_OBJECT_INLINE_VALID(PySlice_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(Slice)

    Slice(const Object &start, const Object &stop, const Object &step)
        : Object(PySlice_New(start.ptr(), stop.ptr(), step.ptr()), true)
    {
    }

    struct Application
    {
        Py_ssize_t start;
        Py_ssize_t stop;
        Py_ssize_t step;
        Py_ssize_t length;
    };

    // Apply slice to a sequence of \c length
    inline Application apply(Py_ssize_t length)
    {
        Application sa;
        PySlice_GetIndicesEx(ptr(), length, &sa.start, &sa.stop, &sa.step, &sa.length);
        return sa;
    }

    inline Object start()
    {
        return getAttr("start");
    }

    inline Object stop()
    {
        return getAttr("stop");
    }

    inline Object step()
    {
        return getAttr("step");
    }
};

PYCPP_OBJECT_IMPL_DEFAULTS(Slice, Object, inline)

} // namespace Py

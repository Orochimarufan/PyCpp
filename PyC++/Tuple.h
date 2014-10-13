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

#include <initializer_list>

namespace Py
{

class PYCPP_EXPORT Tuple : public Sequence
{
    friend void initialize(bool);
    friend void finalize();

public:
    PYCPP_OBJECT_INLINE_VALID(PyTuple_Check)
    PYCPP_OBJECT_DEF_DEFAULTS(Tuple)

    Tuple() : Sequence(PyTuple_New(0), true)
    {
    }

    Tuple(std::initializer_list<Object> il) : Sequence(PyTuple_New(il.size()), true)
    {
        auto it = il.begin();
        for (int i = 0; i < il.size(); ++i)
        {
            PyTuple_SET_ITEM(ptr(), i, it->newref());
            ++it;
        }
    }

    static const Tuple &empty();
};

PYCPP_OBJECT_IMPL_DEFAULTS(Tuple, Sequence, inline)

} // namespace Py

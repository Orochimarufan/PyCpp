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
    using Sequence::SequenceT;
    using Sequence::operator=;

    Tuple() : Sequence(PyTuple_New(0), true)
    {
    }

    explicit Tuple(Py_ssize_t size) : Sequence(PyTuple_New(size), true)
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

    bool valid(PyObject *o) const override
    {
        return o && PyTuple_Check(o);
    }

    static const Tuple &empty();
};

} // namespace Py

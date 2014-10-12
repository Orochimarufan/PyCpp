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
#include "Float.h"

namespace Py
{

class PYCPP_EXPORT Long : public CObject
{
public:
    using CObject::CObject;
    using CObject::operator=;

    // Construct
    Long(long val = 0L) : CObject(PyLong_FromLong(val), true)
    {
    }

    Long(int val) : CObject(PyLong_FromLong(val), true)
    {
    }

    Long(long long val) : CObject(PyLong_FromLongLong(val), true)
    {
    }

    // Type safety
    bool valid(PyObject *o) const override
    {
        return o && PyLong_Check(o);
    }

    // Compare > long long
    inline bool operator==(long long rhs) const
    {
        return PyLong_AsLongLong(ptr()) == rhs;
    }

    inline bool operator!=(long long rhs) const
    {
        return PyLong_AsLongLong(ptr()) != rhs;
    }

    inline bool operator<=(long long rhs) const
    {
        return PyLong_AsLongLong(ptr()) <= rhs;
    }

    inline bool operator>=(long long rhs) const
    {
        return PyLong_AsLongLong(ptr()) >= rhs;
    }

    inline bool operator<(long long rhs) const
    {
        return PyLong_AsLongLong(ptr()) < rhs;
    }

    inline bool operator>(long long rhs) const
    {
        return PyLong_AsLongLong(ptr()) > rhs;
    }

    // Compare > long
    inline bool operator==(long rhs) const
    {
        return PyLong_AsLong(ptr()) == rhs;
    }

    inline bool operator!=(long rhs) const
    {
        return PyLong_AsLong(ptr()) != rhs;
    }

    inline bool operator<=(long rhs) const
    {
        return PyLong_AsLong(ptr()) <= rhs;
    }

    inline bool operator>=(long rhs) const
    {
        return PyLong_AsLong(ptr()) >= rhs;
    }

    inline bool operator<(long rhs) const
    {
        return PyLong_AsLong(ptr()) < rhs;
    }

    inline bool operator>(long rhs) const
    {
        return PyLong_AsLong(ptr()) > rhs;
    }

    // Convert
    inline long long asLongLong() const
    {
        return PyLong_AsLongLong(ptr());
    }

    inline long asLong() const
    {
        return PyLong_AsLong(ptr());
    }

    explicit inline operator long() const
    {
        return PyLong_AsLong(ptr());
    }

    inline operator long long() const
    {
        return PyLong_AsLongLong(ptr());
    }

    explicit inline operator size_t() const
    {
        return PyLong_AsSize_t(ptr());
    }

    // In-place
    inline Long &operator+=(long long rhs)
    {
        set(PyLong_FromLongLong(asLongLong() + rhs), true);
        return *this;
    }

    inline Long &operator-=(long long rhs)
    {
        set(PyLong_FromLongLong(asLongLong() - rhs), true);
        return *this;
    }

    inline Long &operator*=(long long rhs)
    {
        set(PyLong_FromLongLong(asLongLong() - rhs), true);
        return *this;
    }

    inline Long &operator%=(long long rhs)
    {
        set(PyLong_FromLongLong(asLongLong() % rhs), true);
        return *this;
    }

    inline Long &operator&=(long long rhs)
    {
        set(PyLong_FromLongLong(asLongLong() & rhs), true);
    }

    inline Long &operator|=(long long rhs)
    {
        set(PyLong_FromLongLong(asLongLong() | rhs), true);
    }

    // Increase/Decrease
    inline Long &operator++()
    {
        set(PyLong_FromLongLong(asLongLong() + 1LL), true);
        return *this;
    }

    inline long long operator++(int)
    {
        long long x(*this);
        set(PyLong_FromLongLong(asLongLong() + 1LL), true);
        return x;
    }

    inline Long &operator--()
    {
        set(PyLong_FromLongLong(asLongLong() - 1LL), true);
        return *this;
    }

    inline long long operator--(int)
    {
        long long x(*this);
        set(PyLong_FromLongLong(asLongLong() - 1LL), true);
        return x;
    }
};

} // namespace Py

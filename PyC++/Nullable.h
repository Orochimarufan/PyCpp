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

namespace Py
{

template <typename T>
class Nullable
{
    T *it;

public:
    Nullable() : it(nullptr)
    {
    }

    Nullable(const T &i) : it(new T(i))
    {
    }

    Nullable(const Nullable<T> &o) : it(new T(*o.it))
    {
    }

    Nullable(Nullable<T> &&o) : it(o.it)
    {
        o.it = nullptr;
    }

    inline T *operator->()
    {
        return it;
    }

    operator T &()
    {
        return *it;
    }

    bool isNull()
    {
        return it == nullptr;
    }

    Nullable<T> &operator=(decltype(nullptr))
    {
        delete it;
        it = nullptr;
        return *this;
    }

    Nullable<T> &operator=(const T &i)
    {
        delete it;
        it = new T(i);
        return *this;
    }

    Nullable<T> &operator=(T &&i)
    {
        delete it;
        it = new T(i);
        return *this;
    }
};
}

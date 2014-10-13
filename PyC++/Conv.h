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

#include "Python.h"

#include <string>

namespace Py
{
namespace Conv
{

template <typename T>
inline PyObject *newFromValue(T value);

// Default conversions
template <>
inline PyObject *newFromValue(int value)
{
    return PyLong_FromLong(value);
}

template <>
inline PyObject *newFromValue(long value)
{
    return PyLong_FromLong(value);
}

template <>
inline PyObject *newFromValue(long long value)
{
    return PyLong_FromLongLong(value);
}

template <>
inline PyObject *newFromValue(const char *value)
{
    return PyUnicode_FromString(value);
}

template <>
inline PyObject *newFromValue(double value)
{
    return PyFloat_FromDouble(value);
}

template <>
inline PyObject *newFromValue(std::string value)
{
    return PyUnicode_FromStringAndSize(value.c_str(), value.size());
}

} // namespace Conv
} // namespace Py

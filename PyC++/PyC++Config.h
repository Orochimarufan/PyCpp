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

#if defined _WIN32 || defined __CYGWIN__
  #ifdef PYCPP_LIBRARY
    #define PYCPP_EXPORT __declspec(dllexport)
  #else
    #define PYCPP_EXPORT __declspec(dllimport)
  #endif
  #define PYCPP_HIDDEN
#elif __GNUC__ >= 4
  #define PYCPP_EXPORT __attribute__ ((visibility ("default")))
  #define PYCPP_HIDDEN __attribute__ ((visibility ("hidden")))
#else
  #define PYCPP_EXPORT
  #define PYCPP_HIDDEN
#endif

#if defined _CPPRTTI || defined __GNUG__
#define HAS_RTTI 1
#endif

// Forward PyObject
struct _object;
typedef _object PyObject;

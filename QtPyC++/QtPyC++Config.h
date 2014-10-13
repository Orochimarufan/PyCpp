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

#include <qglobal.h>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef QTPYCPP_LIBRARY
    #define QTPYCPP_EXPORT Q_DECL_EXPORT
  #else
    #define QTPYCPP_EXPORT Q_DECL_IMPORT
  #endif
  #define QTPYCPP_HIDDEN
#elif __GNUC__ >= 4
  #define QTPYCPP_EXPORT __attribute__ ((visibility ("default")))
  #define QTPYCPP_HIDDEN __attribute__ ((visibility ("hidden")))
#else
  #define QTPYCPP_EXPORT
  #define QTPYCPP_HIDDEN
#endif

#/*****************************************************************************
# * PyCpp - C++ Python Bindings
# * Copyright (C) 2014 MORI Taeyeon <orochimarufan.x3@gmail.com>
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *****************************************************************************/

project(PyC++)
cmake_minimum_required(VERSION 2.8.11)

########## C++14 ##########
IF(APPLE)
    # assume clang, add C++1y/C++14 stuff
    message(STATUS "Using APPLE CMAKE_CXX_FLAGS")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")
ELSEIF(UNIX)
    # assume GCC, add C++1y/C++14 stuff
    MESSAGE(STATUS "Using UNIX CMAKE_CXX_FLAGS")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
ELSEIF(MINGW)
    MESSAGE(STATUS "Using MINGW CMAKE_CXX_FLAGS")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")
ENDIF()

########## PyC++ ##########
# The base component. Provides Facilities to work with Python objects from C++
add_subdirectory(PyC++)
include_directories(PyC++)

########## PyC++Ex ###########
# The Extension component. Provides Facilities to implement Python classes in C++
add_subdirectory(PyC++Ex)
include_directories(PyC++Ex)

########## QtPyC++ ###########
# The Qt component. Provides interoperability with Qt types. Also allows seamless access to Qt slots from Python
#add_subdirectory(QtPyC++)

########## Test ##########
add_subdirectory(test)

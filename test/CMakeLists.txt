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

########## Headers ##########
set(HEADERS
)

########## Sources ##########
set(SOURCES
    main.cpp
)

########## Python ##########
find_package(PythonLibs 3.3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

########## Build ##########
add_executable(PyC++Test ${SOURCES} ${HEADERS})
target_link_libraries(PyC++Test ${PYTHON_LIBRARIES} PyC++ PyC++Ex)

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
# === Global stuff ===
    PyC++ExConfig.h
    PyC++Ex.h

# === Extending ===
    CppObject.h
    CppType_impl.h
)

########## Sources ##########
set(SOURCES
    CppObject.cpp
)

########## Python ##########
find_package(PythonLibs 3.3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

########## Build ##########
add_library(PyC++Ex SHARED ${SOURCES} ${HEADERS})
target_link_libraries(PyC++Ex PyC++)

########## Install ##########
set_target_properties(PyC++ PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS PyC++Ex EXPORT PyC++ExTargets COMPONENT PyC++Ex-dev
    RUNTIME DESTINATION bin COMPONENT PyC++Ex
    LIBRARY DESTINATION lib COMPONENT PyC++Ex
    ARCHIVE DESTINATION lib COMPONENT PyC++Ex
    PUBLIC_HEADER DESTINATION include/PyC++Ex)

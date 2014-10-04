# PyC++ #
PyC++ is a set of libraries to make working with and writing extensions for CPython in C++ easy.

### Features ###
 * Simple interfaces for builtin objects
 * Exception support
 * Simple interface for defining new types
 * Pythonic interfaces

### TODO ###
 * Finish the missing protocols (number, sequence, mapping, buffer)
 * Add Qt interop component
 * Add CMake library configs
 * Add interface to define Extension modules

### &alpha;lpha grade software ###
This is still alpha grade software, don't expect it to work flawlessly or have stable interfaces.

## Dependencies ##
 * Python (>=3.3, http://www.python.org, distro packages)

### Build dependencies ###
 * Recent g++/clang with C++14 (C++1y) support (distro packages)
 * CMake (>=2.8.11, http://www.cmake.org, distro packages)
 * Ninja (http://martine.github.io/ninja, distro packages)

## Building ##
```bash
git clone https://github.com/Orochimarufan/PyCpp.git PyC++
cd PyC++
mkdir build
cd build

cmake .. -G Ninja
ninja
```

Right now, you can only play around with the code in test/*

## License [![GPLv3][gpl-logo]][gpl-link]##
    PyC++ is (C) 2014 by MORI Taeyeon.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

I reserve the right to release the codebase under a less restrictive license at a later point for now.


[gpl-logo]: https://www.gnu.org/graphics/gplv3-88x31.png
[gpl-link]: https://www.gnu.org/copyleft/gpl.html

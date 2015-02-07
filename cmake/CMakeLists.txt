#
# CMakeLists.txt -- CMake build system for malikania
#
# Copyright (c) 2013, 2014, 2015
#   David Demelier <markand@malikania.fr>
#   Renaud Jenny <renox0@malikania.fr>
#   Alexis Dörr <nanahara@malikania.fr>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

project(cmake)

#
# The following files will be installed:
#
#  MalikaniaConfig	- Include all Malikania CMake files
#  MalikaniaVersion	- The version of the engine
#  MalikaniaSystem	- Platform specific definitions
#  MalikaniaMacros	- Malikania public macros
#
set(
	FILES
	MalikaniaConfig.cmake
	Macros.cmake
	System.cmake
	Version.cmake
)

if (WIN32)
	set(DIRECTORY "Malikania")
elseif (UNIX)
	set(DIRECTORY "share/malikania/cmake")
elseif (APPLE)
	set(DIRECTORY "malikania.framework/Resources/CMake")
endif ()

install(
	FILES ${FILES}
	DESTINATION ${DIRECTORY}
)

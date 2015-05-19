#
# CMakeLists.txt -- CMake build system for malikania
#
# Copyright (c) 2013, 2014, 2015 Malikania Authors
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

#
# Recent versions of CMake has nice C++ feature detection for modern
# C++ but they are still a bit buggy so we use this
# instead.
#
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	#
	# For GCC, we require at least GCC 5.1
	#
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.1")
		message(FATAL_ERROR "You need at least GCC 5.1")
	endif ()

	set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14 ${CMAKE_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	#
	# LLVM/clang implemented C++14 starting from version 3.4 but the
	# switch -std=c++14 was not available.
	#
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4")
		message(FATAL_ERROR "You need at least Clang 3.4")
	endif ()

	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.5")
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++1y ${CMAKE_CXX_FLAGS}")
	else ()
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14 ${CMAKE_CXX_FLAGS}")
	endif ()
else ()
	message(WARNING "Unsupported ${CMAKE_CXX_COMPILER_ID}, may not build correctly.")
endif ()

if (WIN32)
	set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0600 ${CMAKE_CXX_FLAGS}")
endif ()

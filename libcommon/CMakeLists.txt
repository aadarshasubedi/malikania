#
# CMakeLists.txt -- CMake build system for malikania
#
# Copyright (c) 2013-2016 Malikania Authors
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

set(
	HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Application.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Directory.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Game.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/ElapsedTimer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Game.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Hash.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Id.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Js.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Json.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/ResourcesLoader.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/ResourcesLocator.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Util.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Size.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Sockets.h
)

set(
	SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Directory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/ElapsedTimer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Hash.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Js.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Json.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/ResourcesLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/ResourcesLocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Util.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Sockets.cpp
)

if (WIN32)
	list(APPEND LIBRARIES ws2_32)
endif ()

malikania_create_library(
	PROJECT libcommon
	TARGET libcommon
	SOURCES ${HEADERS} ${SOURCES}
	FLAGS "MALIKANIA_COMMON_BUILD"
	PRIVATE_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/include/malikania
	PUBLIC_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${Jansson_INCLUDE_DIRS}
		${ZIP_INCLUDE_DIRS}
		${OPENSSL_INCLUDE_DIR}
	LIBRARIES
		extern-jansson
		${LIBRARIES}
		${Jansson_LIBRARIES}
		${ZIP_LIBRARIES}
		${OPENSSL_LIBRARIES}
)

set_target_properties(libcommon PROPERTIES PREFIX "")

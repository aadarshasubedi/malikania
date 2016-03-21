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
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Application.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Directory.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Game.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ElapsedTimer.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Game.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Hash.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Id.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Js.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Json.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ResourcesLoader.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ResourcesLocator.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Util.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Sockets.h
)

set(
	SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Directory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ElapsedTimer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Hash.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Js.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Json.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ResourcesLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ResourcesLocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Util.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Sockets.cpp
)

if (WITH_BACKEND_SDL)
	find_package(SDL2 REQUIRED)

	list(APPEND HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/CommonSdl.h)
	list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/CommonSdl.cpp)
	list(APPEND INCLUDES ${SDL2_INCLUDE_DIRS})
	list(APPEND LIBRARIES ${SDL2_LIBRARIES})
endif ()

if (WIN32)
	list(APPEND LIBRARIES ws2_32)
endif ()

malikania_create_library(
	PROJECT libcommon
	TARGET libcommon
	SOURCES ${HEADERS} ${SOURCES}
	FLAGS "MALIKANIA_COMMON_BUILD"
	PUBLIC_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}
		${Jansson_INCLUDE_DIRS}
		${ZIP_INCLUDE_DIRS}
		${OPENSSL_INCLUDE_DIR}
		${INCLUDES}
	LIBRARIES
		extern-jansson
		${LIBRARIES}
		${Jansson_LIBRARIES}
		${ZIP_LIBRARIES}
		${OPENSSL_LIBRARIES}
)

set_target_properties(libcommon PROPERTIES PREFIX "")

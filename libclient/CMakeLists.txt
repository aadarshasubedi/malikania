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
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Animation.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Animator.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ClientResourcesLoader.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Color.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Font.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Image.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Label.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Line.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Point.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Rectangle.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Size.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Sprite.h
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Window.h
)

set(
	SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Animator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/ClientResourcesLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Color.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Label.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Sprite.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/malikania/Window.cpp
)

if (WITH_BACKEND_SDL)
	list(
		APPEND
		HEADERS
			${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/ImageSdl.h
			${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/WindowSdl.h
			${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/FontSdl.h
	)
	list(
		APPEND
		SOURCES
			${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/ImageSdl.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/WindowSdl.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/malikania/backend/sdl/FontSdl.cpp
	)
	list(APPEND DEFINITIONS SDL_MAIN_HANDLED)
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

malikania_create_library(
	PROJECT libclient
	TARGET libclient
	SOURCES ${HEADERS} ${SOURCES}
	PUBLIC_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}
		${SDL2_INCLUDE_DIRS}
		${SDL2_IMAGE_INCLUDE_DIRS}
		${SDL2_TTF_INCLUDE_DIRS}
	LIBRARIES
		libcommon
		${SDL2_LIBRARIES}
		${SDL2_IMAGE_LIBRARIES}
		${SDL2_TTF_LIBRARIES}
)

target_compile_definitions(libclient PUBLIC ${DEFINITIONS})

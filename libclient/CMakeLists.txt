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

set(
	HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Animation.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Color.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Font.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Image.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Label.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Line.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Point.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Rectangle.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Size.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Sprite.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/Window.h
)

set(
	SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Animation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Color.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Font.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Image.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Label.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Sprite.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/Window.cpp
)

# TODO: move to a Sdl.cmake
if (WITH_BACKEND_SDL)
	list(
		APPEND
		HEADERS
			${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/backend/sdl/ImageSdl.h
			${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/backend/sdl/WindowSdl.h
			${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/backend/sdl/SpriteSdl.h
			${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/backend/sdl/FontSdl.h
			${CMAKE_CURRENT_SOURCE_DIR}/include/malikania/backend/sdl/LabelSdl.h
	)
	list(
		APPEND
		SOURCES
			${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/backend/sdl/ImageSdl.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/backend/sdl/WindowSdl.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/backend/sdl/SpriteSdl.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/backend/sdl/FontSdl.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/malikania/backend/sdl/LabelSdl.cpp
	)
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

malikania_create_library(
	PROJECT libclient
	TARGET libclient
	SOURCES ${HEADERS} ${SOURCES}
	PRIVATE_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/include/malikania
	PUBLIC_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${SDL2_INCLUDE_DIRS}
		${SDL2_IMAGE_INCLUDE_DIRS}
		${SDL2_TTF_INCLUDE_DIRS}
	LIBRARIES
		libcommon
		${SDL2_LIBRARIES}
		${SDL2_IMAGE_LIBRARIES}
		${SDL2_TTF_LIBRARIES}
)

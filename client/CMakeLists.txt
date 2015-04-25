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

project(client)

set(
	FILES
	main.cpp
	Window.h
	Window.cpp
	Image.h
	Image.cpp
	Rectangle.h
	Rectangle.cpp
	Size.h
	Size.cpp
	Position.h
	Position.cpp
	Sprite.h
	Sprite.cpp
)

# TODO: move to a Sdl.cmake
if (WITH_BACKEND_SDL)
	list(
		APPEND
		FILES
		backend/sdl/ImageSdl.cpp
		backend/sdl/ImageSdl.h
		backend/sdl/WindowSdl.cpp
		backend/sdl/WindowSdl.h
	)
endif ()

add_executable(client ${FILES})

target_include_directories(
	client
	PRIVATE
	${client_SOURCE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
)

target_link_libraries(
	client
	libcommon
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
)

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/resources/images/mokodemo.png
	DEPENDS ${client_SOURCE_DIR}/resources/images/mokodemo.png
	COMMAND
		${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/resources/images
	COMMAND
		${CMAKE_COMMAND} -E copy ${client_SOURCE_DIR}/resources/images/mokodemo.png ${CMAKE_BINARY_DIR}/resources/images/mokodemo.png
)

add_custom_target(
	client-resources ALL
	DEPENDS ${CMAKE_BINARY_DIR}/resources/images/mokodemo.png
	SOURCES ${client_SOURCE_DIR}/resources/images/mokodemo.png
)

add_dependencies(client-resources client)

if (APPLE)
	target_compile_options(libcommon PRIVATE "-Wno-deprecated-declarations")
endif ()

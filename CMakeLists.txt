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

cmake_minimum_required(VERSION 3.0)
project(malikania)

set(
	CMAKE_MODULE_PATH
	"${malikania_SOURCE_DIR}/cmake"
	"${malikania_SOURCE_DIR}/cmake/internal"
	"${malikania_SOURCE_DIR}/cmake/packages"
)

include(cmake/MalikaniaOptions.cmake)
include(cmake/MalikaniaFunctions.cmake)
include(cmake/MalikaniaVersion.cmake)
include(cmake/MalikaniaSystem.cmake)

configure_file(
	${malikania_SOURCE_DIR}/cmake/internal/Config.h.in
	${malikania_BINARY_DIR}/Config.h
)

include_directories(${CMAKE_BINARY_DIR})
enable_testing()

find_package(ZIP REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(extern)
add_subdirectory(docs)
add_subdirectory(libcommon)
add_subdirectory(libclient)
add_subdirectory(libserver)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)

message("Building information:")
message("      General flags:   ${CMAKE_CXX_FLAGS}")
message("      Debug flags:     ${CMAKE_CXX_FLAGS_DEBUG}")
message("      Release flags:   ${CMAKE_CXX_FLAGS_RELEASE}")
message("")
message("Documentation:")
message("      UML diagrams:    ${WITH_DOCS_UML_MSG}")
message("      Doxygen:         ${WITH_DOCS_DOXYGEN_MSG}")
message("      Books:           ${WITH_DOCS_BOOKS_MSG}")

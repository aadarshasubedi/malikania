#
# CMakeLists.txt -- CMake build system for malikania
#
# Copyright (c) 2013, 2014 David Demelier <markand@malikania.fr>
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

cmake_minimum_required(VERSION 2.8.12)

project(malikania)

set(
	CMAKE_MODULE_PATH
	"${malikania_SOURCE_DIR}/cmake"
	"${malikania_SOURCE_DIR}/cmake/internal"
	"${malikania_SOURCE_DIR}/cmake/packages"
)

include(MalikaniaConfig)
include(Options)

configure_file(
	${malikania_SOURCE_DIR}/cmake/internal/Config.h.in
	${malikania_BINARY_DIR}/Config.h
)

add_subdirectory(extern)
add_subdirectory(docs)
add_subdirectory(cmake)

message("Documentation:")
message("  UML diagrams:    ${WITH_DOCS_UML_MSG}")
message("  Doxygen:         ${WITH_DOCS_DOXYGEN_MSG}")
message("  Specifications:  ${WITH_DOCS_SPECS_MSG}")
message("  Coding style:    ${WITH_DOCS_CONVENTIONS_MSG}")

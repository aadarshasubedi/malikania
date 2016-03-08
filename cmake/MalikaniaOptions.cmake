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

#
# Backend selection.
# -------------------------------------------------------------------
#
# The following options are available:
#    WITH_BACKEND		- Type of backend to use. (Default: SDL)
#

set(WITH_BACKEND "SDL"
	CACHE STRING "Which backend to use")

#
# Documentation and technical references
# -------------------------------------------------------------------
#
# The following options are available:
#    WITH_DOCS			- Disable or enable all docs, if set to Off, disable all documentation.
#    WITH_DOCS_UML		- Enable UML diagram generation.
#    WITH_DOCS_DOXYGEN		- Enable doxygen
#    WITH_DOCS_BOOKS		- Enable build of books
#

option(WITH_DOCS "Build all documentation" On)
option(WITH_DOCS_UML "Enable UML diagrams generation" On)
option(WITH_DOCS_DOXYGEN "Enable doxygen generation" On)
option(WITH_DOCS_BOOKS "Enable build of books" On)

if (NOT WITH_DOCS)
	set(WITH_DOCS_UML Off)
	set(WITH_DOCS_DOXYGEN Off)
	set(WITH_DOCS_BOOKS Off)
endif ()

#
# Targets to build
# -------------------------------------------------------------------
#
# The following options are available:
#    WITH_LIBCLIENT	- Build the client library.
#    WITH_LIBSERVER	- Build the server library.
#

option(WITH_LIBCLIENT "Build libclient" On)
option(WITH_LIBSERVER "Build libserver" On)

if (WITH_BACKEND MATCHES "SDL")
	set(WITH_BACKEND_SDL TRUE)
else ()
	message(FATAL_ERROR "Unknown backend selected: ${WITH_BACKEND}")
endif ()

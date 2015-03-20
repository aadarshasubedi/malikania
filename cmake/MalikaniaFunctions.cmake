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

include(CMakeParseArguments)

# ---------------------------------------------------------
# The following macros are available:
#
# malikania_generate_book
# -----------------------
#
# malikania_generate_book(
#	name			The target name (book-${name})
#	output			The output file name
#	sources			The sources files, kept in order
# )
#
# Generate a book using pandoc.
#
# malikania_generate_uml
# ----------------------
#
# malikania_generate_uml(
#	NAME			The target name
#	DIRECTORY		Output directory (relative to docs_BINARY_DIR)
#	SOURCES			The sources files
# )
#
# Generate a target uml-${NAME} with all ${SOURCES} specified to be processed by plantuml.
#
# malikania_define_executable
# ---------------------------
#
# malikania_define_executable(
#	PROJECT			The project name
#	TARGET			The target name
#	SOURCES			The list of sources
#	FLAGS			(Optional) List of flags
#	INCLUDES		(Optional) List of include directories
#	LIBRARIES		(Optional) List of libraries
# )
#
# Create an executable. Be sure to quote SOURCES, if not only the first file will be passed.
# If you need flags, just pass them without -D or /D, this is automatically done for you.
#
# malikania_create_library
# ------------------------
#
# malikania_create_library(
#	PROJECT			The project name
#	TARGET			The target name
#	SOURCES			The sources
#	FLAGS			(Optional) List of flags
#	PRIVATE_INCLUDES	(Optional) List of includes only for building the library
#	PUBLIC_INCLUDES		(Optional) List of public includes to share with the library users
#	LIBRARIES		(Optional) List of libraries to link against
# )
#
# Create a shared library. Follow the same specification as malikania_define_executable.
# However, additional PRIVATE_INCLUDES and PUBLIC_INCLUDES are available.
#
# malikania_create_test
# ---------------------
#
# malikania_create_test(
#	NAME			Test name (must be lowercase)
#	SOURCES			Test sources files
#	LIBRARIES		(Optional) Libraries to link to
# )
#
# This will generate a target named test-<name> where name is the parameter NAME.
#
# setg
# ----
#
# setg(var value)
#
# Set a cache variable internal with name var.
#
# ---------------------------------------------------------

function(apply_flags target flags)
	# Add MALIKANIA_BUILD to enable declspec(dllexport) on Windows
	if (${flags})
		target_compile_definitions(
			${target}
			PRIVATE ${flags}
			PRIVATE "MALIKANIA_BUILD"
		)
	endif ()
endfunction()

function(apply_public_includes target var)
	if (${var})
		target_include_directories(${target} PUBLIC ${${var}})
	endif ()
endfunction()

function(apply_private_includes target var)
	if (${var})
		target_include_directories(${target} PRIVATE ${${var}})
	endif ()
endfunction()

function(apply_includes target var)
	if (${var})
		target_include_directories(${target} PRIVATE ${${var}})
	endif ()
endfunction()

function(apply_libraries target libs)
	if (${libs})
		target_link_libraries(${target} ${${libs}})
	endif ()
endfunction()

function(check_args prefix list)
	foreach (e ${list})
		if (NOT ${prefix}_${e})
			message(FATAL_ERROR "Please set `${e}' parameter")
		endif ()
	endforeach ()
endfunction()

function(set_label target name)
	set_target_properties(
		${target}
		PROPERTIES
		PROJECT_LABEL ${name}
	)
endfunction()

function(malikania_define_executable)
	set(singleArgs PROJECT TARGET)
	set(multiArgs SOURCES FLAGS INCLUDES LIBRARIES)
	set(mandatory PROJECT TARGET SOURCES)

	cmake_parse_arguments(EXE "" "${singleArgs}" "${multiArgs}" ${ARGN})
	check_args(EXE ${mandatory})

	project(${EXE_PROJECT})
	add_executable(${EXE_TARGET} ${EXE_SOURCES})

	apply_libraries(${EXE_TARGET} EXE_LIBRARIES)
	apply_includes(${EXE_TARGET} EXE_INCLUDES)
	apply_flags(${EXE_TARGET} EXE_FLAGS)
endfunction()

function(malikania_create_library)
	set(singleArgs PROJECT TARGET)
	set(multiArgs SOURCES FLAGS PRIVATE_INCLUDES PUBLIC_INCLUDES LIBRARIES)
	set(mandatory PROJECT TARGET SOURCES)

	cmake_parse_arguments(LIB "" "${singleArgs}" "${multiArgs}" ${ARGN})
	check_args(LIB ${mandatory})

	project(${LIB_PROJECT})
	add_library(${LIB_TARGET} SHARED ${LIB_SOURCES})
	set_label(${LIB_TARGET} ${LIB_PROJECT})

	# Remove lib suffix to avoid conflict with client and libclient targets
	set_target_properties(
		${LIB_TARGET}
		PROPERTIES
		PREFIX ""
	)

	apply_libraries(${LIB_TARGET} LIB_LIBRARIES)
	apply_private_includes(${LIB_TARGET} LIB_PRIVATE_INCLUDES)
	apply_public_includes(${LIB_TARGET} LIB_PUBLIC_INCLUDES)
	apply_flags(${LIB_TARGET} LIB_FLAGS)
endfunction()

function(malikania_generate_uml)
	set(options "")
	set(oneValueArgs NAME DIRECTORY)
	set(multiValueArgs SOURCES)

	cmake_parse_arguments(UML "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	foreach (f ${UML_SOURCES})
		get_filename_component(filename ${f} NAME_WE)

		list(APPEND outputs ${docs_BINARY_DIR}/uml/${filename}.png)

		add_custom_command(
			OUTPUT ${docs_BINARY_DIR}/uml/${filename}.png
			DEPENDS ${f}
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${docs_BINARY_DIR}/uml
			COMMAND
				${Java_JAVA_EXECUTABLE} -jar ${PLANTUML_JAR} ${f} -o ${docs_BINARY_DIR}/uml
			VERBATIM
		)
	endforeach ()

	add_custom_target(
		uml-${UML_NAME}
		DEPENDS ${outputs}
		SOURCES ${UML_SOURCES}
	)

	add_dependencies(uml uml-${UML_NAME})
endfunction()

function(malikania_create_test)
	set(singleArgs NAME)
	set(multiArgs LIBRARIES SOURCES)

	set(mandatory NAME SOURCES)

	cmake_parse_arguments(TEST "" "${singleArgs}" "${multiArgs}" ${ARGN})
	check_args(TEST ${mandatory})

	add_executable(test-${TEST_NAME} ${TEST_SOURCES})
	add_test(${TEST_NAME} test-${TEST_NAME})

	if (UNIX)
		list(APPEND TEST_LIBRARIES pthread)
	endif ()

	target_link_libraries(
		test-${TEST_NAME}
		${TEST_LIBRARIES}
		gtest
	)
endfunction()

function(malikania_generate_book name output sources)
	pandoc(
		TARGET book-${name}
		SOURCES ${sources}
		OUTPUT ${docs_BINARY_DIR}/books/${output}
		FROM markdown
		TO latex
		TOC
		STANDALONE
		MAKE_DIRECTORY
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)

	add_dependencies(books book-${name})
endfunction()

macro(setg var value)
	set(${var} ${value} CACHE INTERNAL "" FORCE)
endmacro()

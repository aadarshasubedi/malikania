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

#
# This order makes the book order, don't change it
#
set(
	FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Scope/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Overview/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Architecture/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Architecture/Technologies.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Gameplay/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Gameplay/Stats.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Bundle/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Bundle/Usage.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Bundle/Format.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/GameData/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/GameData/Animations.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/GameData/Sprites.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Network/Intro.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/Network/Commands/identify.txt"
)

malikania_generate_book(
	specifications
	Specifications.pdf
	"${FILES}"
)

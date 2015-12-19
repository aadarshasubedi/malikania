/*
 * main.cpp -- test Sprite
 *
 * Copyright (c) 2013, 2014, 2015 Malikania Authors
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <exception>

#include <gtest/gtest.h>

#include <malikania/Window.h>
#include <malikania/Sprite.h>

using namespace malikania;

TEST(Basic, createSpriteStandard)
{
	malikania::Window mainWindow;

	malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
		"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"cell\": [300, 300], \"size\": [1200, 900]}"
	).toObject());
}

TEST(Basic, createSpriteResourceNotFound)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/notfound.png\", \"alias\": \"testSprite\", \"cell\": [300, 300], \"size\": [1200, 900]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("failed to load texture: Couldn't open resources/images/notfound.png", e.what());
	}
}

TEST(Basic, createSpriteJSONNotObject)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"[\"It's\", \"not\", \"an\", \"object\"]"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: passed JSON is not an object", e.what());
	}
}

TEST(Basic, createSpriteMissingSize)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"cell\": [300, 300]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Error \"size\" of Sprite must be present, it's marked non required but for now Sprite need it", e.what());
	}
}

TEST(Basic, createSpriteMissingImage)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"alias\": \"testSprite\", \"cell\": [300, 300], \"size\": [1200, 900]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: it doesn't contains the \"image\" required key", e.what());
	}
}

TEST(Basic, createSpriteMissingAlias)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/mokodemo.png\", \"cell\": [300, 300], \"size\": [1200, 900]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: it doesn't contains \"alias\" required key", e.what());
	}
}

TEST(Basic, createSpriteMissingCell)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"size\": [1200, 900]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: it doesn't contains \"cell\" required key", e.what());
	}
}

TEST(Basic, createSpriteImageNotString)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": 42, \"alias\": \"testSprite\", \"size\": [1200, 900], \"cell\": [300, 300]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"image\" must be a valid String", e.what());
	}
}

TEST(Basic, createSpriteAliasNotString)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/mokodemo.png\", \"alias\": 42, \"size\": [1200, 900], \"cell\": [300, 300]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"alias\" must be a valid String", e.what());
	}
}

TEST(Basic, createSpriteCellNotArray)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"size\": [1200, 900], \"cell\": \"not an array\"}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"cell\" must be a valid Array", e.what());
	}
}

TEST(Basic, createSpriteCellValuesNotInteger)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"size\": [1200, 900], \"cell\": [\"Hello\", 300]}"
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"cell\" values have to be valid Integer", e.what());
	}
}

TEST(Basic, createSpriteSpaceNotArray)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			std::string("{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\"")
			+ std::string(", \"size\": [1200, 900], \"cell\": [300, 300], \"space\": \"not an array\"}")
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"space\" must be a valid Array", e.what());
	}
}

TEST(Basic, createSpriteSpaceValuesNotInteger)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			std::string("{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\"")
			+ std::string(", \"size\": [1200, 900], \"cell\": [300, 300], \"space\": [120, \"Hello\"]}")
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"space\" values have to be valid Integer", e.what());
	}
}

TEST(Basic, createSpriteSpaceValuesNot2)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			std::string("{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\"")
			+ std::string(", \"size\": [1200, 900], \"cell\": [300, 300], \"space\": [120, 120, 120]}")
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"space\" must have 2 values", e.what());
	}
}

TEST(Basic, createSpriteMarginNotArray)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			std::string("{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\"")
			+ std::string(", \"size\": [1200, 900], \"cell\": [300, 300], \"margin\": \"not an array\"}")
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"margin\" must be a valid Array", e.what());
	}
}

TEST(Basic, createSpriteMarginValuesNotInteger)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			std::string("{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\"")
			+ std::string(", \"size\": [1200, 900], \"cell\": [300, 300], \"margin\": [10, \"Hello\"]}")
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"margin\" values have to be valid Integer", e.what());
	}
}

TEST(Basic, createSpriteMarginValuesNot2)
{
	malikania::Window mainWindow;
	try {
		malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
			std::string("{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\"")
			+ std::string(", \"size\": [1200, 900], \"cell\": [300, 300], \"margin\": [10, 10, 10]}")
		).toObject());
		FAIL() << "Expected exception";
	} catch(std::runtime_error e) {
		ASSERT_STREQ("Couldn't parse JSON Sprite object: \"margin\" must have 2 values", e.what());
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

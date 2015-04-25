/*
 * main.cpp -- main client files
 *
 * Copyright (c) 2014 David Demelier <markand@malikania.fr>
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

#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <malikania/Json.h>
#include "Window.h"
#include "Size.h"
#include "Sprite.h"
#include "Image.h"

using namespace std::literals::chrono_literals;

// TODO delete this... just for fun
bool goRight = true;
bool goDown = true;
const int mokoSize = 300;

void bounce(malikania::Window& window, int &x, int &y) {
	malikania::Size resolution = window.getWindowResolution();
	int width = resolution.width();
	int height = resolution.height();
	if (y < 10) {
		goDown = true;
		y += 1;
	}
	if (goDown && y < height - mokoSize) {
		// Moko falls
		y += 0.2 * y;
	} else {
		// Moko will bounce!!!
		goDown = false;
	}
	if (!goDown && y > 0) {
		y -= 0.1 * y;
	} else {
		goDown = true;
	}

	if (goRight && x < width - mokoSize) {
		x += 4;
	} else {
		goRight = false;
	}
	if (!goRight && x > 0) {
		x -= 4;
	} else {
		goRight = true;
	}
}

// End TODO

int main(void)
{
	malikania::Window mainWindow;

	bool isBouncing = false;

	int mokoPositionX = 0;
	int mokoPositionY = 0;

	std::map<int, bool> keyPressed = { {SDLK_UP, false}, {SDLK_DOWN, false}, {SDLK_RIGHT, false}, {SDLK_LEFT, false} };

	mainWindow.setOnKeyDown([&mainWindow, &mokoPositionX, &mokoPositionY, &isBouncing, &keyPressed](int sdlKey) {
		switch (sdlKey) {
		case SDLK_ESCAPE:
			mainWindow.close();
			break;
		case SDLK_UP:
			keyPressed[SDLK_UP] = true;
			break;
		case SDLK_DOWN:
			keyPressed[SDLK_DOWN] = true;
			break;
		case SDLK_RIGHT:
			keyPressed[SDLK_RIGHT] = true;
			break;
		case SDLK_LEFT:
			keyPressed[SDLK_LEFT] = true;
			break;
		case SDLK_m:
			isBouncing = !isBouncing;
			break;
		}
	});

	mainWindow.setOnKeyUp([&keyPressed](int sdlKey) {
		switch (sdlKey) {
		case SDLK_UP:
			keyPressed[SDLK_UP] = false;
			break;
		case SDLK_DOWN:
			keyPressed[SDLK_DOWN] = false;
			break;
		case SDLK_RIGHT:
			keyPressed[SDLK_RIGHT] = false;
			break;
		case SDLK_LEFT:
			keyPressed[SDLK_LEFT] = false;
			break;
		}
	});

	int animationStep = 1;
	mainWindow.setOnRefresh([&mainWindow, &keyPressed, &animationStep](){
		if (keyPressed[SDLK_LEFT]) {
			std::string animationState = "left" + std::to_string(animationStep > 4 ? 4 : animationStep++);
		} else if (keyPressed[SDLK_RIGHT]) {
			std::string animationState = "right" + std::to_string(animationStep > 4 ? 4 : animationStep++);
		} else if (keyPressed[SDLK_DOWN]) {
			std::string animationState = "down" + std::to_string(animationStep > 4 ? 4 : animationStep++);
		} else {
			animationStep = 1;
		}
	});

	malikania::Size spriteSize(1200, 900);
	malikania::Size cellSize(300, 300);
	std::map<std::string, malikania::Position> mokoCellMap;
	mokoCellMap.emplace("default", malikania::Position(0, 600));
	mokoCellMap.emplace("left1", malikania::Position(0, 0));
	mokoCellMap.emplace("left2", malikania::Position(300, 0));
	mokoCellMap.emplace("left3", malikania::Position(600, 0));
	mokoCellMap.emplace("left4", malikania::Position(900, 0));
	mokoCellMap.emplace("right1", malikania::Position(0, 300));
	mokoCellMap.emplace("right2", malikania::Position(300, 300));
	mokoCellMap.emplace("right3", malikania::Position(600, 300));
	mokoCellMap.emplace("right4", malikania::Position(900, 300));
	mokoCellMap.emplace("down1", malikania::Position(0, 600));
	mokoCellMap.emplace("down2", malikania::Position(300, 600));
	mokoCellMap.emplace("down3", malikania::Position(600, 600));
	mokoCellMap.emplace("down4", malikania::Position(900, 600));

	malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
		"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"cell\": [300, 300], \"size\": [1200, 900]}"
	).toObject());

	while (mainWindow.isOpen()) {

		// TODO delete this, just for fun...
		if (isBouncing) {
			bounce(mainWindow, mokoPositionX, mokoPositionY);
		}

		mainWindow.processEvent();
		mainWindow.clear();
		mainWindow.update();

		testSprite.draw(mainWindow, 0, malikania::Rectangle(0, 0, 300, 300));
		testSprite.draw(mainWindow, 1, malikania::Rectangle(200, 200, 300, 300));
		testSprite.draw(mainWindow, 2, malikania::Rectangle(400, 400, 300, 300));
		testSprite.draw(mainWindow, 11, malikania::Rectangle(600, 400, 300, 300));

		mainWindow.present();

		std::this_thread::sleep_for(5ms);
	}

	//malikania::Window::quit();

	return 0;
}

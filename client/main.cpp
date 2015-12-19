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
#include <malikania/Window.h>
#include <malikania/Size.h>
#include <malikania/Sprite.h>
#include <malikania/Image.h>
#include <malikania/Point.h>

using namespace std::literals::chrono_literals;

// TODO delete this... just for fun
bool goRight = true;
bool goDown = true;
const int mokoSize = 300;

void bounce(malikania::Window& window, int &x, int &y) {
	malikania::Size resolution = window.getWindowResolution();
	int width = resolution.width;
	int height = resolution.height;
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

	malikania::Sprite testSprite = malikania::Sprite::fromJson(mainWindow, malikania::JsonDocument(
		"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"cell\": [300, 300], \"size\": [1200, 900]}"
	).toObject());

	while (mainWindow.isOpen()) {

		// TODO delete this, just for fun...
		if (isBouncing) {
			bounce(mainWindow, mokoPositionX, mokoPositionY);
		}

		mainWindow.processEvent();
		mainWindow.setDrawingColor({255, 255, 255, 255});
		mainWindow.clear();
		mainWindow.update();

		testSprite.draw(mainWindow, 0, {0, 0, 300, 300});
		testSprite.draw(mainWindow, 1, {200, 200, 300, 300});
		testSprite.draw(mainWindow, 2, {400, 400, 300, 300});
		testSprite.draw(mainWindow, 11, {600, 400, 300, 300});

		Color c{255, 50, 40, 255};
		mainWindow.setDrawingColor(c);
		mainWindow.drawLine({0, 0, 300, 300});

		std::vector<malikania::Point> points{{20, 20}, {30, 50}, {100, 200}, {30, 60}, {20, 300}, {100, 20}};
		mainWindow.drawLines(points);

		mainWindow.setDrawingColor({200, 50, 200, 255});
		mainWindow.drawPoint({400, 400});
		mainWindow.drawPoint({400, 402});
		mainWindow.drawPoint({400, 405});
		mainWindow.drawPoint({400, 407});
		mainWindow.drawPoint({400, 410});

		mainWindow.setDrawingColor({0, 0, 0, 255});
		mainWindow.drawPoints(points);

		mainWindow.setDrawingColor({30, 30, 30, 255});
		mainWindow.drawRectangle({500, 500, 200, 100});

		mainWindow.setDrawingColor({130, 30, 30, 255});
		mainWindow.drawRectangles({{800, 800, 200, 100}, {700, 700, 200, 100}, {750, 750, 200, 100}});

		mainWindow.drawRectangle({600, 200, 200, 100}, true, {0, 255, 0, 255});

		mainWindow.drawRectangles(
			{{800, 400, 200, 100}, {700, 450, 200, 100}, {750, 500, 200, 100}},
			true,
			{{255,0,0,255},{0,255,0,255},{0,0,255,255}}
		);

		malikania::Font font("resources/fonts/DejaVuSans.ttf", 42);
		mainWindow.drawText("Malikania FTW!", font, malikania::Point{400, 100});

		mainWindow.present();

		std::this_thread::sleep_for(5ms);
	}

	//malikania::Window::quit();

	return 0;
}

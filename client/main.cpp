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
#include "Window.h"

using namespace std::literals::chrono_literals;

// TODO delete this... just for fun
bool bouncing = false;
bool goRight = true;

void bounce(malikania::Window& window, int &x, int &y) {

	if (!bouncing && y < 5) {
		y += 1;
	} else if (!bouncing && y < 300) {
		// Moko falls
		y += 0.2 * y;
	} else if (!bouncing && y > 300) {
		// Moko will bounce :-)
		bouncing = true;
	} else if (bouncing && y < 5) {
		// Moko stop bouncing
		bouncing = false;
	} else {
		// Moko Bounce!!!
		y -= 0.2 * y;
	}
	if (goRight && x < 500) {
		x += 4;
	} else {
		goRight = false;
	}
	if (!goRight && x > 0) {
		x -= 4;
	} else {
		goRight = true;
	}
	window.updateImagePosition("moko", x, y);
}

// End TODO

int main(void)
{
	malikania::Window mainWindow;

	bool isBouncing = false;

	int mokoPositionX = 0;
	int mokoPositionY = 0;

	mainWindow.onKeyUp([&mainWindow, &mokoPositionX, &mokoPositionY, &isBouncing](int sdlKey) {
		switch (sdlKey) {
		case SDLK_ESCAPE:
			mainWindow.close();
			break;
		case SDLK_UP:
			mainWindow.updateImagePosition("moko", mokoPositionX, --mokoPositionY);
			break;
		case SDLK_DOWN:
			mainWindow.updateImagePosition("moko", mokoPositionX, ++mokoPositionY);
			break;
		case SDLK_RIGHT:
			mainWindow.updateImagePosition("moko", ++mokoPositionX, mokoPositionY);
			break;
		case SDLK_LEFT:
			mainWindow.updateImagePosition("moko", --mokoPositionX, mokoPositionY);
			break;
		case SDLK_m:
			isBouncing = !isBouncing;
		}
	});

	mainWindow.addImage("moko", "resources/images/mokodemo.png");

	while (mainWindow.isOpen()) {

		// TODO delete this, just for fun...
		if (isBouncing) {
			bounce(mainWindow, mokoPositionX, mokoPositionY);
		}

		mainWindow.processEvent();
		mainWindow.clear();
		mainWindow.update();
		mainWindow.draw();

		std::this_thread::sleep_for(5ms);
	}

	malikania::Window::quit();

	return 0;
}

/*
 * Animator.h -- animation drawing object
 *
 * Copyright (c) 2013-2016 Malikania Authors
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

#ifndef _MALIKANIA_ANIMATOR_H_
#define _MALIKANIA_ANIMATOR_H_

/**
 * @file Animator.h
 * @brief Draw animations.
 */

#include <malikania/ElapsedTimer.h>

#include "CommonClient.h"

namespace malikania {

class Animation;
class Position;
class Window;

/**
 * @class Animator
 * @brief Object for drawing animations.
 *
 * The animator contains an animation and a state.
 */
class MALIKANIA_CLIENT_EXPORT Animator {
private:
	std::shared_ptr<Animation> m_animation;
	ElapsedTimer m_timer;
	unsigned m_current{0};

public:
	/**
	 * Construct an animator.
	 *
	 * @pre animation must not be null
	 * @param animation the animation
	 */
	Animator(std::shared_ptr<Animation> animation) noexcept;

	/**
	 * Update the animator state.
	 *
	 * This function should be called in the main loop to update the cell to draw before calling draw().
	 */
	void update() noexcept;

	/**
	 * Draw the animation.
	 *
	 * @param window the window
	 * @param position the position in the window
	 */
	void draw(Window &window, const Point &position);
};

} // !malikania

#endif // !_MALIKANIA_ANIMATOR_H_

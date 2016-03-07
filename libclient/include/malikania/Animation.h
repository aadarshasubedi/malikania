/*
 * Animation.h -- animation description
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

#ifndef _MALIKANIA_ANIMATION_H_
#define _MALIKANIA_ANIMATION_H_

/**
 * @file Animation.h
 * @brief Describe an animation.
 */

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "Sprite.h"

namespace malikania {

class Window;

/**
 * @class AnimationFrame
 * @brief Animation frame description.
 *
 * A frame is a duration before switching to the next sprite cell. It is currently implemented as a class for future
 * usage.
 */
class AnimationFrame {
private:
	std::uint16_t m_delay;

public:
	/**
	 * Construct a frame.
	 *
	 * @param delay the optional delay
	 */
	inline AnimationFrame(std::uint16_t delay = 100) noexcept
		: m_delay(delay)
	{
	}

	/**
	 * Get the the delay.
	 *
	 * @return the delay
	 */
	inline std::uint16_t delay() const noexcept
	{
		return m_delay;
	}
};

/**
 * @class Animation
 * @brief Animation description.
 *
 * An animation is a sprite with a set of frames containing a delay for showing all sprites in a specific amount of
 * time.
 *
 * Because an animation contains an image, a state (time, current cell) it must be constructed with an Animator object
 * so the user is able to use the same animation on different parts of the screen without having to duplicate
 * resources.
 *
 * @see Animator
 */
class Animation {
private:
	std::shared_ptr<Sprite> m_sprite;
	std::vector<AnimationFrame> m_frames;

public:
	/**
	 * Create an animation.
	 *
	 * @pre sprite must not be null
	 * @param sprite the sprite image
	 * @param frames the frames to show
	 */
	Animation(std::shared_ptr<Sprite> sprite, std::vector<AnimationFrame> frames) noexcept;

	/**
	 * Get the underlying sprite.
	 *
	 * @return the sprite
	 */
	inline const std::shared_ptr<Sprite> &sprite() const noexcept
	{
		return m_sprite;
	}

	/**
	 * Get the frames.
	 *
	 * @return the frames
	 */
	inline const std::vector<AnimationFrame> &frames() const noexcept
	{
		return m_frames;
	}

	/**
	 * Access a frame.
	 *
	 * @pre index < number of frames
	 * @param index the index
	 * @return the frame
	 */
	inline const AnimationFrame &operator[](unsigned index) const noexcept
	{
		assert(index < m_frames.size());

		return m_frames[index];
	}
};

} // !malikania

#endif // !_MALIKANIA_ANIMATION_H_

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 150

class Animation
{
public:
	bool loop = true;
	bool pingpong = false;
	float speed = 5.0f;
	SDL_Rect frames[MAX_FRAMES];


private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

	enum pingpong
	{
		forward,
		backward
	} direction = forward;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		current_frame += speed*dt;

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
	}
};

#endif



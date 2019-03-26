#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SDL/include/SDL_rect.h"
#include "p2List.h"
#include "j1Module.h"


#define MAX_FRAMES 200

class Animation
{

public:

	bool loop;
	bool pingpong;
	float speed;
	SDL_Rect frames[MAX_FRAMES];

private:

	float current_frame;
	int last_frame;
	int loops;


public:

	Animation() {}

	Animation(const Animation& anim) :loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		current_frame += speed * dt;

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

	bool Awake(pugi::xml_node&)
	{
		
	}

	void LoadAnimation(const char* file_path, const char* animation_name)
	{
		Animation* animation = new Animation();

		bool anim = false;

		pugi::xml_document anim_doc;
		pugi::xml_parse_result result = anim_doc.load_file(file_path);


		if (result == NULL)
		{
			LOG("Issue loading animation");
		}

		pugi::xml_node objgroup;
		for (objgroup = anim_doc.child("map").child("objectgroup"); objgroup; objgroup = objgroup.next_sibling("objectgroup"))
		{
			p2SString name = objgroup.attribute("name").as_string();
			if (name == animation_name)
			{
				anim = true;
				int x, y, h, w;

				for (pugi::xml_node sprite = objgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
				{
					x = sprite.attribute("x").as_int();
					y = sprite.attribute("y").as_int();
					w = sprite.attribute("width").as_int();
					h = sprite.attribute("height").as_int();

					animation->PushBack({ x, y, w, h });
				}

			}
		}
		if (anim = true)
			return animation;
		else
			return nullptr;

	}
};

#endif
#ifndef __UI_MARKER_H__
#define __UI_MARKER_H__

#include "UI_element.h"
#include <iterator>

struct SDL_Rect;

enum class Marker_anim
{
	LEFT_TO_RIGHT = -1,
	NONE = 0,
	RIGHT_TO_LEFT = 1
};

struct marker_custom_button_rects
{
	SDL_Rect custom_unlocked;
	SDL_Rect custom_locked;
};

struct Marker_anim_data
{
	int speed;
	int max_movement_distance;
	Marker_anim anim;

	Marker_anim_data()
	{
		speed = 60;
		max_movement_distance = 30;
		anim = Marker_anim::LEFT_TO_RIGHT;
	}
};

class Marker : public UI_element
{
public:

	Marker()
	{}

	Marker(SDL_Texture* texture, int x, int y, iPoint margin, std::list<UI_element*>::iterator* target, SDL_Rect section, j1Module* callback, bool is_support_marker, marker_custom_button_rects rects) :
		UI_element(x, y, element_type::MARKER, section, callback, texture), margin(margin), target(target),callback(callback), original_section(section), rects(rects), is_support_marker(is_support_marker)
	{
		is_valid_iterator = true;
		movement_timer.Start();
	}

	~Marker() {}

	void BlitElement();
	void SetTarget(std::list<UI_element*>::iterator* target);

public:
	std::list<UI_element*>::iterator* target;
	iPoint margin = {0,0};

	j1Module* callback;
	j1Timer movement_timer;
	int speed = 60;
	int distance_moved = 0;
	int max_movement_distance = 30;

protected:
	bool is_valid_iterator = false;
	bool is_support_marker = false;
	SDL_Rect original_section;

	Marker_anim_data anim_data;
	marker_custom_button_rects rects;
};


#endif

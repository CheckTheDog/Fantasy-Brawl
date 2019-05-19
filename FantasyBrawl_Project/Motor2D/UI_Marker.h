#ifndef __UI_MARKER_H__
#define __UI_MARKER_H__

#include "UI_element.h"
#include <iterator>

struct SDL_Rect;

class Marker : public UI_element
{
public:

	Marker()
	{}

	Marker(SDL_Texture* texture, int x, int y, iPoint margin, std::list<UI_element*>::iterator* target, SDL_Rect section, j1Module* callback) :
		UI_element(x, y, element_type::MARKER, section, callback, texture), margin(margin), target(target)
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

	j1Timer movement_timer;
	int speed = 60;
	int distance_moved = 0;
	int max_movement_distance = 30;

protected:
	bool is_valid_iterator = false;

};


#endif

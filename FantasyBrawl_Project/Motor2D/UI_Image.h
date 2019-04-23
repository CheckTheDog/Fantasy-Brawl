#ifndef __UI_IMAGE__
#define __UI_IMAGE__
#include "UI_element.h"

struct SDL_Rect;

class Image : public UI_element
{
public:

	Image()
	{}

	Image(SDL_Texture* texture, int x, int y, SDL_Rect section, j1Module* callback) : UI_element(x, y, element_type::IMAGE, section, callback, texture)
	{
		tmp_section.x = section.x;
		tmp_section.y = section.y;
		tmp_section.h = section.h;
		tmp_section.w = section.w;
	}

	~Image()
	{}

	void BlitElement();


	float multiplier;
	int player_hp = 300;
	SDL_Rect tmp_section = {0,0,0,0};
};

#endif 
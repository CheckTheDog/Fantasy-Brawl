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
	{}

	~Image()
	{}

	void BlitElement();
};

#endif 
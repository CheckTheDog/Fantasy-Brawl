#ifndef __UI_WINDOW__
#define __UI_WINDOW__

#include "UI_element.h"

class Window : public UI_element
{
public:

	Window()
	{}

	Window(SDL_Texture* texture, int x, int y, SDL_Rect section, j1Module* callback) : UI_element(x, y, element_type::WINDOW, section, callback, texture)
	{}

	~Window()
	{}

	void appendChild(int x, int y, UI_element* child);
	void appendChildAtCenter(UI_element* child);
	void BlitElement();

public:

	std::list <UI_element*>::iterator focus;
	std::list <UI_element*> content;
};

#endif // !__UI_WINDOW__


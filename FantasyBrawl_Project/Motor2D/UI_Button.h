#ifndef __UI_BUTTON__
#define __UI_BUTTON 

#include "UI_element.h"
#include "UI_Text.h"

class Button : public UI_element
{
public:

	Button()
	{}

	Button(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback) : UI_element(x, y, element_type::BUTTON, standby, callback, texture),
		OnMouse(OnMouse),
		OnClick(OnClick)
	{}
	Button(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect standbyActive, SDL_Rect OnMouseActive, j1Module* callback) : UI_element(x, y, element_type::SWITCH, standby, callback, texture),
		OnMouse(OnMouse),
		OnClick(OnMouse),
		sectionActive(standbyActive),
		OnMouseActive(OnMouseActive)
	{}

	~Button()
	{}

	void setOutlined(bool isOutlined);
	void appendChild(int x, int y, UI_element* child);
	void appendChildAtCenter(UI_element* child);
	void BlitElement();

public:

	Text * text = nullptr;
	SDL_Rect OnMouse;
	SDL_Rect sectionActive;
	SDL_Rect OnMouseActive;
	SDL_Rect OnClick;
	bool active = false;
	bool clicked = false;
};

#endif 


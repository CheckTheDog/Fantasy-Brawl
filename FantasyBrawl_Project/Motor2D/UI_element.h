#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Module.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Input.h"

struct SDL_Texture;

enum element_type
{
	TEXT,
	IMAGE,
	BUTTON,
	SWITCH,
	SLIDER,
	WINDOW,
	CLOCK,
	PLAYERINFO,
	FINALINFO,
	MARKER
};

enum element_function
{
	NONE,
	NEW_GAME,
	INGAME,
	INGAME_NEW_GAME,
	Last_Button_1,
	Last_Button_2,
	Last_Button_3,
	Last_Button_4,
	SELECTING1,
	SELECTING2,
	SELECTING3,
	SELECTING4,
	INV_SELECTING1,
	INV_SELECTING2,
	INV_SELECTING3,
	INV_SELECTING4,
	CONTINUE,
	SETTINGS,
	CREDITS,
	EXIT,
	PAUSE,
	RESTART,
	SCORES,
	RESTORE,
	BACK,
	CANCEL,
	APPLY,
	HOME,
	WEBPAGE
};

enum element_state
{
	STANDBY,
	MOUSEOVER,
	CLICKED
};

class UI_element
{
public:

	UI_element()
	{}

	UI_element(int x, int y, element_type type, SDL_Rect section, j1Module* callback, SDL_Texture* texture = nullptr) : localPosition({ x, y }), Original_Pos({ x, y }), element_type(type), section(section), callback(callback), texture(texture)
	{}

	virtual ~UI_element()
	{
		if (texture != nullptr && texture != App->gui->GetAtlas())
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}

	virtual void setOutlined(bool isOutlined)
	{}

	iPoint calculateAbsolutePosition() const
	{
		if (parent != nullptr)
			return (localPosition + parent->calculateAbsolutePosition());
		else
			return localPosition;
	}

	virtual void appendChild(int x, int y, UI_element* child)
	{}

	virtual void appendChildAtCenter(UI_element* child)
	{}

	virtual void BlitElement()
	{}

	void setDragable(bool horizontally, bool vertically)
	{
		if (horizontally || vertically)
			dragable = true;
		else
			dragable = false;
		horizontalMovement = horizontally;
		verticalMovement = vertically;
	}

	void setOriginalPos(int x, int y)
	{
		Original_Pos = { x, y };
	}
	//-1 to delete limit
	void setLimits(int right_limit, int left_limit, int top_limit, int bottom_limit)
	{
		this->right_limit = right_limit;
		this->left_limit = left_limit;
		this->top_limit = top_limit;
		this->bottom_limit = bottom_limit;
	}
	void Mouse_Drag()
	{
		iPoint Mouse_Movement;
		App->input->GetMousePosition(Mouse_Movement.x, Mouse_Movement.y);
		if (horizontalMovement)
		{
			localPosition.x += (Mouse_Movement.x - Click_Pos.x);
			Click_Pos.x += (Mouse_Movement.x - Click_Pos.x);
		}
		if (verticalMovement)
		{
			localPosition.y += (Mouse_Movement.y - Click_Pos.y);
			Click_Pos.y += (Mouse_Movement.y - Click_Pos.y);
		}
		if (left_limit >= 0 && localPosition.x < Original_Pos.x - left_limit)
			localPosition.x = Original_Pos.x - left_limit;
		else if (right_limit >= 0 && localPosition.x > Original_Pos.x + right_limit)
			localPosition.x = Original_Pos.x + right_limit;
		if (top_limit >= 0 && localPosition.y < Original_Pos.y - top_limit)
			localPosition.y = Original_Pos.y - top_limit;
		else if (bottom_limit >= 0 && localPosition.y > Original_Pos.y + bottom_limit)
			localPosition.y = Original_Pos.y + bottom_limit;

	}
	void Start_Drag()
	{
		iPoint Mouse_Movement;
		App->input->GetMousePosition(Mouse_Movement.x, Mouse_Movement.y);
		Click_Pos = Mouse_Movement;
		moving = true;
	}
	void End_Drag()
	{
		Click_Pos = { 0,0 };

		moving = false;
	}

public:

	SDL_Texture * texture = nullptr;
	iPoint localPosition;
	SDL_Rect section;
	element_type element_type;
	element_state state = STANDBY;
	element_function function = NONE;
	j1Module* callback = nullptr;
	UI_element* parent = nullptr;
	std::list<UI_element*> children;
	bool hovering = false;
	bool moving = false;
	bool dragable = false;
	bool solid = true;

protected:
	iPoint Click_Pos{ 0,0 };
	iPoint Original_Pos{ 0, 0 };
	bool verticalMovement = false;
	bool horizontalMovement = false;
	int right_limit = -1;
	int left_limit = -1;
	int top_limit = -1;
	int bottom_limit = -1;
};

#endif // !__UI_ELEMENT__

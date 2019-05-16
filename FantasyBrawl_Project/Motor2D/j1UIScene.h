#ifndef __j1UISCENE_H__
#define __j1UISCENE_H__

#include "j1Module.h"
#include <list>

//Fix later
#include "j1Input.h"


class UI_element;
class Clock;
class Button;
class j1Player;
struct _TTF_Font;


enum menu_id
{
	START_MENU,
	SETTINGS_MENU,
	INGAME_MENU,
	SELECTION_MENU,
	CREDITS_MENU,
	FINAL_MENU
};
struct menu
{
	menu(menu_id id) : id(id)
	{}
	std::list <UI_element*> elements;
	menu_id id;

	//Gamepad traversal
	std::list<UI_element*> gamepad_tabs[4];
	UI_element* gamepads_focus[4];

};

struct settings_values
{
	bool fullscreen = false;
	float music = 0.5f;
	float fx = 0.5f;
};


class j1UIScene : public j1Module
{
public:
	j1UIScene();
	~j1UIScene();
	// Called before render is available
	bool Awake();
	// Called before the first frame
	bool Start();
	// Called before all Updates
	bool PreUpdate();
	// Called each loop iteration
	bool Update(float dt);
	// Called before all Updates
	bool PostUpdate(float dt);
	bool OnUIEvent(UI_element* element, event_type event_type);
	// Called before quitting
	bool CleanUp();

	bool loadMenu(menu_id id);


	void applySettings(settings_values values);

	void pauseClock();
	void playClock();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void CreateScoreBoard(int num);

	void AddControllerSupport(UI_element* element, PLAYER gamepad_supported, menu_id id_menu);
	

public:
	std::list <menu*> menus;
	menu* current_menu = nullptr;
	settings_values newValues;
	settings_values startValues;
	settings_values defaultValues;
	Clock* clock = nullptr;
	Button* continueButton = nullptr;
	menu_id previous_menu;
	menu_id actual_menu = START_MENU;
	bool ui_id1 = false;
	bool ui_id2 = false;

	//PLAYER HP BARS
	UI_element* hp_bar1 = nullptr;
	UI_element* hp_bar2 = nullptr;
	UI_element* hp_bar3 = nullptr;
	UI_element* hp_bar4 = nullptr;

	//PLAYER SP BARS
	UI_element* sp_bar1 = nullptr;
	UI_element* sp_bar2 = nullptr;
	UI_element* sp_bar3 = nullptr;
	UI_element* sp_bar4 = nullptr;

	//PLAYER SHIELD BARS
	UI_element* shield_bar1 = nullptr;
	UI_element* shield_bar2 = nullptr;
	UI_element* shield_bar3 = nullptr;
	UI_element* shield_bar4 = nullptr;

	//PLAYER HP CAPSULES
	UI_element* hp_capsule1 = nullptr;
	UI_element* hp_capsule2 = nullptr;
	UI_element* hp_capsule3 = nullptr;
	UI_element* hp_capsule4 = nullptr;

	//PLAYER SP CAPSULES
	UI_element* sp_capsule1 = nullptr;
	UI_element* sp_capsule2 = nullptr;
	UI_element* sp_capsule3 = nullptr;
	UI_element* sp_capsule4 = nullptr;

	//PLAYER SHIELD CAPSULES
	UI_element* shield_capsule1 = nullptr;
	UI_element* shield_capsule2 = nullptr;
	UI_element* shield_capsule3 = nullptr;
	UI_element* shield_capsule4 = nullptr;

	//PLAYER MARKS(?)
	UI_element* mark1 = nullptr;
	UI_element* mark2 = nullptr;
	UI_element* mark3 = nullptr;
	UI_element* mark4 = nullptr;

	//READY
	UI_element* ready = nullptr;
	UI_element* ready_text = nullptr;

	//SELECTION BOOLS
	int counter = 0;
	bool player1_select = false;
	bool player2_select = false;
	bool player3_select = false;
	bool player4_select = false;

	//PLAYER FOR SCOREBOARD
	j1Player* player_winner = nullptr;
	menu* finalMenu = nullptr;
	UI_element* win_text = nullptr;
	bool scoreboard = false;


	//FONTS
	_TTF_Font* small_texts_font = nullptr;
	_TTF_Font* big_buttons_font = nullptr;
	_TTF_Font* mid_buttons_font = nullptr;

	_TTF_Font* small_font = nullptr;
	_TTF_Font* big_font = nullptr;
	_TTF_Font* mid_font = nullptr;

	float scale = 0.66f;

	//Menu
	menu* ingameMenu;
	
	//timer
	UI_element* timer;
};
#endif // !__j1UISCENE_H__ 

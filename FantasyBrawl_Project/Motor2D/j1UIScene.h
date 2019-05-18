#ifndef __j1UISCENE_H__
#define __j1UISCENE_H__

#include "j1Module.h"
#include <list>



class UI_element;
class Clock;
class Button;
class j1Player;
struct _TTF_Font;
struct SDL_Texture;
class Slider;

enum menu_id
{
	START_MENU,
	SETTINGS_MENU,
	INGAME_MENU,
	SELECTION_MENU,
	CREDITS_MENU,
	FINAL_MENU,
	INGAMESETTINGS_MENU
};
struct menu
{
	menu(menu_id id) : id(id)
	{}
	std::list <UI_element*> elements;
	menu_id id;

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

	UI_element* photos[4];
	bool passing1 = false;
	bool passing2 = false;
	bool passing3 = false;
	bool passing4 = false;

	//READY
	UI_element* ready = nullptr;
	UI_element* ready_text = nullptr;

	//SELECTION BOOLS
	int counter1 = 1;
	int counter2 = 1;
	int counter3 = 1;
	int counter4 = 1;
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

	float scale = 0.66f;

	SDL_Texture* panel = nullptr;
	SDL_Texture* margin_tex = nullptr;
	UI_element* margin = nullptr;
	SDL_Texture* champselect_bg = nullptr;
	UI_element* selection_image = nullptr;

	// --- Slider pointers ---
	Slider* music_slider = nullptr;
	Slider* fx_slider = nullptr;
	Slider* music_sliderMM = nullptr;
	Slider* fx_sliderMM = nullptr;
	
};
#endif // !__j1UISCENE_H__ 

#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include <list>

struct _TTF_Font;
struct SDL_Texture;
struct marker_custom_button_rects;
struct Marker_anim_data;
struct SDL_Color;
class UI_element;
class Text;
class Image;
class Button;
class Window;
class Slider;
class Clock;
class Marker;
enum class fade_step;


enum event_type
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_RIGHT_CLICK,
	MOUSE_RIGHT_RELEASE,
	MOUSE_LEFT_CLICK,
	MOUSE_LEFT_RELEASE,
	TIMER_ZERO,
	STOPWATCH_ALARM,
	BUTTON_ENTER,
	BUTTON_ANY,
	TRIGGER_ANY,
	MARKER_OVER_CUSTOMIZER,
	MARKER_OVER_LOCKED_ELEMENT
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	friend class j1UIScene;

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void UIDebugDraw();
	// Gui creation functions
	const SDL_Texture* GetAtlas() const;
	Text* createText(const char* text, int x, int y, _TTF_Font* font, SDL_Color color = { 255, 255, 255, 255 }, j1Module* callback = nullptr);
	Clock* createTimer(int x, int y, int initial_value, _TTF_Font* font, SDL_Color color, j1Module* callback = nullptr);
	Clock* createStopWatch(int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback = nullptr);
	Image* createImage(int x, int y, SDL_Texture* texture, j1Module* callback = nullptr);
	Image* createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Button* createButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback = nullptr);
	Button* createInteractiveButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, uint id, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Button* createSwitch(int x, int y, SDL_Texture* texture, SDL_Rect standbyUnactive, SDL_Rect OnMouseUnactive, SDL_Rect standbyActive, SDL_Rect OnMouseActive, j1Module* callback = nullptr);

	//NULL texture to use atlas
	Window* createWindow(int x, int y, SDL_Texture* texture, SDL_Rect section, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Slider* createSlider(int x, int y, SDL_Texture* texture, SDL_Rect empty, SDL_Rect full, Button* button, _TTF_Font* text_font, SDL_Color text_color, float default_progress = 0.5f, j1Module* callback = nullptr, char* text = "");

	Marker* createMarker(int x, int y, iPoint margin, std::list<UI_element*>::iterator* target, SDL_Texture* texture, SDL_Rect rect, j1Module* callback, marker_custom_button_rects rects, Marker_anim_data anim_data, bool is_support_marker = false);
	bool ManageAutomaticTraverseTiming(int player_timer, float time_to_start = 1.5f, float time_margin = 0.1f);

	//For keybinding UI purposes
	SDL_Rect GetButtonRect(int button) { return { 0,button * 72, 72,72 }; }
public:
	bool UI_Debug = false;
	float UI_scale;
	int alpha_value = 255;
	uint button_click_fx = 0;

private:
	j1Timer time_since_press[4];
	j1Timer automatic_traverse_margin[4];

	SDL_Texture * atlas;
	std::string atlas_file_name;
	std::list <UI_element*> UI_elements;
	UI_element* draggingElement = nullptr;
	UI_element* last_mouse_focus = nullptr;
	UI_element* gamepad_last_focus[4] = { nullptr };

	// --- Timer Fade ---
	fade_step current_step;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Color colorT;
	float alphaT = 0;
};

#endif // __j1GUI_H__

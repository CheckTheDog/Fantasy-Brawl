#ifndef __UI_CLOCK__
#define __UI_CLOCK__

#include "UI_element.h"
#include "UI_Text.h"
#include "SDL/include/SDL_pixels.h"
#include "j1Timer.h"
#include <vector>

struct _TTF_Font;

enum chrono_type
{
	STOPWATCH,
	TIMER
};

class Clock : public UI_element
{
public:

	Clock()
	{}

	Clock(int x, int y, chrono_type type, _TTF_Font* font, SDL_Color color, j1Module* callback) : UI_element(x, y, element_type::CLOCK, { 0, 0, 0, 0 }, callback, nullptr),
		type(type)
	{
		text = new Text("0000", x, y, font, color, nullptr);
		/*counter.Pause();*/
	}

	~Clock()
	{
		delete text;
	}

	void BlitElement();
	void setStartValue(int new_start_value);
	void setAlarm(int alarm);
	void restartChrono();


public:
	int time = 1;
	uint time_elapsed = 0;
	uint start_value = 0;
	std::vector <uint> alarms;
	j1Timer counter;
	Text* text;
	chrono_type type;
	std::string last_secs;

};

#endif

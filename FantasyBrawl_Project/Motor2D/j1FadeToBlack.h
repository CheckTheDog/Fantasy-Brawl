#ifndef __J1FADETOBLACK_H__
#define __J1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

enum class fade_step
{
	none,
	fade_to_black,
	fade_from_black
};

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(SDL_Rect screen_rect, Uint32 &start_time, Uint32 &total_time, fade_step &current_step);
	bool FadeCustom(int r, int g, int b, float a, float time, Uint32 &start_time, Uint32 &total_time, fade_step &current_step);

private:

	//Colors and Alpha values
	int r, g, b;
	float a;
};

#endif //__MODULEFADETOBLACK_H__

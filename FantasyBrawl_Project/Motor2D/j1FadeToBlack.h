#ifndef __J1FADETOBLACK_H__
#define __J1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"
#define MAX_FADE_TEXTURES  1

struct SDL_Texture;

enum class FADE_TEX
{
	NONE = -1,
	PLAYER_DEATH = 0,
	MAX_TEXS
};

static_assert (MAX_FADE_TEXTURES == (int)FADE_TEX::MAX_TEXS, "MAX_TEXS from FADE_TEX enum class, is not equal to MAX_FADE_TEXTURES (ASK Didac if you don't understand what happened);");

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
	bool FadeCustom(int r, int g, int b, float a, float time, Uint32 &start_time, Uint32 &total_time, fade_step &current_step, FADE_TEX texture_from_enum = FADE_TEX::NONE);
	bool CleanUp();
private:

	//Colors and Alpha values
	int r, g, b;
	float a;

	FADE_TEX texture_to_use = FADE_TEX::NONE;
	SDL_Texture* textures[MAX_FADE_TEXTURES] = {nullptr};
};

#endif //__MODULEFADETOBLACK_H__

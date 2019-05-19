#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Window.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Viewport.h"

j1FadeToBlack::j1FadeToBlack() : j1Module()
{
	name.assign("FadeToBlack");
	r = g = b = 0;
	a = 255.0f;
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	return true;
}

bool j1FadeToBlack::PostUpdate(SDL_Rect screen_rect, Uint32 &start_time, Uint32 &total_time, fade_step &current_step, SDL_Texture* tex)
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			// ---
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;

	}

	if (tex == nullptr)
	{
		// Finally render the color square with alpha on the screen
		SDL_SetRenderDrawColor(App->render->renderer, r, g, b, (Uint8)(normalized * a));

		SDL_RenderFillRect(App->render->renderer, &screen_rect);
	}
	else
	{
		SDL_SetTextureAlphaMod(tex, (Uint8)(normalized * a));

		App->render->Blit(tex, screen_rect.x, screen_rect.y);

		SDL_SetTextureAlphaMod(tex, 255);
	}

	return true;
}


// Fade to the desired color & max alpha occupying all screen.
bool j1FadeToBlack::FadeCustom(int r, int g, int b, float a, float time, Uint32 &start_time, Uint32 &total_time, fade_step &current_step)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);


		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		ret = true;
	}

	return ret;
}
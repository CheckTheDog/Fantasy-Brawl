#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Viewport.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1UIScene.h"

j1FadeToBlack::j1FadeToBlack() : j1Module()
{
	name.assign("FadeToBlack");
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
	if (!App->scene->player1->active)
	{
		if (App->scene->player1->current_stepD == fade_step::fade_from_black)
			App->scene->player1->current_stepD = fade_step::maintain;
		else if (App->ui_scene->actual_menu == START_MENU)
			App->scene->player2->current_stepD = fade_step::none;

		PostUpdate(App->view->four_views_1, App->scene->player1->start_timeD, App->scene->player1->total_timeD, App->scene->player1->current_stepD, App->scene->player1->colB, App->scene->player1->alphaB, App->entities->death_tex);
	}

	if (!App->scene->player2->active)
	{
		if (App->scene->player2->current_stepD == fade_step::fade_from_black)
			App->scene->player2->current_stepD = fade_step::maintain;
		else if (App->ui_scene->actual_menu == START_MENU)
			App->scene->player2->current_stepD = fade_step::none;

		PostUpdate(App->view->four_views_2, App->scene->player2->start_timeD, App->scene->player2->total_timeD, App->scene->player2->current_stepD, App->scene->player2->colB, App->scene->player2->alphaB, App->entities->death_tex);
	}

	if (!App->scene->player3->active)
	{
		if (App->scene->player3->current_stepD == fade_step::fade_from_black)
			App->scene->player3->current_stepD = fade_step::maintain;
		else if(App->ui_scene->actual_menu == START_MENU)
			App->scene->player3->current_stepD = fade_step::none;

		PostUpdate(App->view->four_views_3, App->scene->player3->start_timeD, App->scene->player3->total_timeD, App->scene->player3->current_stepD, App->scene->player3->colB, App->scene->player3->alphaB, App->entities->death_tex);
	}

	if (!App->scene->player4->active)
	{
		if (App->scene->player4->current_stepD == fade_step::fade_from_black)
			App->scene->player4->current_stepD = fade_step::maintain;
		else if (App->ui_scene->actual_menu == START_MENU)
			App->scene->player4->current_stepD = fade_step::none;

		PostUpdate(App->view->four_views_4, App->scene->player4->start_timeD, App->scene->player4->total_timeD, App->scene->player4->current_stepD, App->scene->player4->colB, App->scene->player4->alphaB, App->entities->death_tex);
	}

	return true;
}

bool j1FadeToBlack::PostUpdate(SDL_Rect screen_rect, Uint32 &start_time, Uint32 &total_time, fade_step &current_step, SDL_Color &color, float &alpha, SDL_Texture* tex)
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
		SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, (Uint8)(normalized * alpha));

		SDL_RenderFillRect(App->render->renderer, &screen_rect);
	}
	else
	{
		SDL_SetTextureAlphaMod(tex, (Uint8)(normalized * alpha));

		App->render->Blit(tex, screen_rect.x, screen_rect.y);

		SDL_SetTextureAlphaMod(tex, 255);
	}

	return true;
}


// Fade to the desired color & max alpha occupying all screen.
bool j1FadeToBlack::FadeCustom(int r, int g, int b, float &a, float time, Uint32 &start_time, Uint32 &total_time, fade_step &current_step, SDL_Color color)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		a = (float) color.a;

		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::CleanUp()
{
	return true;
}

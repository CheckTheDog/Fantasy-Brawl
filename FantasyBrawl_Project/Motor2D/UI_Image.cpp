#include "UI_Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"
#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1Viewport.h"
#include "j1Transition.h"
#include "j1Window.h"

void Image::BlitElement()
{
	BROFILER_CATEGORY("Image Blit", Profiler::Color::Chocolate);

	if (texture != App->gui->GetAtlas())
		SDL_SetTextureAlphaMod(texture, App->gui->alpha_value);

	iPoint globalPos = calculateAbsolutePosition();

	float scale = 1.0f;

	if (App->transition->doingMenuTransition)
	{
		if ((App->ui_scene->actual_menu == menu_id::SETTINGS_MENU 
			|| App->ui_scene->actual_menu == menu_id::START_MENU)
			&& App->ui_scene->previous_menu != menu_id::INGAMESETTINGS_MENU
			)
		{
			uint w, h;
			App->win->GetWindowSize(w, h);
			SDL_Rect tmp = { 0,0,w,h };

			App->render->DrawQuad(tmp, 0, 0, 0, 255);
		}
	}
	
	if (this == App->ui_scene->hp_bar1)
	{
		multiplier = (App->scene->player1->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if (App->scene->player1->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,0,0,0,0,0, scale);
		
	}

	else if (this == App->ui_scene->hp_bar2)
	{
		multiplier = (App->scene->player2->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if(App->scene->player2->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 0, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->hp_bar3)
	{
		multiplier = (App->scene->player3->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if (App->scene->player3->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 0, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->hp_bar4)
	{
		multiplier = (App->scene->player4->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if (App->scene->player4->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 0, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_bar1)
	{
		float time = App->scene->player1->superTimer.ReadSec()*20.0f;

		if (time >= 5.0f*20.0f)
		{
			time = 5.0f*20.0f;
		}

		multiplier = (time + tmp_section.w) / (5.0f*20.0f + section.w);
		tmp_section.w = (section.w * multiplier);

		if (App->scene->player1->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,1, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_bar2)
	{
		float time = App->scene->player2->superTimer.ReadSec()*20.0f;

		if (time >= 5.0f*20.0f)
		{
			time = 5.0f*20.0f;
		}

		multiplier = (time + tmp_section.w) / (5.0f*20.0f + section.w);
		tmp_section.w = (section.w * multiplier);

		if (App->scene->player2->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,2, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_bar3)
	{
		float time = App->scene->player3->superTimer.ReadSec()*20.0f;

		if (time >= 5.0f*20.0f)
		{
			time = 5.0f*20.0f;
		}

		multiplier = (time + tmp_section.w) / (5.0f*20.0f + section.w);
		tmp_section.w = (section.w * multiplier);

		if (App->scene->player3->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,3, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_bar4)
	{
		float time = App->scene->player4->superTimer.ReadSec()*20.0f;

		if (time >= 5.0f*20.0f)
		{
			time = 5.0f*20.0f;
		}

		multiplier = (time + tmp_section.w) / (5.0f*20.0f + section.w);
		tmp_section.w = (section.w * multiplier);

		if (App->scene->player4->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,4, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->shield_bar1)
	{
	float time = App->scene->player1->shieldTimer.ReadSec()*20.0f;

	if (time >= 10.0f*20.0f)
	{
		time = 10.0f*20.0f;
	}

	multiplier = (time + tmp_section.w) / (10.0f*20.0f + section.w);
	tmp_section.w = (section.w * multiplier);

	if (App->scene->player1->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 1, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->shield_bar2)
	{
	float time = App->scene->player2->shieldTimer.ReadSec()*20.0f;

	if (time >= 10.0f*20.0f)
	{
		time = 10.0f*20.0f;
	}

	multiplier = (time + tmp_section.w) / (10.0f*20.0f + section.w);
	tmp_section.w = (section.w * multiplier);

	if (App->scene->player2->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 2, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->shield_bar3)
	{
	float time = App->scene->player3->shieldTimer.ReadSec()*20.0f;

	if (time >= 10.0f*20.0f)
	{
		time = 10.0f*20.0f;
	}

	multiplier = (time + tmp_section.w) / (10.0f*20.0f + section.w);
	tmp_section.w = (section.w * multiplier);

	if (App->scene->player3->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 3, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->shield_bar4)
	{
	float time = App->scene->player4->shieldTimer.ReadSec()*20.0f;

	if (time >= 10.0f*20.0f)
	{
		time = 10.0f*20.0f;
	}

	multiplier = (time + tmp_section.w) / (10.0f*20.0f + section.w);
	tmp_section.w = (section.w * multiplier);

	if (App->scene->player4->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section, 4, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene-> hp_capsule1)
	{
		if (App->scene->player1->active)
		App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 0, 0, 0, 0, 0, scale);
	}

	else if ( this == App->ui_scene->hp_capsule2)
	{
		if (App->scene->player2->active)
			App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 0, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->hp_capsule3)
	{
		if (App->scene->player3->active)
			App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 0, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->hp_capsule4)
	{
		if (App->scene->player4->active)
			App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 0, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_capsule1 || this == App->ui_scene->shield_capsule1)
	{
		if (App->scene->player1->active)
		App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 1, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_capsule2 || this == App->ui_scene->shield_capsule2)
	{
		if (App->scene->player2->active)
		App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 2, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_capsule3 || this == App->ui_scene->shield_capsule3)
	{
		if (App->scene->player3->active)
		App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 3, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->sp_capsule4 || this == App->ui_scene->shield_capsule4)
	{
		if (App->scene->player4->active)
		App->view->PushQueue(9, texture, localPosition.x, localPosition.y, section, 4, 0, 0, 0, 0, scale);
	}

	else if (this == App->ui_scene->margin)
	{
		App->render->Blit(texture, localPosition.x, localPosition.y, &section);
	}

	else if (this == App->ui_scene->selection_image)
	{
		if (App->ui_scene->actual_menu == INGAME_MENU)
		{
			App->render->Blit(texture, globalPos.x, globalPos.y, &section);
		}

		else
		App->view->PushQueue(1, texture, 0, 0, section, 1);
	}

	else
		App->render->Blit(texture, globalPos.x, globalPos.y, &section);
}
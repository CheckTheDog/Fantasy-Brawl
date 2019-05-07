#include "UI_Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"
#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1Viewport.h"

void Image::BlitElement()
{
	BROFILER_CATEGORY("Image Blit", Profiler::Color::Chocolate);

	if (texture != App->gui->GetAtlas())
		SDL_SetTextureAlphaMod(texture, App->gui->alpha_value);
	iPoint globalPos = calculateAbsolutePosition();
	
	if (this == App->ui_scene->hp_bar1)
	{
		multiplier = (App->scene->player1->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if (App->scene->player1->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section);
		
	}

	else if (this == App->ui_scene->hp_bar2)
	{
		multiplier = (App->scene->player2->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if(App->scene->player2->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section);
	}

	else if (this == App->ui_scene->hp_bar3)
	{
		multiplier = (App->scene->player3->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if (App->scene->player3->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section);
	}

	else if (this == App->ui_scene->hp_bar4)
	{
		multiplier = (App->scene->player4->Entityinfo.health + tmp_section.w) / (player_hp + section.w);
		tmp_section.w = section.w * multiplier;

		if (App->scene->player4->active)
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section);
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
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,1);
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
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,2);
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
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,3);
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
		App->view->PushQueue(10, texture, localPosition.x, localPosition.y, tmp_section,4);
	}

	else
		App->render->Blit(texture, globalPos.x, globalPos.y, &section);
}
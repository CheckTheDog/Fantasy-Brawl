#include "j1Transition.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Viewport.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "p2Log.h"

j1Transition::j1Transition()
{
	alpha_value = 0; intro_alpha = 255;
}
j1Transition::~j1Transition()
{
}
bool j1Transition::Awake(pugi::xml_node& config)
{
	book = *App->entities->LoadAnimation("Animations/Book.tmx", "bookAnim");
	book.loop = false;
	book.speed = 12.0f;

	bookcover_anim = *App->entities->LoadAnimation("Animations/FullBook.tmx", "coverAnim");
	bookcover_anim.loop = false;
	bookcover_anim.speed = 12.0f;

	intro_anim = *App->entities->LoadAnimation("Animations/Intro.tmx", "Intro");
	intro_anim.loop = false;
	intro_anim.speed = 7.5f;

	return true;
}
bool j1Transition::Start()
{
	book_texture = App->tex->Load("gui/BgSprite.png");
	intro_tex = App->tex->Load("gui/AnimationLogo.png");
	bookcover_tex = App->tex->Load("gui/FullBgSprite.png");

	intro_timer.Start();

	return true;
}
bool j1Transition::Update(float dt)
{

	if (doingMenuTransition)
	{
		//if (App->ui_scene->actual_menu != menu_id::INGAME_MENU)
		//{
		//	uint w, h;
		//	App->win->GetWindowSize(w, h);
		//	SDL_Rect tmp = { 0,0,w,h };

		//	//App->render->DrawQuad(tmp, 0, 0, 0, 255);
		//}

		float Dalpha = 255 / (total_time / dt);
		if (menuState == GIN)
		{
			App->gui->alpha_value -= Dalpha;

			if (newMenuID == menu_id::SELECTION_MENU)
			App->view->PushQueue(11, bookcover_tex, 0, 0, bookcover_anim.frames[0]);
			else if(newMenuID == menu_id::START_MENU && App->ui_scene->previous_menu == menu_id::START_MENU)
			App->view->PushQueue(11, bookcover_tex, 0, 0, bookcover_anim.frames[bookcover_anim.last_frame]);
			else
			App->view->PushQueue(11, book_texture, 0, 0, book.frames[0]);

			if (App->gui->alpha_value <= 0)
			{
				App->gui->alpha_value = 0;

				uint width, height = 0;
				App->win->GetWindowSize(width, height);
				SDL_Rect screen = { 0,0,width,height };

				if(newMenuID == menu_id::SELECTION_MENU)
				App->view->PushQueue(12, bookcover_tex, 0, 0, bookcover_anim.GetCurrentFrame(dt));
				else if(newMenuID == menu_id::START_MENU && App->ui_scene->previous_menu == menu_id::START_MENU)
				App->view->PushQueue(12, bookcover_tex, 0, 0, bookcover_anim.GetCurrentFrame(dt));
				else
				App->view->PushQueue(12, book_texture, 0, 0, book.GetCurrentFrame(dt));

				if (book.Finished() 
					|| newMenuID == menu_id::INGAME_MENU 
					|| newMenuID == menu_id::INGAMESETTINGS_MENU
					|| (newMenuID == menu_id::START_MENU && App->ui_scene->actual_menu == menu_id::INGAMESETTINGS_MENU))
				{
					book.Reset();
					menuState = GOUT;
					App->ui_scene->loadMenu(newMenuID);
				}
				else if (bookcover_anim.Finished() || (bookcover_anim.speed < 0 && bookcover_anim.current_frame == 0))
				{

					if (newMenuID == menu_id::SELECTION_MENU)
					{
						bookcover_anim.Reset();
						bookcover_anim.current_frame = bookcover_anim.last_frame;
						bookcover_anim.speed *= -1;
					}
					else
					{
						bookcover_anim.speed = 12.0f;
						bookcover_anim.Reset();
					}

					menuState = GOUT;
					App->ui_scene->loadMenu(newMenuID);
				}
			}
		}
		else if (menuState == GOUT)
		{
			App->gui->alpha_value += Dalpha;

			if (newMenuID == menu_id::START_MENU)
				App->view->PushQueue(11, bookcover_tex, 0, 0, bookcover_anim.frames[0]);
			else
				App->view->PushQueue(11, book_texture, 0, 0, book.frames[0]);

			if (App->gui->alpha_value >= 255)
			{
				App->gui->alpha_value = 255;
				menuState = UNACTIVE;
				doingMenuTransition = false;
			}
		}
	}

	if (doingSceneTransition)
	{
		float Dalpha = 255 / (total_time / dt);
		if (sceneState == GOUT)
		{
			alpha_value -= Dalpha;
			if (alpha_value <= 0)
			{
				alpha_value = 0;
				sceneState = UNACTIVE;
				doingSceneTransition = false;
			}
		}
		else if (sceneState == GIN)
		{
			alpha_value += Dalpha;
			if (alpha_value >= 255)

			{
				alpha_value = 255;
				sceneState = GOUT;
				App->scene->newLvl = newLvl;
				App->scene->load_lvl = true;
			}

		}
	}

	return true;
}

bool j1Transition::PostUpdate(float dt)
{
	if (doingSceneTransition)
		App->render->DrawQuad({ 0, 0, App->win->screen_surface->w, App->win->screen_surface->h }, 0, 0, 0, alpha_value, true, false);

	if (intro_timer.ReadSec() > 0.5f && !intro_anim.Finished())
	{
		App->render->DrawQuad({ 0, 0, App->win->screen_surface->w, App->win->screen_surface->h }, 3, 3, 3, 255, true, false);
		App->render->Blit(intro_tex, -100, 65, &intro_anim.GetCurrentFrame(dt));
	}
	else if (!intro_anim.Finished())
	{
		App->render->DrawQuad({ 0, 0, App->win->screen_surface->w, App->win->screen_surface->h }, 3, 3, 3, 255, true, false);
		App->render->Blit(intro_tex, -100, 65, &intro_anim.frames[0]);
	}
	else if (intro_anim.Finished() && intro_alpha != 0)
	{
		intro_alpha = intro_alpha - 5;

		if (intro_alpha < 0)
			intro_alpha = 0;

		App->render->DrawQuad({ 0, 0, App->win->screen_surface->w, App->win->screen_surface->h }, 3, 3, 3, intro_alpha, true, false);

		SDL_SetTextureAlphaMod(intro_tex, intro_alpha);
		App->render->Blit(intro_tex, -100, 65, &intro_anim.GetCurrentFrame(dt));
	}

	return true;
}

void j1Transition::menuTransition(menu_id newMenuID, float time)
{

	this->newMenuID = newMenuID;
	timer.Start();
	total_time = time * 0.5f;
	doingMenuTransition = true;
	menuState = GIN;

}
void j1Transition::sceneTransition(int newLvl, float time)
{

	this->newLvl = newLvl;
	timer.Start();
	total_time = time * 0.5f;
	doingSceneTransition = true;
	sceneState = GIN;

}
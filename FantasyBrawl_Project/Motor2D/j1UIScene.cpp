#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Transition.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Image.h"
//#include "UI_Slider.h"
#include "UI_Window.h"
//#include "UI_Clock.h"


j1UIScene::j1UIScene()
{
	
}

j1UIScene::~j1UIScene()
{
}

bool j1UIScene::Awake()
{
	name.assign("UIScene");
	return true;
}

bool j1UIScene::Start()
{
	_TTF_Font* big_buttons_font = App->fonts->Load("fonts/finalf.ttf", 80);
	_TTF_Font* mid_buttons_font = App->fonts->Load("fonts/finalf.ttf", 50);
	_TTF_Font* big_texts_font = App->fonts->Load("fonts/finalf.ttf", 55);
	_TTF_Font* mid_texts_font = App->fonts->Load("fonts/finalf.ttf", 36);
	_TTF_Font* small_texts_font = App->fonts->Load("fonts/finalf.ttf", 15);
	_TTF_Font* huge_texts_font = App->fonts->Load("fonts/finalf.ttf", 80);
	_TTF_Font* special_text_font = App->fonts->Load("fonts/finalf.ttf", 55);

	SDL_Color yellow_color = { 229, 168, 61, 255 };
	SDL_Color white_color = { 255, 255, 255, 0 };
	SDL_Color grey_color = { 190, 177, 158, 191 };
	SDL_Color dark_yellow_color = { 146, 97, 45, 255 };
	SDL_Color black_color = { 0, 0, 0, 255 };
	SDL_Color brown_color = { 139,69,19 };


	

	//menu* creditsMenu = new menu(CREDITS_MENU);
	//{
	//	
	//}

	//menu* startMenu = new menu(START_MENU);
	//{
	//	


	//	//PLAY
	//	UI_element* new_game = App->gui->createButton(372 * App->gui->UI_scale, 250 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	//	new_game->function = NEW_GAME;

	//	UI_element* new_text = App->gui->createText("PLAY", 200, 200, big_buttons_font, brown_color);
	//	new_text->setOutlined(true);
	//	new_game->appendChildAtCenter(new_text);

	//	//CHAMPIONS
	//	continueButton = App->gui->createButton(372 * App->gui->UI_scale, 400 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	//	continueButton->function = CONTINUE;


	//	UI_element* continue_text = App->gui->createText("CHAMPIONS", 200, 200, big_buttons_font, brown_color);
	//	continue_text->setOutlined(true);
	//	continueButton->appendChildAtCenter(continue_text);

	//	//EXIT GAME
	//	UI_element* exit_game = App->gui->createButton(372 * App->gui->UI_scale, 550 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);

	//	exit_game->function = EXIT;

	//	UI_element* exit_text = App->gui->createText("EXIT", 200, 200, big_buttons_font, brown_color);
	//	exit_text->setOutlined(true);
	//	exit_game->appendChildAtCenter(exit_text);

	//	//CREDITS
	//	UI_element* credits = App->gui->createButton(App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 0,613,160,100 }, { 0,613,160,100 }, { 0,613,160,100 }, this);
	//	credits->function = CREDITS;



	//	//SETTINGS
	//	UI_element* settings_start_menu = App->gui->createButton(823 * App->gui->UI_scale, App->gui->UI_scale, NULL, { 250,613,170,150 }, { 250,613,170,150 }, { 250,613,170,150 }, this);

	//	settings_start_menu->function = SETTINGS;


	//	//BACKGROUND
	//	UI_element* background_image = App->gui->createImage(0, 0, App->tex->Load("gui/background.png"), this);

	//	startMenu->elements.push_back(background_image);
	//	startMenu->elements.push_back(new_game);
	//	startMenu->elements.push_back(new_text);
	//	startMenu->elements.push_back(continueButton);
	//	startMenu->elements.push_back(continue_text);
	//	startMenu->elements.push_back(exit_game);
	//	startMenu->elements.push_back(exit_text);
	//	startMenu->elements.push_back(credits);
	//	startMenu->elements.push_back(settings_start_menu);
	//	menus.push_back(startMenu);
	//}


	//menu* championSelection = new menu(SELECTION_MENU);
	//{
	//	UI_element* selection_window = App->gui->createWindow(51 * App->gui->UI_scale, 93 * App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 0,0,923,581 }, this);
	//	UI_element* settings_text = App->gui->createText("CHAMPION SELECTION", 425, 120, big_buttons_font, brown_color);
	//	settings_text->setOutlined(true);



	//	championSelection->elements.push_back(selection_window);
	//	menus.push_back(championSelection);
	//}

	//menu* settingsMenu = new menu(SETTINGS_MENU);
	//{
	//	//WINDOW
	//	UI_element* settings_window = App->gui->createWindow(51 * App->gui->UI_scale, 93 * App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 0,0,923,581 }, this);
	//	UI_element* settings_text = App->gui->createText("OPTIONS", 425, 120, big_buttons_font, brown_color);
	//	settings_text->setOutlined(true);


	//	//BACK BUTTON
	//	UI_element* back_button = App->gui->createButton(350 * App->gui->UI_scale, 525 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	//	back_button->function = BACK;
	//	UI_element* back_text = App->gui->createText("BACK", 200, 200, mid_buttons_font, brown_color);
	//	back_text->setOutlined(true);
	//	back_button->appendChildAtCenter(back_text);

	//	//AUDIO
	//	Button* music_slider_butt = App->gui->createButton(0, 0, NULL, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, this);
	//	Slider* music_slider = App->gui->createSlider(0, 0, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, music_slider_butt, mid_texts_font, brown_color, music_progress);
	//	music_slider->modify = MUSIC;
	//	settings_window->appendChild(430 * App->gui->UI_scale, 160 * App->gui->UI_scale, music_slider);

	//	UI_element* audio_text = App->gui->createText("AUDIO", 280, 240, mid_buttons_font, brown_color);
	//	audio_text->setOutlined(true);

	//	//FULLSCREEN
	//	Button* full_switch = App->gui->createSwitch(0, 0, NULL, { 404, 291, 47, 22 }, { 404, 291, 47, 22 }, { 404, 314, 47, 22 }, { 404, 314, 47, 22 }, this);
	//	settings_window->appendChild(550 * App->gui->UI_scale, 325 * App->gui->UI_scale, full_switch);

	//	UI_element* fullscreen_text = App->gui->createText("FULLSCREEN", 280, 400, mid_buttons_font, brown_color);
	//	fullscreen_text->setOutlined(true);



	//	settingsMenu->elements.push_back(settings_window);
	//	settingsMenu->elements.push_back(settings_text);
	//	settingsMenu->elements.push_back(back_button);
	//	settingsMenu->elements.push_back(back_text);
	//	settingsMenu->elements.push_back(music_slider_butt);
	//	settingsMenu->elements.push_back(music_slider);
	//	settingsMenu->elements.push_back(audio_text);
	//	settingsMenu->elements.push_back(full_switch);
	//	settingsMenu->elements.push_back(fullscreen_text);
	//	menus.push_back(settingsMenu);
	//}



	//current_menu = startMenu;

	//defaultValues.fx = fx_progress;
	//defaultValues.music = music_progress;
	//newValues = defaultValues;

	return true;
}

bool j1UIScene::PreUpdate()
{

	return true;
}

bool j1UIScene::Update(float dt)
{
	return true;
}

bool j1UIScene::PostUpdate(float dt)
{
	return true;
}

bool j1UIScene::OnUIEvent(UI_element* element, event_type event_type)
{
	bool ret = true;
	/*
	if (event_type == MOUSE_ENTER)
	{
		element->state = MOUSEOVER;

	}
	else if (event_type == MOUSE_LEAVE)
	{
		element->state = STANDBY;

	}
	else if (event_type == MOUSE_LEFT_CLICK)
	{
		element->state = CLICKED;

		if (element->element_type == SWITCH)
		{
			Button* tmp = (Button*)element;
			tmp->active = !tmp->active;
			newValues.fullscreen = tmp->active;
		}


		switch (element->function)
		{
		case NEW_GAME:
		{
			App->transition->menuTransition(SELECTION_MENU, 0.3);
			break;
		}
		case RESTART:
		{

		}
		break;
		case CONTINUE:
		{

		}
		break;
		case SETTINGS:
			App->transition->menuTransition(SETTINGS_MENU, 0.3);
			break;
		case CREDITS:

			break;
		case EXIT:
			ret = false;
			break;
		case PAUSE:
			if (!App->paused)
			{

			}
			else
			{

			}
			break;
		case APPLY:

			break;
		case CANCEL:

			break;
		case BACK:
			App->transition->menuTransition(previous_menu, 0.3);
			break;
		case RESTORE:

			break;
		case HOME:

			break;
		case WEBPAGE:

			break;
		}
	}


	else if (event_type == MOUSE_LEFT_RELEASE)
	{
		if (element->parent != nullptr && element->parent->element_type == SLIDER)
		{
			Slider* tmp = (Slider*)element->parent;
			switch (tmp->modify)
			{
			case MUSIC:
				newValues.music = tmp->progress;
				break;
			case FX:
				newValues.fx = tmp->progress;
				break;
			}
		}
		if (element->state == CLICKED)
			element->state = MOUSEOVER;
	}
	else if (event_type == MOUSE_RIGHT_CLICK)
	{
	}
	else if (event_type == MOUSE_RIGHT_RELEASE)
	{
	}
	else if (event_type == TIMER_ZERO)
	{
		LOG("Clock reached zero");
	}
	else if (event_type == STOPWATCH_ALARM)
	{
		Clock* clock = (Clock*)element;
		LOG("Clock alarm at: %d", clock->time);
	}*/

	return ret;
}

bool j1UIScene::CleanUp()
{
	std::list <menu*>::iterator item = menus.begin();

	while (item != menus.end())
	{
		delete *item;
		item++;
	}

	menus.clear();
	current_menu = nullptr;

	return true;
}


bool j1UIScene::Load(pugi::xml_node& data)
{

	return true;
}

bool j1UIScene::Save(pugi::xml_node& data) const
{

	return true;
}

bool j1UIScene::loadMenu(menu_id id)
{
	bool ret = false;
	/*
	previous_menu = current_menu->id;
	pauseClock();
	for (std::list <menu*>::const_iterator item = menus.begin(); item != menus.end(); item++)
	{
		if ((*item)->id == id)
		{
			current_menu = *item;
			playClock();
			ret = true;
			if (id == SETTINGS_MENU)
			{
				for (std::list <UI_element*>::const_iterator item2 = current_menu->elements.begin(); item2 != current_menu->elements.end(); ++item2)
				{
					if ((*item2)->element_type == SWITCH)
					{
						Button* full_switch = (Button*)*item2;
						startValues.fullscreen = full_switch->active;
					}
					if ((*item2)->element_type == SLIDER)
					{
						Slider* slider = (Slider*)*item2;
						switch (slider->modify)
						{
						case MUSIC:
							startValues.music = slider->getProgress();
							break;
						case FX:
							startValues.fx = slider->getProgress();
							break;
						}
					}
				}
			}
			break;
		}
	}

	*/
	return ret;
}



void j1UIScene::applySettings(settings_values values)
{
	/*Uint32 flag = 0;
	if (values.fullscreen)
		flag = SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(App->win->window, flag);

	App->audio->setMusicVolume(values.music);
	App->audio->setFxVolume(values.fx);

	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == SWITCH)
		{
			Button* full_switch = (Button*)*item;
			full_switch->active = values.fullscreen;
		}
		if ((*item)->element_type == SLIDER)
		{
			Slider* slider = (Slider*)*item;
			switch (slider->modify)
			{
			case MUSIC:
				slider->setProgress(values.music);
				break;
			case FX:
				slider->setProgress(values.fx);
				break;
			}
			slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
		}
	}*/
}

void j1UIScene::pauseClock()
{
	/*for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == CLOCK)
		{
			Clock* clock = (Clock*)*item;
			if (!clock->counter.isPaused())
				clock->counter.Pause();
		}
	}*/
}

void j1UIScene::playClock()
{
	///*for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	//{
	//	if ((*item)->element_type == CLOCK)
	//	{
	//		Clock* clock = (Clock*)*item;
	//		if (clock->counter.isPaused())
	//			clock->counter.Play();
	//	}
	//}*/
}
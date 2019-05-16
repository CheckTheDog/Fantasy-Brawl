#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1ArenaInteractions.h"
#include "p2Log.h"
#include "j1Transition.h"
#include "j1Input.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_Slider.h"
#include "UI_Window.h"
#include "UI_Clock.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "j1Gui.h"


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

	
	 small_texts_font = App->fonts->Load("fonts/BMYEONSUNG.ttf", 50);
	
	_TTF_Font* big_texts_font = App->fonts->Load("fonts/finalf.ttf", 55);
	_TTF_Font* mid_texts_font = App->fonts->Load("fonts/finalf.ttf", 36);
	
	_TTF_Font* huge_texts_font = App->fonts->Load("fonts/finalf.ttf", 80);
	_TTF_Font* special_text_font = App->fonts->Load("fonts/finalf.ttf", 55);
	 big_buttons_font = App->fonts->Load("fonts/finalf.ttf", 70);
	 mid_buttons_font = App->fonts->Load("fonts/finalf.ttf", 50);
	

	SDL_Color yellow_color = { 229, 168, 61, 255 };
	SDL_Color white_color = { 255, 255, 255, 0 };
	SDL_Color grey_color = { 190, 177, 158, 191 };
	SDL_Color dark_yellow_color = { 146, 97, 45, 255 };
	SDL_Color black_color = { 0, 0, 0, 255 };
	SDL_Color brown_color = { 139,69,19 };
	


	float music_progress = (float)App->audio->getMusicVolume() / 128;
	float fx_progress = (float)App->audio->getFxVolume() / 128;

	

	menu* creditsMenu = new menu(CREDITS_MENU);
	{
		
	}

	menu* startMenu = new menu(START_MENU);
	{
		


		//PLAY
		UI_element* new_game = App->gui->createButton(372 * App->gui->UI_scale, 250 * App->gui->UI_scale, NULL, { 0,148,278,106 }, { 286,148,278,106 }, { 570,148,278,106 }, this);
		new_game->function = NEW_GAME;

		UI_element* new_text = App->gui->createText("PLAY", 460, 270, big_buttons_font, brown_color);
		new_text->setOutlined(true);
		
		//CHAMPIONS
		continueButton = App->gui->createButton(372 * App->gui->UI_scale, 400 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 0,148,281,111 }, { 0,148,281,111 }, this);
		continueButton->function = CONTINUE;


		UI_element* continue_text = App->gui->createText("CHAMPIONS", 200, 200, big_buttons_font, brown_color);
		continue_text->setOutlined(true);
		continueButton->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(372 * App->gui->UI_scale, 550 * App->gui->UI_scale, NULL, { 0,148,278,106 }, { 286,148,278,106 }, { 570,148,278,106 }, this);

		exit_game->function = EXIT;

		UI_element* exit_text = App->gui->createText("EXIT", 200, 200, big_buttons_font, brown_color);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 0,425,190,94}, { 0,593,190,94}, { 0,749,190,94}, this);
		credits->function = WEBPAGE;



		//SETTINGS
		UI_element* settings_start_menu = App->gui->createButton(823 * App->gui->UI_scale, App->gui->UI_scale, NULL, { 250,386,160,157 }, { 250,547,160,157 }, { 250,712,160,157 }, this);

		settings_start_menu->function = SETTINGS;


		//BACKGROUND
		UI_element* background_image = App->gui->createImage(0, 0, App->tex->Load("gui/background.png"), this);

		startMenu->elements.push_back(background_image);
		startMenu->elements.push_back(new_game);
		startMenu->elements.push_back(new_text);
		startMenu->elements.push_back(continueButton);
		startMenu->elements.push_back(continue_text);
		startMenu->elements.push_back(exit_game);
		startMenu->elements.push_back(exit_text);
		startMenu->elements.push_back(credits);
		startMenu->elements.push_back(settings_start_menu);
		menus.push_back(startMenu);
	}

	menu* ingameMenu = new menu(INGAME_MENU);
	{
		// margin
		SDL_Texture* margin_tex = App->tex->Load("gui/TimerMargins.png");
		UI_element* margin = App->gui->createImage(0, 0, margin_tex, this);

		//player1
		UI_element* hp_bar_player1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y,  {70, 0, 43, 4}, this);
		hp_bar1 = hp_bar_player1;

		UI_element* sp_bar_player1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 114, 0, 22, 4 }, this);
		sp_bar1 = sp_bar_player1;

		UI_element* shield_bar_player1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 136, 0, 22, 4 }, this);
		shield_bar1 = shield_bar_player1;

		UI_element* hp_capsule_bar1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 0, 0, 45, 6 }, this);
		hp_capsule1 = hp_capsule_bar1;

		UI_element* sp_capsule_bar1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		sp_capsule1 = sp_capsule_bar1;

		UI_element* shield_capsule_bar1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		shield_capsule1 = shield_capsule_bar1;

		//player2
		UI_element* hp_bar_player2 = App->gui->createImageFromAtlas(App->scene->player2->Entityinfo.position.x, App->scene->player2->Entityinfo.position.y, { 70, 0, 43, 4 }, this);
		hp_bar2 = hp_bar_player2;

		UI_element* sp_bar_player2 = App->gui->createImageFromAtlas(App->scene->player2->Entityinfo.position.x, App->scene->player2->Entityinfo.position.y, { 114, 0, 22, 4 }, this);
		sp_bar2 = sp_bar_player2;

		UI_element* shield_bar_player2 = App->gui->createImageFromAtlas(App->scene->player2->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 136, 0, 22, 4 }, this);
		shield_bar2 = shield_bar_player2;

		UI_element* hp_capsule_bar2 = App->gui->createImageFromAtlas(App->scene->player2->Entityinfo.position.x, App->scene->player2->Entityinfo.position.y, { 0, 0, 45, 6 }, this);
		hp_capsule2 = hp_capsule_bar2;

		UI_element* sp_capsule_bar2 = App->gui->createImageFromAtlas(App->scene->player2->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		sp_capsule2 = sp_capsule_bar2;

		UI_element* shield_capsule_bar2 = App->gui->createImageFromAtlas(App->scene->player2->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		shield_capsule2 = shield_capsule_bar2;

		//player3
		UI_element* hp_bar_player3 = App->gui->createImageFromAtlas(App->scene->player3->Entityinfo.position.x, App->scene->player3->Entityinfo.position.y, { 70, 0, 43, 4 }, this);
		hp_bar3 = hp_bar_player3;

		UI_element* sp_bar_player3 = App->gui->createImageFromAtlas(App->scene->player3->Entityinfo.position.x, App->scene->player3->Entityinfo.position.y, { 114, 0, 22, 4 }, this);
		sp_bar3 = sp_bar_player3;

		UI_element* shield_bar_player3 = App->gui->createImageFromAtlas(App->scene->player3->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 136, 0, 22, 4 }, this);
		shield_bar3 = shield_bar_player3;

		UI_element* hp_capsule_bar3 = App->gui->createImageFromAtlas(App->scene->player3->Entityinfo.position.x, App->scene->player3->Entityinfo.position.y, { 0, 0, 45, 6 }, this);
		hp_capsule3 = hp_capsule_bar3;

		UI_element* sp_capsule_bar3 = App->gui->createImageFromAtlas(App->scene->player3->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		sp_capsule3 = sp_capsule_bar3;

		UI_element* shield_capsule_bar3 = App->gui->createImageFromAtlas(App->scene->player3->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		shield_capsule3 = shield_capsule_bar3;

		//player4
		UI_element* hp_bar_player4 = App->gui->createImageFromAtlas(App->scene->player4->Entityinfo.position.x, App->scene->player4->Entityinfo.position.y, { 70, 0, 43, 4 }, this);
		hp_bar4 = hp_bar_player4;

		UI_element* sp_bar_player4 = App->gui->createImageFromAtlas(App->scene->player4->Entityinfo.position.x, App->scene->player4->Entityinfo.position.y, { 114, 0, 22, 4 }, this);
		sp_bar4 = sp_bar_player4;

		UI_element* shield_bar_player4 = App->gui->createImageFromAtlas(App->scene->player4->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 136, 0, 22, 4 }, this);
		shield_bar4 = shield_bar_player4;

		UI_element* hp_capsule_bar4 = App->gui->createImageFromAtlas(App->scene->player4->Entityinfo.position.x, App->scene->player4->Entityinfo.position.y, { 0, 0, 45, 6 }, this);
		hp_capsule4 = hp_capsule_bar4;

		UI_element* sp_capsule_bar4 = App->gui->createImageFromAtlas(App->scene->player4->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		sp_capsule4 = sp_capsule_bar4;

		UI_element* shield_capsule_bar4 = App->gui->createImageFromAtlas(App->scene->player4->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 46, 0, 23, 6 }, this);
		shield_capsule4 = shield_capsule_bar4;
		

		ingameMenu->elements.push_back(margin);
		ingameMenu->elements.push_back(hp_capsule_bar1);
		ingameMenu->elements.push_back(hp_capsule_bar2);
		ingameMenu->elements.push_back(hp_capsule_bar3);
		ingameMenu->elements.push_back(hp_capsule_bar4);
		ingameMenu->elements.push_back(sp_capsule_bar1);
		ingameMenu->elements.push_back(sp_capsule_bar2);
		ingameMenu->elements.push_back(sp_capsule_bar3);
		ingameMenu->elements.push_back(sp_capsule_bar4);
		ingameMenu->elements.push_back(shield_capsule_bar1);
		ingameMenu->elements.push_back(shield_capsule_bar2);
		ingameMenu->elements.push_back(shield_capsule_bar3);
		ingameMenu->elements.push_back(shield_capsule_bar4);
		ingameMenu->elements.push_back(hp_bar_player1);
		ingameMenu->elements.push_back(hp_bar_player2);
		ingameMenu->elements.push_back(hp_bar_player3);
		ingameMenu->elements.push_back(hp_bar_player4);
		ingameMenu->elements.push_back(sp_bar_player1);
		ingameMenu->elements.push_back(sp_bar_player2);
		ingameMenu->elements.push_back(sp_bar_player3);
		ingameMenu->elements.push_back(sp_bar_player4);
		ingameMenu->elements.push_back(shield_bar_player1);
		ingameMenu->elements.push_back(shield_bar_player2);
		ingameMenu->elements.push_back(shield_bar_player3);
		ingameMenu->elements.push_back(shield_bar_player4);
		menus.push_back(ingameMenu);

	}

	championSelection = new menu(SELECTION_MENU);
	{
		UI_element* selection_image = App->gui->createImage(0, 0, App->tex->Load("gui/ChampSelect.png"), this);
		UI_element* selection_text = App->gui->createText("CHAMPION SELECTION", 450, 60, big_buttons_font, brown_color);
		selection_text->setOutlined(true);

		/*UI_element* champion_button1 = App->gui->createButton(300 * App->gui->UI_scale, 175 * App->gui->UI_scale, NULL, { 560,507,180,180 }, { 560,507,180,180 }, { 560,507,180,180 }, this);
		champion_button1->function = SELECTING;

		UI_element* champion_button2 = App->gui->createButton(500 * App->gui->UI_scale, 175* App->gui->UI_scale, NULL, { 740,507,180,180 }, { 740,507,180,180 }, { 740,507,180,180 }, this);
		UI_element* champion_button3 = App->gui->createButton(300 * App->gui->UI_scale, 375 * App->gui->UI_scale, NULL, { 560,696,180,180 }, { 560,696,180,180 }, { 560,696,180,180 }, this);
		UI_element* champion_button4 = App->gui->createButton(500 * App->gui->UI_scale, 375 * App->gui->UI_scale, NULL, { 740,696,180,180 }, { 740,696,180,180 }, { 740,696,180,180 }, this);*/

		//PLAYER QUADS
		UI_element* player1_quad = App->gui->createImageFromAtlas(20, 20, { 18, 904,200,180 }, this);
		UI_element* player2_quad = App->gui->createImageFromAtlas(20, 200, { 232, 904,200,180 }, this);
		UI_element* player3_quad = App->gui->createImageFromAtlas(20, 380, { 447, 904,200,180 }, this);
		UI_element* player4_quad = App->gui->createImageFromAtlas(20, 560, { 666, 904,200,180 }, this);

		//ARROWS
		UI_element* arrow1 = App->gui->createButton(230 * App->gui->UI_scale, 100 * App->gui->UI_scale, NULL, { 341,287,16,32 }, { 341,287,16,32 }, { 341,287,16,32 }, this);
		arrow1->function = SELECTING1;

		UI_element* arrow2 = App->gui->createButton(230 * App->gui->UI_scale, 280 * App->gui->UI_scale, NULL, { 341,287,16,32 }, { 341,287,16,32 }, { 341,287,16,32 }, this);
		arrow2->function = SELECTING2;

		UI_element* arrow3 = App->gui->createButton(230 * App->gui->UI_scale, 460 * App->gui->UI_scale, NULL, { 341,287,16,32 }, { 341,287,16,32 }, { 341,287,16,32 }, this);
		arrow3->function = SELECTING3;

		UI_element* arrow4 = App->gui->createButton(230 * App->gui->UI_scale, 640 * App->gui->UI_scale, NULL, { 341,287,16,32 }, { 341,287,16,32 }, { 341,287,16,32 }, this);
		arrow4->function = SELECTING4;

		//PLAYER IMAGE(?)
		mark1 = App->gui->createImageFromAtlas(35, 31, { 33, 1107, 169,122 }, this);
		photos[0] = mark1;
		
		mark2 = App->gui->createImageFromAtlas(33, 211, { 33, 1107, 169,122 }, this);
		photos[1] = mark2;

		mark3 = App->gui->createImageFromAtlas(33, 391, { 33, 1107, 169,122 }, this);
		photos[2] = mark3;

		mark4 = App->gui->createImageFromAtlas(33, 571, { 33, 1107, 169,122 }, this);
		photos[3] = mark4;
		/*photos[1] = mark2;*/
		/*mark3 = App->gui->createImageFromAtlas(552, 591, { 33, 1107, 169,122 }, this);
		mark4 = App->gui->createImageFromAtlas(812, 591, { 33, 1107, 169,122 }, this);*/


		//READY BUTTON
		ready = App->gui->createButton(500 * App->gui->UI_scale, 620 * App->gui->UI_scale, NULL, { 0,28,278,105 }, { 0,28,278,105 }, { 0,28,278,105 }, this);//{ 281,148,281,111 }, { 562,148,281,111 }
		ready_text = App->gui->createText("READY", 580, 640, big_buttons_font, black_color);
		

		championSelection->elements.push_back(selection_image);
		championSelection->elements.push_back(selection_text);
		/*championSelection->elements.push_back(champion_button1);
		championSelection->elements.push_back(champion_button2);
		championSelection->elements.push_back(champion_button3);
		championSelection->elements.push_back(champion_button4);*/
		championSelection->elements.push_back(player1_quad);
		championSelection->elements.push_back(player2_quad);
		championSelection->elements.push_back(player3_quad);
		championSelection->elements.push_back(player4_quad);
		championSelection->elements.push_back(mark1);
		championSelection->elements.push_back(mark2);
		championSelection->elements.push_back(mark3);
		championSelection->elements.push_back(mark4);
		championSelection->elements.push_back(ready);
		championSelection->elements.push_back(ready_text);
		championSelection->elements.push_back(arrow1);
		championSelection->elements.push_back(arrow2);
		championSelection->elements.push_back(arrow3);
		championSelection->elements.push_back(arrow4);
		menus.push_back(championSelection);
	}

	menu* settingsMenu = new menu(SETTINGS_MENU);
	{
		

		//WINDOW
		/*UI_element* settings_window = App->gui->createWindow(App->gui->UI_scale, App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 246,162,1000,718 }, this);*/
		UI_element* settings_image = App->gui->createImage(0, 0, App->tex->Load("gui/big_parchment.png"), this);
		UI_element* settings_text = App->gui->createText("OPTIONS", 425, 60, big_buttons_font, brown_color);
		settings_text->setOutlined(true);


		//BACK BUTTON
		UI_element* back_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0,148,278,106 }, { 286,148,278,106 }, { 570,148,278,106 }, this);
		back_button->function = BACK;
		UI_element* back_text = App->gui->createText("BACK", 300, 300, mid_buttons_font, brown_color);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		//AUDIO
		Button* music_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, this);
		Slider* music_slider = App->gui->createSlider(400, 255, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, music_slider_butt, mid_texts_font, brown_color, music_progress);
		music_slider->modify = MUSIC;
		settings_image->appendChild(430 * App->gui->UI_scale, 160 * App->gui->UI_scale, music_slider);

		UI_element* audio_text = App->gui->createText("AUDIO", 280, 240, mid_buttons_font, brown_color);
		audio_text->setOutlined(true);

		//FX
		Button* fx_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, this);
		Slider* fx_slider = App->gui->createSlider(400, 400, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, fx_slider_butt, mid_texts_font, brown_color, fx_progress);
		fx_slider->modify = FX;
		settings_image->appendChild(430 * App->gui->UI_scale, 160 * App->gui->UI_scale, fx_slider);

		UI_element* fx_text = App->gui->createText("FX", 280, 400, mid_buttons_font, brown_color);
		fx_text->setOutlined(true);

		//FULLSCREEN
		/*Button* full_switch = App->gui->createSwitch(600, 415, NULL, { 404, 291, 47, 22 }, { 404, 291, 47, 22 }, { 404, 314, 47, 22 }, { 404, 314, 47, 22 }, this);
		settings_image->appendChild(550 * App->gui->UI_scale, 325 * App->gui->UI_scale, full_switch);

		UI_element* fullscreen_text = App->gui->createText("FULLSCREEN", 280, 400, mid_buttons_font, brown_color);
		fullscreen_text->setOutlined(true);*/

		//APPLY
		UI_element* apply_button = App->gui->createButton(375 * App->gui->UI_scale, 450 * App->gui->UI_scale, NULL, { 0,148,278,106 }, { 286,148,278,106 }, { 570,148,278,106 }, this);
		apply_button->function = APPLY;

		UI_element* apply_text = App->gui->createText("APPLY", 200, 400, mid_texts_font, yellow_color);
		apply_text->setOutlined(true);
		apply_button->appendChildAtCenter(apply_text);
		
		settingsMenu->elements.push_back(settings_image);
		settingsMenu->elements.push_back(settings_text);
		settingsMenu->elements.push_back(back_button);
		settingsMenu->elements.push_back(back_text);
		settingsMenu->elements.push_back(music_slider_butt);
		settingsMenu->elements.push_back(music_slider);
		settingsMenu->elements.push_back(audio_text);
		settingsMenu->elements.push_back(fx_slider_butt);
		settingsMenu->elements.push_back(fx_slider);
		settingsMenu->elements.push_back(fx_text);
		settingsMenu->elements.push_back(apply_button);
		settingsMenu->elements.push_back(apply_text);
		/*settingsMenu->elements.push_back(full_switch);
		settingsMenu->elements.push_back(fullscreen_text);*/
		menus.push_back(settingsMenu);
	}

	

	finalMenu = new menu(FINAL_MENU);
	{

		/*CreateScoreBoard(2);*/

		/*player_winner = App->scene->GetWinner();

		if (player_winner != nullptr)
		{
			if(player_winner == App->scene->player1)
				CreateScoreBoard(1);
			else if(player_winner == App->scene->player2)
				CreateScoreBoard(2);
			else if (player_winner == App->scene->player3)
				CreateScoreBoard(3);
			else if (player_winner == App->scene->player4)
				CreateScoreBoard(4);
		}*/
		menus.push_back(finalMenu);
	}

	
	current_menu = startMenu;

	App->audio->PlayMusic(App->audio->pathIntro.data(), 0);

	defaultValues.fx = fx_progress;
	defaultValues.music = music_progress;
	newValues = defaultValues;

	return true;
}

bool j1UIScene::PreUpdate()
{
	return true;
}

bool j1UIScene::Update(float dt)
{
	bool ret = true;


	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (actual_menu == START_MENU)
		{
			ret = false;
		}
		else if (actual_menu == INGAME_MENU)
		{
			App->on_GamePause = true;
			actual_menu = SETTINGS_MENU;

			Mix_PauseMusic();
			App->transition->menuTransition(SETTINGS_MENU, 0.3f);
			App->arena_interactions->PauseStorm();
			App->audio->PlayFx(App->audio->fxPause);
			ret = true;
		}
		else if (actual_menu == SETTINGS_MENU && previous_menu == INGAME_MENU)
		{
			App->on_GamePause = false;
			actual_menu = INGAME_MENU;

			Mix_ResumeMusic();
			App->transition->menuTransition(INGAME_MENU, 0.3f);
			App->arena_interactions->ContinueStorm();
			ret = true;

		}
		else if (actual_menu == SELECTION_MENU)
		{
			App->audio->PlayMusic(App->audio->pathIntro.data(), 0);
			actual_menu = START_MENU;
			App->transition->menuTransition(START_MENU, 0.3f);
			ret = true;
		}
	}


	//PAUSING GAME WHILE ON MENUS
	if (actual_menu == START_MENU)
	{
		
		App->on_GamePause = true;
		
	}
	else if (actual_menu == SETTINGS_MENU)
	{
		
		App->on_GamePause = true;
	}
	else if (actual_menu == SELECTION_MENU)
	{
		App->on_GamePause = true;
	}
	else if (actual_menu == INGAME_MENU)
	{
		App->on_GamePause = false;
	}
	else if (actual_menu == FINAL_MENU)
	{
		App->on_GamePause = true;
	}
	
	//UPDATING HP CAPSULES POSITION
	hp_capsule1->localPosition.x = App->scene->player1->Entityinfo.position.x - (int)(App->scene->player1->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	hp_capsule1->localPosition.y = App->scene->player1->Entityinfo.position.y - 45;
	hp_capsule2->localPosition.x = App->scene->player2->Entityinfo.position.x - (int)(App->scene->player2->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	hp_capsule2->localPosition.y = App->scene->player2->Entityinfo.position.y - 45;
	hp_capsule3->localPosition.x = App->scene->player3->Entityinfo.position.x - (int)(App->scene->player3->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	hp_capsule3->localPosition.y = App->scene->player3->Entityinfo.position.y - 45;
	hp_capsule4->localPosition.x = App->scene->player4->Entityinfo.position.x - (int)(App->scene->player4->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	hp_capsule4->localPosition.y = App->scene->player4->Entityinfo.position.y - 45;

	//UPDATING SP CAPSULES POSITION
	sp_capsule1->localPosition.x = App->scene->player1->Entityinfo.position.x - (int)(App->scene->player1->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	sp_capsule1->localPosition.y = App->scene->player1->Entityinfo.position.y - 40;
	sp_capsule2->localPosition.x = App->scene->player2->Entityinfo.position.x - (int)(App->scene->player2->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	sp_capsule2->localPosition.y = App->scene->player2->Entityinfo.position.y - 40;
	sp_capsule3->localPosition.x = App->scene->player3->Entityinfo.position.x - (int)(App->scene->player3->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	sp_capsule3->localPosition.y = App->scene->player3->Entityinfo.position.y - 40;
	sp_capsule4->localPosition.x = App->scene->player4->Entityinfo.position.x - (int)(App->scene->player4->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 64;
	sp_capsule4->localPosition.y = App->scene->player4->Entityinfo.position.y - 40;

	//UPDATING SHIELD CAPSULES POSITION
	shield_capsule1->localPosition.x = App->scene->player1->Entityinfo.position.x - (int)(App->scene->player1->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 86;
	shield_capsule1->localPosition.y = App->scene->player1->Entityinfo.position.y - 40;
	shield_capsule2->localPosition.x = App->scene->player2->Entityinfo.position.x - (int)(App->scene->player2->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 86;
	shield_capsule2->localPosition.y = App->scene->player2->Entityinfo.position.y - 40;
	shield_capsule3->localPosition.x = App->scene->player3->Entityinfo.position.x - (int)(App->scene->player3->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 86;
	shield_capsule3->localPosition.y = App->scene->player3->Entityinfo.position.y - 40;
	shield_capsule4->localPosition.x = App->scene->player4->Entityinfo.position.x - (int)(App->scene->player4->Entityinfo.entitycoll->rect.w * 3.5f  * scale) + 86;
	shield_capsule4->localPosition.y = App->scene->player4->Entityinfo.position.y - 40;

	//UPDATING HP BARS POSITION
	hp_bar1->localPosition.x = App->scene->player1->Entityinfo.position.x - App->scene->player1->Entityinfo.entitycoll->rect.w + 25;
	hp_bar1->localPosition.y = App->scene->player1->Entityinfo.position.y - 44;				 
	hp_bar2->localPosition.x = App->scene->player2->Entityinfo.position.x - App->scene->player2->Entityinfo.entitycoll->rect.w + 25;
	hp_bar2->localPosition.y = App->scene->player2->Entityinfo.position.y - 44;
	hp_bar3->localPosition.x = App->scene->player3->Entityinfo.position.x - App->scene->player3->Entityinfo.entitycoll->rect.w + 25;
	hp_bar3->localPosition.y = App->scene->player3->Entityinfo.position.y - 44;
	hp_bar4->localPosition.x = App->scene->player4->Entityinfo.position.x - App->scene->player4->Entityinfo.entitycoll->rect.w + 25;
	hp_bar4->localPosition.y = App->scene->player4->Entityinfo.position.y - 44;
																								 
	//UPDATING SP BARS POSITION																	 
	sp_bar1->localPosition.x = App->scene->player1->Entityinfo.position.x - App->scene->player1->Entityinfo.entitycoll->rect.w + 25;
	sp_bar1->localPosition.y = App->scene->player1->Entityinfo.position.y - 39;
	sp_bar2->localPosition.x = App->scene->player2->Entityinfo.position.x - App->scene->player2->Entityinfo.entitycoll->rect.w + 25;
	sp_bar2->localPosition.y = App->scene->player2->Entityinfo.position.y - 39;
	sp_bar3->localPosition.x = App->scene->player3->Entityinfo.position.x - App->scene->player3->Entityinfo.entitycoll->rect.w + 25;
	sp_bar3->localPosition.y = App->scene->player3->Entityinfo.position.y - 39;
	sp_bar4->localPosition.x = App->scene->player4->Entityinfo.position.x - App->scene->player4->Entityinfo.entitycoll->rect.w + 25;
	sp_bar4->localPosition.y = App->scene->player4->Entityinfo.position.y - 39;

	//UPDATING SHIELD BARS POSITION																	 
	shield_bar1->localPosition.x = App->scene->player1->Entityinfo.position.x - App->scene->player1->Entityinfo.entitycoll->rect.w + 47;
	shield_bar1->localPosition.y = App->scene->player1->Entityinfo.position.y - 39;
	shield_bar2->localPosition.x = App->scene->player2->Entityinfo.position.x - App->scene->player2->Entityinfo.entitycoll->rect.w + 47;
	shield_bar2->localPosition.y = App->scene->player2->Entityinfo.position.y - 39;
	shield_bar3->localPosition.x = App->scene->player3->Entityinfo.position.x - App->scene->player3->Entityinfo.entitycoll->rect.w + 47;
	shield_bar3->localPosition.y = App->scene->player3->Entityinfo.position.y - 39;
	shield_bar4->localPosition.x = App->scene->player4->Entityinfo.position.x - App->scene->player4->Entityinfo.entitycoll->rect.w  + 47;
	shield_bar4->localPosition.y = App->scene->player4->Entityinfo.position.y - 39;

	//SELECTION LOGIC
	if(passing1 && player1_select == false)
	{
		if (counter1 == 0)
			photos[0]->section = { 33, 1107, 169,122 };

		if (counter1 == 1)
		{
			photos[0]->section = { 243, 1107, 169,122 };
			App->scene->player1->character = CHARACTER::WENDOLIN;
			App->scene->player1->AssignCharacter();
		}

		if (counter1 == 2)
		{
			photos[0]->section = { 435,1107,169,122 };
			App->scene->player1->character = CHARACTER::SIMON;
			App->scene->player1->AssignCharacter();
		}

		if (counter1 == 3)
		{
			photos[0]->section = { 618,1107,169,122 };
			App->scene->player1->character = CHARACTER::TRAKT;
			App->scene->player1->AssignCharacter();
		}

		if (counter1 == 4)
		{
			photos[0]->section = { 808,1107,169,122 };
			App->scene->player1->character = CHARACTER::MELIADOUL;
			App->scene->player1->AssignCharacter();
		}

		if (counter1 > 4)//again to wendolin
		{
			counter1 = 1;
		}
	
		//IF PLAYER1 INPUT A GAMEPAD, passing1 = false && player1_select = true
		
		
	}

	if (passing2 && player2_select == false)
	{
		if (counter2 == 0)
			photos[1]->section = { 33, 1107, 169,122 };

		if (counter2 == 1)
		{
			photos[1]->section = { 243, 1107, 169,122 };
			App->scene->player2->character = CHARACTER::WENDOLIN;
			App->scene->player2->AssignCharacter();
		}

		if (counter2 == 2)
		{
			photos[1]->section = { 435,1107,169,122 };
			App->scene->player2->character = CHARACTER::SIMON;
			App->scene->player2->AssignCharacter();
		}

		if (counter2 == 3)
		{
			photos[1]->section = { 618,1107,169,122 };
			App->scene->player2->character = CHARACTER::TRAKT;
			App->scene->player2->AssignCharacter();
		}

		if (counter2 == 4)
		{
			photos[1]->section = { 808,1107,169,122 };
			App->scene->player2->character = CHARACTER::MELIADOUL;
			App->scene->player2->AssignCharacter();
		}

		if (counter2 > 4)//again to wendolin
		{
			counter2 = 1;
		}

		//IF PLAYER2 INPUT A GAMEPAD, passing2 = false && player2_select = true


	}

	if (passing3 && player3_select == false)
	{
		if (counter3 == 0)
			photos[2]->section = { 33, 1107, 169,122 };

		if (counter3 == 1)
		{
			photos[2]->section = { 243, 1107, 169,122 };
			App->scene->player3->character = CHARACTER::WENDOLIN;
			App->scene->player3->AssignCharacter();
		}

		if (counter3 == 2)
		{
			photos[2]->section = { 435,1107,169,122 };
			App->scene->player3->character = CHARACTER::SIMON;
			App->scene->player3->AssignCharacter();
		}

		if (counter3 == 3)
		{
			photos[2]->section = { 618,1107,169,122 };
			App->scene->player3->character = CHARACTER::TRAKT;
			App->scene->player3->AssignCharacter();
		}

		if (counter3 == 4)
		{
			photos[2]->section = { 808,1107,169,122 };
			App->scene->player3->character = CHARACTER::MELIADOUL;
			App->scene->player3->AssignCharacter();
		}

		if (counter3 > 4)//again to wendolin
		{
			counter3 = 1;
		}

		//IF PLAYER3 INPUT A GAMEPAD, passing3 = false && player3_select = true


	}

	if (passing4 && player4_select == false)
	{
		if (counter4 == 0)
			photos[3]->section = { 33, 1107, 169,122 };

		if (counter4 == 1)
		{
			photos[3]->section = { 243, 1107, 169,122 };
			App->scene->player4->character = CHARACTER::WENDOLIN;
			App->scene->player4->AssignCharacter();
		}

		if (counter4 == 2)
		{
			photos[3]->section = { 435,1107,169,122 };
			App->scene->player4->character = CHARACTER::SIMON;
			App->scene->player4->AssignCharacter();
		}

		if (counter4 == 3)
		{
			photos[3]->section = { 618,1107,169,122 };
			App->scene->player4->character = CHARACTER::TRAKT;
			App->scene->player4->AssignCharacter();
		}

		if (counter4 == 4)
		{
			photos[3]->section = { 808,1107,169,122 };
			App->scene->player4->character = CHARACTER::MELIADOUL;
			App->scene->player4->AssignCharacter();
		}

		if (counter4 > 4)//again to wendolin
		{
			counter4 = 1;
		}

		//IF PLAYER4 INPUT A GAMEPAD, passing4 = false && player4_select = true


	}

	if (passing1 && passing2 && passing3 && passing4)//JUST BY NOW, WHEN PLAYERS CAN CONFIRM CHANGE FOR PLAYER_SELECT
	{
		ready->function = INGAME;
	}
	
	/*if (player1_select == true)
	{
		mark1->section = { 243, 1107, 169,122 };
	}
	else if (player1_select == false)
	{
		mark1->section = { 33, 1107, 169,122 };
	}

	if (player2_select == true)
	{
		mark2->section = { 243, 1107, 169,122 };
	}
	else if (player2_select == false)
	{
		mark2->section = { 33, 1107, 169,122 };
	}

	if (player3_select == true)
	{
		mark3->section = { 243, 1107, 169,122 };
	}
	else if (player3_select == false)
	{
		mark3->section = { 33, 1107, 169,122 };
	}

	if (player4_select == true)
	{
		mark4->section = { 243, 1107, 169,122 };
	}
	else if (player4_select == false)
	{
		mark4->section = { 33, 1107, 169,122 };
	}

	if (player1_select == true && player2_select == true && player3_select == true && player4_select == true)
	{
		ready->section = { 281,148,281,111 };
		ready->function = INGAME;
		
	}
	else
	{
		ready->section = { 0,28,278,105 };
		ready->function = NONE;
	}*/


	//GET TO SCOREBOARD SCREEN

	player_winner = App->scene->GetWinner();

	if (player_winner != nullptr && scoreboard == false)
	{
		player_winner->active = false;
		scoreboard = true;
		App->audio->PlayMusic(App->audio->pathLeaderBoard.data(), 0);
		actual_menu = FINAL_MENU;
		App->transition->menuTransition(FINAL_MENU, 0.3);
		if (player_winner == App->scene->player1)
			CreateScoreBoard(1);
		else if (player_winner == App->scene->player2)
			CreateScoreBoard(2);
		else if (player_winner == App->scene->player3)
			CreateScoreBoard(3);
		else if (player_winner == App->scene->player4)
			CreateScoreBoard(4);
		//If we finished, we need to destroy the storm, or the storm will affect entities as if it resumed
		//from the previous game when we play again
		App->arena_interactions->DestroyStorm();
		App->arena_interactions->PauseStorm();
	}

	


	return ret;
}

bool j1UIScene::PostUpdate(float dt)
{
	return true;
}

bool j1UIScene::OnUIEvent(UI_element* element, event_type event_type)
{
	bool ret = true;

	
	
	if (event_type == MOUSE_ENTER)
	{
		element->state = MOUSEOVER;
		App->audio->PlayFx(App->audio->fxCursor);
	}
	else if (event_type == MOUSE_LEAVE)
	{
		element->state = STANDBY;

	}
	else if (event_type == MOUSE_LEFT_CLICK)
	{
		
		element->state = CLICKED;

		/*if (element->element_type == SWITCH)
		{
			Button* tmp = (Button*)element;
			tmp->active = !tmp->active;
			newValues.fullscreen = tmp->active;
		}*/
		
	
		switch (element->function)
		{
		case NEW_GAME:
		{
			App->audio->PlayFx(App->audio->fxConfirm);

			//RESET SELECTION BOOLS && COUNTER
			counter1 = 0;
			counter2 = 0;
			counter3 = 0;
			counter4 = 0;
			player1_select = false;
			player2_select = false;
			player3_select = false;
			player4_select = false;

			// --- Reset everything ---
			App->scene->ResetAll();
			
			scoreboard = false;

			actual_menu = SELECTION_MENU;
			App->transition->menuTransition(SELECTION_MENU, 0.3);

			App->audio->PlayMusic(App->audio->pathChampSelect.data(), 0);
			break;
		}
		case INGAME:
		{			
			actual_menu = INGAME_MENU;
			App->transition->menuTransition(INGAME_MENU, 0.3);

			App->audio->PlayFx(App->audio->fxConfirm);
			App->audio->PlayFx(App->audio->fxBrawlStart);
			App->arena_interactions->StartStorm();
			App->arena_interactions->ContinueStorm();

			int music = rand() % 5 + 1;

			switch (music)
			{
			case 1:
				App->audio->PlayMusic(App->audio->pathMap1.data(), 0);
				break;
			case 2:
				App->audio->PlayMusic(App->audio->pathMap2_1.data(), 0);
				break;
			case 3:
				App->audio->PlayMusic(App->audio->pathMap2_2.data(), 0);
				break;
			case 4:
				App->audio->PlayMusic(App->audio->pathMap3.data(), 0);
				break;
			case 5:
				App->audio->PlayMusic(App->audio->pathMap4.data(), 0);
				break;
			}
		
			break;
		}
		case SELECTING1:
		{	
			passing1 = true;
			counter1++;
			

			/*counter++;

			if (counter == 1)
			{
				player1_select = true;
			}
			if (counter == 2)
			{
				player2_select = true;
			}
			if (counter == 3)
			{
				player3_select = true;
			}
			if (counter == 4)
			{
				player4_select = true;
			}

			if (counter <= 4)
				App->audio->PlayFx(App->audio->fxConfirmChamp);*/

			break;
		}
		case SELECTING2:
		{
			passing2 = true;
			counter2++;

			break;
		}
		case SELECTING3:
		{
			passing3 = true;
			counter3++;

			break;
		}
		case SELECTING4:
		{
			passing4 = true;
			counter4++;

			break;
		}
		case RESTART:
		{
			App->audio->PlayMusic(App->audio->pathMainMenu1.data(), 0);
			actual_menu = START_MENU;
			App->transition->menuTransition(START_MENU, 0.3);
			break;
		}
		break;
		case CONTINUE:
		{

		}
		break;
		case SETTINGS:
			App->audio->PlayFx(App->audio->fxConfirm);
			actual_menu = SETTINGS_MENU;
			App->transition->menuTransition(SETTINGS_MENU, 0.3);
			App->audio->PlayMusic(App->audio->pathOptions.data(), 0);

			break;
		case CREDITS:
			App->audio->PlayFx(App->audio->fxConfirm);
			break;
		case EXIT:
			ret = false;
			break;
		case PAUSE:

			break;
		case APPLY:
			applySettings(newValues);
			break;
		case CANCEL:

			break;
		case BACK:
			if (actual_menu == SETTINGS_MENU && previous_menu != INGAME_MENU)
			{
				App->transition->menuTransition(previous_menu, 0.3);
				actual_menu = START_MENU;
				App->audio->PlayMusic(App->audio->pathMainMenu1.data(),0);
				App->audio->PlayFx(App->audio->fxCancel);
			}

			if (actual_menu == SETTINGS_MENU && previous_menu == INGAME_MENU)
			{
				App->on_GamePause = false;
				App->arena_interactions->DestroyStorm();
				App->transition->menuTransition(START_MENU, 0.3);
				actual_menu = START_MENU;
				App->audio->PlayMusic(App->audio->pathMainMenu1.data(), 0);
				App->audio->PlayFx(App->audio->fxCancel);
			}
			break;
		case RESTORE:

			break;
		case HOME:

			break;
		case WEBPAGE:

			App->RequestBrowser("https://github.com/CheckTheDog/Fantasy-Brawl");
			App->audio->PlayFx(App->audio->fxConfirm);
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
	}

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
					/*if ((*item2)->element_type == SWITCH)
					{
						Button* full_switch = (Button*)*item2;
						startValues.fullscreen = full_switch->active;
					}*/
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

	
	return ret;
}



void j1UIScene::applySettings(settings_values values)
{
	/*Uint32 flag = 0;
	if (values.fullscreen)
		flag = SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(App->win->window, flag);*/

	App->audio->setMusicVolume(values.music);
	App->audio->setFxVolume(values.fx);

	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		/*if ((*item)->element_type == SWITCH)
		{
			Button* full_switch = (Button*)*item;
			full_switch->active = values.fullscreen;
		}*/
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
	}
}

void j1UIScene::pauseClock()
{
	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == CLOCK)
		{
			Clock* clock = (Clock*)*item;
			/*if (!clock->counter.isPaused())
				clock->counter.Pause();*/
		}
	}
}

void j1UIScene::playClock()
{
	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == CLOCK)
		{
			Clock* clock = (Clock*)*item;
			/*if (clock->counter.isPaused())
				clock->counter.Play();*/
		}
	}
}

void j1UIScene::CreateScoreBoard(int num)
{
	
	SDL_Color brown_color = { 139,69,19 };
	SDL_Color black_color = { 0, 0, 0, 255 };

	std::list <UI_element*>::iterator item = App->gui->UI_elements.begin();

	std::list <UI_element*>::iterator item_finalmenu = finalMenu->elements.begin();

	while (item != App->gui->UI_elements.end())
	{
		if (*item == *item_finalmenu)
		{
			if (App->gui->GetAtlas() != (*item)->texture)
				App->tex->UnLoad((*item)->texture);

			delete *item;
			App->gui->UI_elements.erase(item);
			finalMenu->elements.erase(item_finalmenu);
			
			item_finalmenu++;
		}

		item++;
	}

	finalMenu->elements.clear();

	//WINDOW

	UI_element* final_image = App->gui->createImage(0, 0, App->tex->Load("gui/big_parchment.png"), this);

	UI_element* final_text = App->gui->createText("SCOREBOARD", 400, 60, big_buttons_font, brown_color);
	final_text->setOutlined(true);

	//SCOREBOARD INFO
	UI_element* winner_text = App->gui->createText("WINNER:", 260, 230, big_buttons_font, brown_color);
	winner_text->setOutlined(true);


	switch (num)
	{
		case 1:
		{
			win_text = App->gui->createText("PLAYER 1", 500, 240, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
		case 2:
		{
			win_text = App->gui->createText("PLAYER 2", 500, 240, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
		case 3:
		{
			win_text = App->gui->createText("PLAYER 3", 500, 240, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
		case 4:
		{
			win_text = App->gui->createText("PLAYER 4", 500, 240, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
	}
	
	//PLAYER1 KILLS

	UI_element* player1kills = App->gui->createText("PLAYER 1 kills:" ,260, 300, small_texts_font, brown_color);
	player1kills->setOutlined(true);

	std::string p1kills  = std::to_string(App->scene->player1->kills);

	UI_element* p1_kills = App->gui->createText(p1kills.data(), 600, 300, small_texts_font, black_color);
	p1_kills->setOutlined(true);

	//PLAYER2 KILLS
	UI_element* player2kills = App->gui->createText("PLAYER 2 kills:", 260, 370, small_texts_font, brown_color);
	player2kills->setOutlined(true);

	std::string p2kills = std::to_string(App->scene->player2->kills);

	UI_element* p2_kills = App->gui->createText(p2kills.data(), 600, 370, small_texts_font, black_color);
	p2_kills->setOutlined(true);

	//PLAYER3 KILLS
	UI_element* player3kills = App->gui->createText("PLAYER 3 kills:", 260, 440, small_texts_font, brown_color);
	player3kills->setOutlined(true);

	std::string p3kills = std::to_string(App->scene->player3->kills);

	UI_element* p3_kills = App->gui->createText(p3kills.data(), 600, 440, small_texts_font, black_color);
	p3_kills->setOutlined(true);

	//PLAYER4 KILLS
	UI_element* player4kills = App->gui->createText("PLAYER 4 kills:", 260, 510, small_texts_font, brown_color);
	player4kills->setOutlined(true);

	std::string p4kills = std::to_string(App->scene->player4->kills);

	UI_element* p4_kills = App->gui->createText(p4kills.data(), 600, 510, small_texts_font, black_color);
	p4_kills->setOutlined(true);
	

	//END BUTTON
	UI_element* end_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0,148,278,106 }, { 286,148,278,106 }, { 570,148,278,106 }, this);
	end_button->function = RESTART;
	UI_element* end_text = App->gui->createText("END", 300, 300, mid_buttons_font, brown_color);
	end_text->setOutlined(true);
	end_button->appendChildAtCenter(end_text);




	finalMenu->elements.push_back(final_image);
	finalMenu->elements.push_back(final_text);
	finalMenu->elements.push_back(winner_text);
	finalMenu->elements.push_back(win_text);
	finalMenu->elements.push_back(player1kills);
	finalMenu->elements.push_back(p1_kills);
	finalMenu->elements.push_back(player2kills);
	finalMenu->elements.push_back(p2_kills);
	finalMenu->elements.push_back(player3kills);
	finalMenu->elements.push_back(p3_kills);
	finalMenu->elements.push_back(player4kills);
	finalMenu->elements.push_back(p4_kills);
	finalMenu->elements.push_back(end_button);
	finalMenu->elements.push_back(end_text);

	LOG("%i", finalMenu->elements.size());

}


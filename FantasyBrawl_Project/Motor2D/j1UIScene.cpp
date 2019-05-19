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
#include "UI_Marker.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "j1Gui.h"
#include "j1Viewport.h"
#include <string>


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
	 big_buttons_font = App->fonts->Load("fonts/finalf.ttf", 70);
	 mid_buttons_font = App->fonts->Load("fonts/finalf.ttf", 50);
	 small_font = App->fonts->Load("fonts/Minecraft.ttf",30);
	 big_font = App->fonts->Load("fonts/Minecraft.ttf", 70);
	 mid_font = App->fonts->Load("fonts/Minecraft.ttf", 50);
	_TTF_Font* big_texts_font = App->fonts->Load("fonts/finalf.ttf", 55);
	_TTF_Font* mid_texts_font = App->fonts->Load("fonts/finalf.ttf", 36);
	_TTF_Font* huge_texts_font = App->fonts->Load("fonts/finalf.ttf", 80);
	_TTF_Font* special_text_font = App->fonts->Load("fonts/finalf.ttf", 55);

	 big_buttons_font = App->fonts->Load("fonts/finalf.ttf", 70);
	 mid_buttons_font = App->fonts->Load("fonts/finalf.ttf", 50);
	 panel = App->tex->Load("gui/Options.png");
	 champselect_bg = App->tex->Load("gui/ChampSelect.png");
	 A_Butt = App->tex->Load("gui/controller_buttons.png");


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
		UI_element* new_game = App->gui->createButton(500 * App->gui->UI_scale, 250 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);
		new_game->function = NEW_GAME;

		UI_element* new_text = App->gui->createText("PLAY", 588, 270, big_buttons_font, brown_color);
		new_text->setOutlined(true);
		
		//OPTIONS
		UI_element* settings_start_menu = App->gui->createButton(500 * App->gui->UI_scale, 400 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);
		settings_start_menu->function = SETTINGS;


		UI_element* continue_text = App->gui->createText("OPTIONS", 588, 200, big_buttons_font, brown_color);
		continue_text->setOutlined(true);
		settings_start_menu->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(500 * App->gui->UI_scale, 550 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);

		exit_game->function = EXIT;

		UI_element* exit_text = App->gui->createText("EXIT", 588, 200, big_buttons_font, brown_color);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(300 * App->gui->UI_scale, 600 * App->gui->UI_scale, NULL, { 0,425,190,94}, { 0,593,190,94}, { 0,749,190,94}, this);
		credits->function = WEBPAGE;

		//MARKER
		UI_element* feather = App->gui->createMarker(0, 0, { 20,0 },&startMenu->gamepads_focus[0],App->gui->atlas, {875,174,84,47},nullptr);



		//BACKGROUND
		UI_element* background_image = App->gui->createImage(0, 0, App->tex->Load("gui/MenuStart.png"), this);

		startMenu->elements.push_back(background_image);
		startMenu->elements.push_back(new_game);
		startMenu->elements.push_back(new_text);

		startMenu->elements.push_back(exit_game);
		startMenu->elements.push_back(exit_text);
		startMenu->elements.push_back(credits);
		startMenu->elements.push_back(settings_start_menu);
		startMenu->elements.push_back(continue_text);
		startMenu->elements.push_back(feather);


		menus.push_back(startMenu);
		AddControllerSupport(new_game, PLAYER::P1, START_MENU);
		AddControllerSupport(settings_start_menu, PLAYER::P1, START_MENU);
		AddControllerSupport(exit_game, PLAYER::P1, START_MENU);
		AddControllerSupport(credits, PLAYER::P1, START_MENU);

		
	}

	ingameMenu = new menu(INGAME_MENU);
	{
		// margin
		margin_tex = App->tex->Load("gui/TimerMargins.png");
		margin = App->gui->createImage(0, 0, margin_tex, this);

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

	menu* championSelection = new menu(SELECTION_MENU);
	{

		selection_image = App->gui->createImage(0, 0, champselect_bg, this);
		UI_element* selection_text = App->gui->createText("CHAMPION SELECTION", 450, 60, big_buttons_font, brown_color);
		selection_text->setOutlined(true);


		//PLAYER QUADS
		player1_quad = App->gui->createImageFromAtlas(40, 3, { 288, 518, 170,191 }, this);
		player2_quad = App->gui->createImageFromAtlas(40, 194, { 288, 518, 170,191 }, this);
		player3_quad = App->gui->createImageFromAtlas(40, 385, { 288, 518, 170,191 }, this);
		player4_quad = App->gui->createImageFromAtlas(40, 576, { 288, 518, 170,191 }, this);

		//PLAYER TEXTS
		UI_element* player1_text = App->gui->createImageFromAtlas(60, 166, { 296, 799, 117, 20 }, this);
		UI_element* player2_text = App->gui->createImageFromAtlas(60, 357, { 413, 799,120, 20 }, this);
		UI_element* player3_text = App->gui->createImageFromAtlas(60, 548, { 534, 799,120, 20 }, this);
		UI_element* player4_text = App->gui->createImageFromAtlas(60, 739, { 654, 799,120, 20 }, this);

		//ARROWS
		UI_element* arrow1 = App->gui->createButton(230 * App->gui->UI_scale, 100 * App->gui->UI_scale, NULL, { 883, 814, 25, 21 }, { 883, 814, 25, 21 }, { 883, 774, 25, 21}, this);
		arrow1->function = SELECTING1;

		UI_element* arrow2 = App->gui->createButton(230 * App->gui->UI_scale, 280 * App->gui->UI_scale, NULL, { 883, 814, 25, 21 }, { 883, 814, 25, 21 }, { 883, 774, 25, 21 }, this);
		arrow2->function = SELECTING2;

		UI_element* arrow3 = App->gui->createButton(230 * App->gui->UI_scale, 460 * App->gui->UI_scale, NULL, { 883, 814, 25, 21 }, { 883, 814, 25, 21 }, { 883, 774, 25, 21 }, this);
		arrow3->function = SELECTING3;

		UI_element* arrow4 = App->gui->createButton(230 * App->gui->UI_scale, 640 * App->gui->UI_scale, NULL, { 883, 814, 25, 21 }, { 883, 814, 25, 21 }, { 883, 774, 25, 21 }, this);
		arrow4->function = SELECTING4;

		UI_element* inv_arrow1 = App->gui->createButton(5 * App->gui->UI_scale, 100 * App->gui->UI_scale, NULL, { 855, 814, 25, 21 }, { 855, 814, 25, 21 }, { 855, 774, 25, 21 }, this);
		inv_arrow1->function = INV_SELECTING1;

		UI_element* inv_arrow2 = App->gui->createButton(5 * App->gui->UI_scale, 280 * App->gui->UI_scale, NULL, { 855, 814, 25, 21 }, { 855, 814, 25, 21 }, { 855, 774, 25, 21 }, this);
		inv_arrow2->function = INV_SELECTING2;

		UI_element* inv_arrow3 = App->gui->createButton(5 * App->gui->UI_scale, 460 * App->gui->UI_scale, NULL, { 855, 814, 25, 21 }, { 855, 814, 25, 21 }, { 855, 774, 25, 21 }, this);
		inv_arrow3->function = INV_SELECTING3;

		UI_element* inv_arrow4 = App->gui->createButton(5 * App->gui->UI_scale, 640 * App->gui->UI_scale, NULL, { 855, 814, 25, 21 }, { 855, 814, 25, 21 }, { 855, 774, 25, 21 }, this);
		inv_arrow4->function = INV_SELECTING4;

		//PLAYER IMAGE(?)
		mark1 = App->gui->createImageFromAtlas(58, 15, { 698, 384, 142, 153 }, this);
		photos[0] = mark1;
		
		mark2 = App->gui->createImageFromAtlas(58, 206, { 698, 384, 142, 153 }, this);
		photos[1] = mark2;

		mark3 = App->gui->createImageFromAtlas(58, 397, { 698, 384, 142, 153 }, this);
		photos[2] = mark3;

		mark4 = App->gui->createImageFromAtlas(58, 588, { 698, 384, 142, 153 }, this);
		photos[3] = mark4;
		


		//READY BUTTON
		ready = App->gui->createButton(500 * App->gui->UI_scale, 2000 * App->gui->UI_scale, NULL, { 0,28,278,105 }, { 0,28,278,105 }, { 0,28,278,105 }, this);//{ 281,148,281,111 }, { 562,148,281,111 }
		ready_text = App->gui->createText("READY", 2000, 640, big_buttons_font, black_color);


		player1_quad->appendChild(175, 90, arrow1);
		player1_quad->appendChild(-30, 90, inv_arrow1);

		player2_quad->appendChild(175, 90, arrow2);
		player2_quad->appendChild(-30, 90, inv_arrow2);

		player3_quad->appendChild(175, 90, arrow3);
		player3_quad->appendChild(-30, 90, inv_arrow3);

		player4_quad->appendChild(175, 90, arrow4);
		player4_quad->appendChild(-30, 90, inv_arrow4);
		


		championSelection->elements.push_back(selection_image);
		championSelection->elements.push_back(selection_text);
		championSelection->elements.push_back(mark1);
		championSelection->elements.push_back(mark2);
		championSelection->elements.push_back(mark3);
		championSelection->elements.push_back(mark4);
		championSelection->elements.push_back(player1_quad);
		championSelection->elements.push_back(player2_quad);
		championSelection->elements.push_back(player3_quad);
		championSelection->elements.push_back(player4_quad);
		championSelection->elements.push_back(ready);
		championSelection->elements.push_back(ready_text);
		championSelection->elements.push_back(arrow1);
		championSelection->elements.push_back(arrow2);
		championSelection->elements.push_back(arrow3);
		championSelection->elements.push_back(arrow4);
		championSelection->elements.push_back(inv_arrow1);
		championSelection->elements.push_back(inv_arrow2);
		championSelection->elements.push_back(inv_arrow3);
		championSelection->elements.push_back(inv_arrow4);
		championSelection->elements.push_back(player1_text);
		championSelection->elements.push_back(player2_text);
		championSelection->elements.push_back(player3_text);
		championSelection->elements.push_back(player4_text);

		menus.push_back(championSelection);

		
		AddControllerSupport(player1_quad,PLAYER::P1,SELECTION_MENU);

		
		AddControllerSupport(player2_quad, PLAYER::P2, SELECTION_MENU);

		
		AddControllerSupport(player3_quad, PLAYER::P3, SELECTION_MENU);

		
		AddControllerSupport(player4_quad, PLAYER::P4, SELECTION_MENU);
		/*AddControllerSupport(inv_arrow1, PLAYER::P1, SELECTION_MENU);
		AddControllerSupport(arrow2, PLAYER::P2, SELECTION_MENU);
		AddControllerSupport(inv_arrow2, PLAYER::P2, SELECTION_MENU);
		AddControllerSupport(arrow3, PLAYER::P3, SELECTION_MENU);
		AddControllerSupport(inv_arrow3, PLAYER::P3, SELECTION_MENU);
		AddControllerSupport(arrow4, PLAYER::P4, SELECTION_MENU);
		AddControllerSupport(inv_arrow4, PLAYER::P4, SELECTION_MENU);*/
	}

	menu* settingsMenu = new menu(SETTINGS_MENU);
	{
		

		//WINDOW
		/*UI_element* settings_window = App->gui->createWindow(App->gui->UI_scale, App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 246,162,1000,718 }, this);*/
		UI_element* settings_image = App->gui->createImage(0, 0, panel, this);
		UI_element* settings_text = App->gui->createText("OPTIONS", 425, 60, big_buttons_font, brown_color);
		settings_text->setOutlined(true);


		//BACK BUTTON
		UI_element* back_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);
		back_button->function = BACK;
		UI_element* back_text = App->gui->createText("BACK", 300, 300, mid_buttons_font, brown_color);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		//AUDIO
		Button* music_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, this);

		music_sliderMM = App->gui->createSlider(400, 100, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, music_slider_butt, mid_texts_font, brown_color, music_progress);
		music_sliderMM->modify = MUSIC;
		settings_image->appendChild(500 * App->gui->UI_scale, 125 * App->gui->UI_scale, music_sliderMM);


		UI_element* audio_text = App->gui->createText("AUDIO", 280, 240, mid_buttons_font, brown_color);
		audio_text->setOutlined(true);

		//FX
		Button* fx_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, this);

		fx_sliderMM = App->gui->createSlider(400, 400, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, fx_slider_butt, mid_texts_font, brown_color, fx_progress);
		fx_sliderMM->modify = FX;
		settings_image->appendChild(500 * App->gui->UI_scale, 175 * App->gui->UI_scale, fx_sliderMM);


		UI_element* fx_text = App->gui->createText("FX", 280, 400, mid_buttons_font, brown_color);
		fx_text->setOutlined(true);

		//FULLSCREEN
		/*Button* full_switch = App->gui->createSwitch(600, 415, NULL, { 404, 291, 47, 22 }, { 404, 291, 47, 22 }, { 404, 314, 47, 22 }, { 404, 314, 47, 22 }, this);
		settings_image->appendChild(550 * App->gui->UI_scale, 325 * App->gui->UI_scale, full_switch);

		UI_element* fullscreen_text = App->gui->createText("FULLSCREEN", 280, 400, mid_buttons_font, brown_color);
		fullscreen_text->setOutlined(true);*/

		//APPLY
		UI_element* apply_button = App->gui->createButton(375 * App->gui->UI_scale, 450 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);
		apply_button->function = APPLY;

		UI_element* apply_text = App->gui->createText("APPLY", 200, 400, mid_texts_font, yellow_color);
		apply_text->setOutlined(true);
		apply_button->appendChildAtCenter(apply_text);

		//MARKER
		UI_element* feather = App->gui->createMarker(0, 0, { 20,0 }, &settingsMenu->gamepads_focus[0], App->gui->atlas, { 875,174,84,47 }, nullptr);
		
		settingsMenu->elements.push_back(settings_image);
		settingsMenu->elements.push_back(settings_text);
		settingsMenu->elements.push_back(back_button);
		settingsMenu->elements.push_back(back_text);
		settingsMenu->elements.push_back(music_slider_butt);
		settingsMenu->elements.push_back(music_sliderMM);
		settingsMenu->elements.push_back(audio_text);
		settingsMenu->elements.push_back(fx_slider_butt);
		settingsMenu->elements.push_back(fx_sliderMM);
		settingsMenu->elements.push_back(fx_text);
		settingsMenu->elements.push_back(apply_button);
		settingsMenu->elements.push_back(apply_text);
		settingsMenu->elements.push_back(feather);
		/*settingsMenu->elements.push_back(full_switch);
		settingsMenu->elements.push_back(fullscreen_text);*/
		menus.push_back(settingsMenu);

		AddControllerSupport(music_slider_butt, PLAYER::P1, SETTINGS_MENU);
		AddControllerSupport(fx_slider_butt, PLAYER::P1, SETTINGS_MENU);
		AddControllerSupport(apply_button, PLAYER::P1, SETTINGS_MENU);
		AddControllerSupport(back_button, PLAYER::P1, SETTINGS_MENU);

	}

	menu* ingamesettingsMenu = new menu(INGAMESETTINGS_MENU);
	{

		//WINDOW
		/*UI_element* settings_window = App->gui->createWindow(App->gui->UI_scale, App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 246,162,1000,718 }, this);*/
		UI_element* settings_image = App->gui->createImage(0, 0, panel, this);
		UI_element* settings_text = App->gui->createText("OPTIONS", 425, 60, big_buttons_font, brown_color);
		settings_text->setOutlined(true);


		//BACK BUTTON
		UI_element* back_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);
		back_button->function = BACK;
		UI_element* back_text = App->gui->createText("BACK", 300, 300, mid_buttons_font, brown_color);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		//AUDIO
		Button* music_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, this);
		music_slider = App->gui->createSlider(400, 255, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, music_slider_butt, mid_texts_font, brown_color, music_progress);
		music_slider->modify = MUSIC;
		settings_image->appendChild(400 * App->gui->UI_scale, 255 * App->gui->UI_scale, music_slider);

		UI_element* audio_text = App->gui->createText("AUDIO", 280, 240, mid_buttons_font, brown_color);
		audio_text->setOutlined(true);

		//FX
		Button* fx_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, { 341, 287, 17, 40 }, this);
		fx_slider = App->gui->createSlider(400, 400, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, fx_slider_butt, mid_texts_font, brown_color, fx_progress);
		fx_slider->modify = FX;
		settings_image->appendChild(400 * App->gui->UI_scale, 400 * App->gui->UI_scale, fx_slider);

		UI_element* fx_text = App->gui->createText("FX", 280, 400, mid_buttons_font, brown_color);
		fx_text->setOutlined(true);

		//FULLSCREEN
		/*Button* full_switch = App->gui->createSwitch(600, 415, NULL, { 404, 291, 47, 22 }, { 404, 291, 47, 22 }, { 404, 314, 47, 22 }, { 404, 314, 47, 22 }, this);
		settings_image->appendChild(550 * App->gui->UI_scale, 325 * App->gui->UI_scale, full_switch);

		UI_element* fullscreen_text = App->gui->createText("FULLSCREEN", 280, 400, mid_buttons_font, brown_color);
		fullscreen_text->setOutlined(true);*/

		//APPLY
		UI_element* apply_button = App->gui->createButton(375 * App->gui->UI_scale, 450 * App->gui->UI_scale, NULL, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, { 0, 74, 267, 101 }, this);
		apply_button->function = APPLY;

		UI_element* apply_text = App->gui->createText("APPLY", 200, 400, mid_texts_font, yellow_color);
		apply_text->setOutlined(true);
		apply_button->appendChildAtCenter(apply_text);

		//MARKER
		UI_element* feather = App->gui->createMarker(0, 0, { 20, 0 }, &ingamesettingsMenu->gamepads_focus[0], App->gui->atlas, { 875,174,84,47 }, nullptr);




		ingamesettingsMenu->elements.push_back(settings_image);
		ingamesettingsMenu->elements.push_back(settings_text);
		ingamesettingsMenu->elements.push_back(back_button);
		ingamesettingsMenu->elements.push_back(back_text);
		ingamesettingsMenu->elements.push_back(music_slider_butt);
		ingamesettingsMenu->elements.push_back(music_slider);
		ingamesettingsMenu->elements.push_back(audio_text);
		ingamesettingsMenu->elements.push_back(fx_slider_butt);
		ingamesettingsMenu->elements.push_back(fx_slider);
		ingamesettingsMenu->elements.push_back(fx_text);
		ingamesettingsMenu->elements.push_back(apply_button);
		ingamesettingsMenu->elements.push_back(apply_text);
		ingamesettingsMenu->elements.push_back(feather);
		/*settingsMenu->elements.push_back(full_switch);
		settingsMenu->elements.push_back(fullscreen_text);*/
		menus.push_back(ingamesettingsMenu);

		AddControllerSupport(music_slider_butt, PLAYER::P1, INGAMESETTINGS_MENU);
		AddControllerSupport(fx_slider_butt, PLAYER::P1, INGAMESETTINGS_MENU);
		AddControllerSupport(apply_button, PLAYER::P1, INGAMESETTINGS_MENU);
		AddControllerSupport(back_button, PLAYER::P1, INGAMESETTINGS_MENU);
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


	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || App->input->GetButton(PLAYER::P1,SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
	{
		if (actual_menu == START_MENU)
		{
			ret = false;
		}
		else if (actual_menu == INGAME_MENU && !App->transition->doingMenuTransition)
		{
			App->on_GamePause = true;
			actual_menu = INGAMESETTINGS_MENU;

			Mix_PauseMusic();
			App->transition->menuTransition(INGAMESETTINGS_MENU, 0.3f);
			App->arena_interactions->PauseStorm();
			App->audio->PlayFx(App->audio->fxPause);
			ret = true;
		}
		else if (actual_menu == INGAMESETTINGS_MENU && previous_menu == INGAME_MENU)
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

		if (timer != nullptr)
		{
			//App->tex->UnLoad(timer->texture);

			std::list <UI_element*>::iterator item = App->gui->UI_elements.begin();

			while (item != App->gui->UI_elements.end())
			{
				if (*item == timer)
				{
					delete *item;
					App->gui->UI_elements.erase(item);
					break;
				}
				item++;
			}
		}
			//timer
			timer = App->gui->createText(App->arena_interactions->time_for_timer.data(), 496, 372, small_font, { 0,0,0,1 }, this);
			//ingameMenu->elements.push_back(timer);
			
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

	//SELECTION CHAMPION LOGIC
	if(passing1 && player1_select == false)
	{
		
		if (counter1 == 1)
		{
			if (App->scene->player1->character != CHARACTER::WENDOLIN)
			{
				photos[0]->section = { 698, 384, 142, 153 };
				App->scene->player1->character = CHARACTER::WENDOLIN;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 == 2)
		{
			if (App->scene->player1->character != CHARACTER::SIMON)
			{
				photos[0]->section = { 862, 384, 142, 153 };
				App->scene->player1->character = CHARACTER::SIMON;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 == 3)
		{
			if (App->scene->player1->character != CHARACTER::TRAKT)
			{
				photos[0]->section = { 698, 556, 142, 153 };
				App->scene->player1->character = CHARACTER::TRAKT;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 == 4)
		{
			if (App->scene->player1->character != CHARACTER::MELIADOUL)
			{
				photos[0]->section = { 862, 556, 142, 153 };
				App->scene->player1->character = CHARACTER::MELIADOUL;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 > 4)//again to wendolin
		{
			counter1 = 1;
		}

		if (counter1 == 0)
		{
			if (App->scene->player1->character != CHARACTER::MELIADOUL)
			{
				photos[0]->section = { 862, 556, 142, 153 };
				App->scene->player1->character = CHARACTER::MELIADOUL;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 == -1)
		{
			if (App->scene->player1->character != CHARACTER::TRAKT)
			{
				photos[0]->section = { 698, 556, 142, 153 };
				App->scene->player1->character = CHARACTER::TRAKT;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 == -2)
		{
			if (App->scene->player1->character != CHARACTER::SIMON)
			{
				photos[0]->section = { 862, 384, 142, 153 };
				App->scene->player1->character = CHARACTER::SIMON;
				App->scene->player1->AssignCharacter();
			}
		}

		if (counter1 < -2)//again to wendolin
		{
			counter1 = 1;
		}
	
		//IF PLAYER1 INPUT A GAMEPAD, passing1 = false && player1_select = true
		
		
	}

	if (passing2 && player2_select == false)
	{
		

		if (counter2 == 1)
		{
			if (App->scene->player2->character != CHARACTER::WENDOLIN)
			{
				photos[1]->section = { 698, 384, 142, 153 };
				App->scene->player2->character = CHARACTER::WENDOLIN;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 == 2)
		{
			if (App->scene->player2->character != CHARACTER::SIMON)
			{
				photos[1]->section = { 862, 384, 142, 153 };
				App->scene->player2->character = CHARACTER::SIMON;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 == 3)
		{
			if (App->scene->player2->character != CHARACTER::TRAKT)
			{
				photos[1]->section = { 698, 556, 142, 153 };
				App->scene->player2->character = CHARACTER::TRAKT;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 == 4)
		{
			if (App->scene->player2->character != CHARACTER::MELIADOUL)
			{
				photos[1]->section = { 862, 556, 142, 153 };
				App->scene->player2->character = CHARACTER::MELIADOUL;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 > 4)//again to wendolin
		{
			counter2 = 1;
		}

		if (counter2 == 0)
		{
			if (App->scene->player2->character != CHARACTER::MELIADOUL)
			{
				photos[1]->section = { 862, 556, 142, 153 };
				App->scene->player2->character = CHARACTER::MELIADOUL;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 == -1)
		{
			if (App->scene->player2->character != CHARACTER::TRAKT)
			{
				photos[1]->section = { 698, 556, 142, 153 };
				App->scene->player2->character = CHARACTER::TRAKT;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 == -2)
		{
			if (App->scene->player2->character != CHARACTER::SIMON)
			{
				photos[1]->section = { 862, 384, 142, 153 };
				App->scene->player2->character = CHARACTER::SIMON;
				App->scene->player2->AssignCharacter();
			}
		}

		if (counter2 < -2)//again to wendolin
		{
			counter2 = 1;
		}

		//IF PLAYER2 INPUT A GAMEPAD, passing2 = false && player2_select = true


	}

	if (passing3 && player3_select == false)
	{
		

		if (counter3 == 1)
		{
			if (App->scene->player3->character != CHARACTER::WENDOLIN)
			{
				photos[2]->section = { 698, 384, 142, 153 };
				App->scene->player3->character = CHARACTER::WENDOLIN;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 == 2)
		{
			if (App->scene->player3->character != CHARACTER::SIMON)
			{
				photos[2]->section = { 862, 384, 142, 153 };
				App->scene->player3->character = CHARACTER::SIMON;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 == 3)
		{
			if (App->scene->player3->character != CHARACTER::TRAKT)
			{
				photos[2]->section = { 698, 556, 142, 153 };
				App->scene->player3->character = CHARACTER::TRAKT;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 == 4)
		{
			if (App->scene->player3->character != CHARACTER::MELIADOUL)
			{
				photos[2]->section = { 862, 556, 142, 153 };
				App->scene->player3->character = CHARACTER::MELIADOUL;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 > 4)//again to wendolin
		{
			counter3 = 1;
		}

		if (counter3 == 0)
		{
			if (App->scene->player3->character != CHARACTER::MELIADOUL)
			{
				photos[2]->section = { 862, 556, 142, 153 };
				App->scene->player3->character = CHARACTER::MELIADOUL;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 == -1)
		{
			if (App->scene->player3->character != CHARACTER::TRAKT)
			{
				photos[2]->section = { 698, 556, 142, 153 };
				App->scene->player3->character = CHARACTER::TRAKT;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 == -2)
		{
			if (App->scene->player3->character != CHARACTER::SIMON)
			{
				photos[2]->section = { 862, 384, 142, 153 };
				App->scene->player3->character = CHARACTER::SIMON;
				App->scene->player3->AssignCharacter();
			}
		}

		if (counter3 < -2)//again to wendolin
		{
			counter3 = 1;
		}

		//IF PLAYER3 INPUT A GAMEPAD, passing3 = false && player3_select = true


	}

	if (passing4 && player4_select == false)
	{
		

		if (counter4 == 1)
		{
			if (App->scene->player4->character != CHARACTER::WENDOLIN)
			{
				photos[3]->section = { 698, 384, 142, 153 };
				App->scene->player4->character = CHARACTER::WENDOLIN;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 == 2)
		{
			if (App->scene->player4->character != CHARACTER::SIMON)
			{
				photos[3]->section = { 862, 384, 142, 153 };
				App->scene->player4->character = CHARACTER::SIMON;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 == 3)
		{
			if (App->scene->player4->character != CHARACTER::TRAKT)
			{
				photos[3]->section = { 698, 556, 142, 153 };
				App->scene->player4->character = CHARACTER::TRAKT;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 == 4)
		{
			if (App->scene->player4->character != CHARACTER::MELIADOUL)
			{
				photos[3]->section = { 862, 556, 142, 153 };
				App->scene->player4->character = CHARACTER::MELIADOUL;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 > 4)//again to wendolin
		{
			counter4 = 1;
		}

		if (counter4 == 0)
		{
			if (App->scene->player4->character != CHARACTER::MELIADOUL)
			{
				photos[3]->section = { 862, 556, 142, 153 };
				App->scene->player4->character = CHARACTER::MELIADOUL;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 == -1)
		{
			if (App->scene->player4->character != CHARACTER::TRAKT)
			{
				photos[3]->section = { 698, 556, 142, 153 };
				App->scene->player4->character = CHARACTER::TRAKT;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 == -2)
		{
			if (App->scene->player4->character != CHARACTER::SIMON)
			{
				photos[2]->section = { 862, 384, 142, 153 };
				App->scene->player4->character = CHARACTER::SIMON;
				App->scene->player4->AssignCharacter();
			}
		}

		if (counter4 < -2)//again to wendolin
		{
			counter4 = 1;
		}
		//IF PLAYER4 INPUT A GAMEPAD, passing4 = false && player4_select = true


	}


	photo_back_up1 = photos[0]->section;
	photo_back_up2 = photos[1]->section;
	photo_back_up3 = photos[2]->section;
	photo_back_up4 = photos[3]->section;

	//if (champ_selected[0] && passing2 && passing3 && passing4)//JUST BY NOW, WHEN PLAYERS CAN CONFIRM CHANGE FOR PLAYER_SELECT
	//{
	//	ready->function = INGAME;
	//}

	//MARKS RESET AND LOGIC
	if (marks_reset == false)
	{
		player1_quad->section = { 288, 518, 170,191 };
		player2_quad->section = { 288, 518, 170,191 };
		player3_quad->section = { 288, 518, 170,191 };
		player4_quad->section = { 288, 518, 170,191 };
	}



	//Champion selection locking
	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			ready->function = INGAME;
			ready->callback->OnUIEvent(ready, MOUSE_LEFT_CLICK);
			for (int j = 0; j < MAX_GAMEPADS; ++j)
			{
				champ_selected[j] = false;
			}
		}
		
		if (champ_selected[i] == false)
			break;
		
		if (i == MAX_GAMEPADS - 1 && current_menu->id == SELECTION_MENU)
		{
			ready->function = INGAME;
			ready->callback->OnUIEvent(ready, MOUSE_LEFT_CLICK);
			for (int j = 0; j < MAX_GAMEPADS; ++j)
			{
				champ_selected[j] = false;
			}
		}

	}
	
	if (current_menu->id == INGAMESETTINGS_MENU)
	{
		for (int i = 0; i < MAX_GAMEPADS; ++i)
		{
			if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_START) == BUTTON_DOWN)
						App->input->ForceKeyboardKeyState(SDL_SCANCODE_ESCAPE, KEY_DOWN);
		}
	}

	if (player1lock == true && current_menu->id == FINAL_MENU)
	{
		player1_quads->section = { 288, 842, 169, 191 };
	}
	if (player2lock == true && current_menu->id == FINAL_MENU)
	{
		player2_quads->section = { 288, 842, 169, 191 };
	}
	if (player3lock == true && current_menu->id == FINAL_MENU)
	{
		player3_quads->section = { 288, 842, 169, 191 };
	}
	if (player4lock == true && current_menu->id == FINAL_MENU)
	{
		player4_quads->section = { 288, 842, 169, 191 };
	}
	if (player1lock == true && player2lock == true && player3lock == true && player4lock == true && current_menu->id == FINAL_MENU)
	{
		if(rounds >= 4)
			ready->function = RESTART;
		else if(rounds < 3)
			ready->function = INGAME_NEW_GAME;

		ready->callback->OnUIEvent(ready, MOUSE_LEFT_CLICK);
		player1lock = false;
		player2lock = false;
		player3lock = false;
		player4lock = false;

	}

	//GET TO SCOREBOARD SCREEN

	player_winner = App->scene->GetWinner();
	if (rounds >= 3)
	{
		if (player_winner != nullptr && scoreboard == false)
		{
			player_winner->active = false;
			scoreboard = true;
			App->audio->PlayMusic(App->audio->pathLeaderBoard.data(), 0);
			actual_menu = FINAL_MENU;
			App->transition->menuTransition(FINAL_MENU, 3.0);
			if (player_winner == App->scene->player1)
				CreateFinalScoreBoard(1);
			else if (player_winner == App->scene->player2)
				CreateFinalScoreBoard(2);
			else if (player_winner == App->scene->player3)
				CreateFinalScoreBoard(3);
			else if (player_winner == App->scene->player4)
				CreateFinalScoreBoard(4);
			//If we finished, we need to destroy the storm, or the storm will affect entities as if it resumed
			//from the previous game when we play again
			App->arena_interactions->DestroyStorm();
			App->arena_interactions->PauseStorm();
			rounds = 0;
		}
	}
	else if (rounds < 3)
	{
		if (player_winner != nullptr && scoreboard == false)
		{
			player_winner->active = false;
			scoreboard = true;
			App->audio->PlayMusic(App->audio->pathLeaderBoard.data(), 0);
			actual_menu = FINAL_MENU;
			App->transition->menuTransition(FINAL_MENU, 3.0);
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
			counter1 = 1;
			counter2 = 1;
			counter3 = 1;
			counter4 = 1;
			player1_select = false;
			player2_select = false;
			player3_select = false;
			player4_select = false;
			marks_reset = true;

			//Reset Stars
			P1stars = 0;
			P2stars = 0;
			P3stars = 0;
			P4stars = 0;

			//Reset rounds
			rounds = 3;

			// --- Reset everything ---
			App->scene->ResetAll();
			
			scoreboard = false;

			actual_menu = SELECTION_MENU;
			App->transition->menuTransition(SELECTION_MENU, 0.3);

			App->scene->ChangeMap(1);
			App->view->SetViews(1);

			App->scene->player1->Future_position.x = 510;
			App->scene->player1->Future_position.y = 200;
			App->scene->player1->superTimer.Start();
			App->scene->player1->shieldON = false;
			App->scene->player1->shieldTimer.Start();

			App->scene->player2->Future_position.x = 840;
			App->scene->player2->Future_position.y = 200;
			App->scene->player2->superTimer.Start();
			App->scene->player2->shieldON = false;
			App->scene->player2->shieldTimer.Start();

			App->scene->player3->Future_position.x = 510;
			App->scene->player3->Future_position.y = 550;
			App->scene->player3->superTimer.Start();
			App->scene->player3->shieldON = false;
			App->scene->player3->shieldTimer.Start();

			App->scene->player4->Future_position.x = 840;
			App->scene->player4->Future_position.y = 550;
			App->scene->player4->superTimer.Start();
			App->scene->player4->shieldON = false;
			App->scene->player4->shieldTimer.Start();


			App->audio->PlayMusic(App->audio->pathChampSelect.data(), 0);
			break;
		}
		case INGAME:
		{		
			

			if (actual_menu == SELECTION_MENU)
			{
				App->view->SetViews(4);
				App->scene->ChangeMap(0);
				App->scene->player1->Future_position.x = App->scene->initialposP1.x;
				App->scene->player1->Future_position.y = App->scene->initialposP1.y;
				App->scene->player1->superTimer.Start();
				App->scene->player1->shieldON = false;
				App->scene->player1->shieldTimer.Start();

				App->scene->player2->Future_position.x = App->scene->initialposP2.x;
				App->scene->player2->Future_position.y = App->scene->initialposP2.y;
				App->scene->player2->superTimer.Start();
				App->scene->player2->shieldON = false;
				App->scene->player2->shieldTimer.Start();

				App->scene->player3->Future_position.x = App->scene->initialposP3.x;
				App->scene->player3->Future_position.y = App->scene->initialposP3.y;
				App->scene->player3->superTimer.Start();
				App->scene->player3->shieldON = false;
				App->scene->player3->shieldTimer.Start();

				App->scene->player4->Future_position.x = App->scene->initialposP4.x;
				App->scene->player4->Future_position.y = App->scene->initialposP4.y;
				App->scene->player4->superTimer.Start();
				App->scene->player4->shieldON = false;
				App->scene->player4->shieldTimer.Start();

				App->particlesys->CleanUp();
			}

			actual_menu = INGAME_MENU;
			App->transition->menuTransition(INGAME_MENU, 1.5);
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
		case INGAME_NEW_GAME:
		{
			actual_menu = INGAME_MENU;
			App->transition->menuTransition(INGAME_MENU, 0.3);

			App->audio->PlayFx(App->audio->fxConfirm);
			App->audio->PlayFx(App->audio->fxBrawlStart);
			App->arena_interactions->StartStorm();
			App->arena_interactions->ContinueStorm();
			App->scene->ResetAll();
			scoreboard = false;
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
		case INV_SELECTING1:
		{
			passing1 = true;
			counter1--;

			break;
		}
		case INV_SELECTING2:
		{
			passing2 = true;
			counter2--;

			break;
		}
		case INV_SELECTING3:
		{
			passing3 = true;
			counter3--;

			break;
		}
		case INV_SELECTING4:
		{
			passing4 = true;
			counter4--;

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

			if (actual_menu == INGAMESETTINGS_MENU && previous_menu == INGAME_MENU)
			{
				App->on_GamePause = false;
				marks_reset = false;
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

			App->RequestBrowser("https://checkthedog.github.io/Fantasy-Brawl/");
			App->audio->PlayFx(App->audio->fxConfirm);
			break;
		case Last_Button_1:

			player1lock = true;			
			break;
		case Last_Button_2:

			player2lock = true;
			break;
		case Last_Button_3:

			player3lock = true;	
			break;
		case Last_Button_4:

			player4lock = true;
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

				if (music_slider != tmp)
				{
					music_slider->progress = newValues.music;
					music_slider->button->localPosition.x = ((music_slider->section.w * App->gui->UI_scale) - 5 - music_slider->button->section.w / (2 / App->gui->UI_scale)) * music_slider->progress;
				}
				else if (music_sliderMM != tmp)
				{
					music_sliderMM->progress = newValues.music;
					music_sliderMM->button->localPosition.x = ((music_sliderMM->section.w * App->gui->UI_scale) - 5 - music_sliderMM->button->section.w / (2 / App->gui->UI_scale)) * music_sliderMM->progress;
				}

				tmp->button->localPosition.x = ((tmp->section.w * App->gui->UI_scale) - 5 - tmp->button->section.w / (2 / App->gui->UI_scale)) * tmp->progress;
				break;
			case FX:
				newValues.fx = tmp->progress;

				if (fx_slider != tmp)
				{
					fx_slider->progress = newValues.fx;
					fx_slider->button->localPosition.x = ((fx_slider->section.w * App->gui->UI_scale) - 5 - fx_slider->button->section.w / (2 / App->gui->UI_scale)) * fx_slider->progress;
				}
				else if (fx_sliderMM != tmp)
				{
					fx_sliderMM->progress = newValues.fx;
					fx_sliderMM->button->localPosition.x = ((fx_sliderMM->section.w * App->gui->UI_scale) - 5 - fx_sliderMM->button->section.w / (2 / App->gui->UI_scale)) * fx_sliderMM->progress;
				}

				tmp->button->localPosition.x = ((tmp->section.w * App->gui->UI_scale) - 5 - tmp->button->section.w / (2 / App->gui->UI_scale)) * tmp->progress;
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

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		current_menu->gamepads_focus[i] = current_menu->gamepad_tabs[i].begin();
	}
	
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

					if ((*item2)->element_type == SLIDER)
					{
						/*Slider* slider = (Slider*)*item2;
						switch (slider->modify)
						{
						case MUSIC:
							if (music_slider == slider)
								music_slider->progress = music_sliderMM->progress;
							
							else if (music_sliderMM == slider)
								music_sliderMM->progress = music_slider->progress;
							

							slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
							break;
						case FX:
							if (fx_slider == slider)
								fx_slider->progress = fx_sliderMM->progress;
							else if (fx_sliderMM == slider)
								fx_sliderMM->progress = fx_slider->progress;

							slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
							break;


						}*/
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

	App->audio->setMusicVolume(values.music);
	App->audio->setFxVolume(values.fx);

	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == SLIDER)
		{
			Slider* slider = (Slider*)*item;

			switch (slider->modify)
			{
			case MUSIC:

				slider->setProgress(values.music);

				if (music_slider != slider)
				{
					music_slider->setProgress(values.music);
					music_slider->button->localPosition.x = ((music_slider->section.w * App->gui->UI_scale) - 5 - music_slider->button->section.w / (2 / App->gui->UI_scale)) * music_slider->progress;
				}
				else if (music_sliderMM != slider)
				{
					music_sliderMM->setProgress(values.music);
					music_sliderMM->button->localPosition.x = ((music_sliderMM->section.w * App->gui->UI_scale) - 5 - music_sliderMM->button->section.w / (2 / App->gui->UI_scale)) * music_sliderMM->progress;
				}

				slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
				break;
			case FX:

				slider->setProgress(values.fx);

				if (fx_slider != slider)
				{
					fx_slider->setProgress(values.fx);
					fx_slider->button->localPosition.x = ((fx_slider->section.w * App->gui->UI_scale) - 5 - fx_slider->button->section.w / (2 / App->gui->UI_scale)) * fx_slider->progress;

				}
				else if (fx_sliderMM != slider)
				{
					fx_sliderMM->setProgress(values.fx);
					fx_sliderMM->button->localPosition.x = ((fx_sliderMM->section.w * App->gui->UI_scale) - 5 - fx_sliderMM->button->section.w / (2 / App->gui->UI_scale)) * fx_sliderMM->progress;

				}

				slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
				break;
			}
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

	P1stars = P1stars + App->scene->player1->kills;
	P2stars = P2stars + App->scene->player2->kills;
	P3stars = P3stars + App->scene->player3->kills;
	P4stars = P4stars + App->scene->player4->kills;
	if (App->scene->GetWinner() == App->scene->player1)
	{
		P1stars += 3;
	}
	else if (App->scene->GetWinner() == App->scene->player2)
	{
		P2stars += 3;
	}
	else if (App->scene->GetWinner() == App->scene->player3)
	{
		P3stars += 3;
	}
	else if (App->scene->GetWinner() == App->scene->player4)
	{
		P4stars += 3;
	}
	//WINDOW

	UI_element* final_image = App->gui->createImage(0, 0, App->tex->Load("gui/MapPrev.png"), this);

	//Text
	UI_element* text1 = App->gui->createText("Press      to continue to next round", 432, 670, small_font, {0, 0, 0, 1});
	text1->setOutlined(false);

	UI_element* text2 = App->gui->createText("Player Spawns", 482, 620, small_font, { 0, 0, 0, 1 });
	text1->setOutlined(false);

	//Stars
	UI_element* stars1 = App->gui->createImageFromAtlas(215, 48, { 0, 874, 55, 54 }, this);
	UI_element* stars2 = App->gui->createImageFromAtlas(215, 242, { 0, 874, 55, 54 }, this);
	UI_element* stars3 = App->gui->createImageFromAtlas(215, 433, { 0, 874, 55, 54 }, this);
	UI_element* stars4 = App->gui->createImageFromAtlas(215, 614, { 0, 874, 55, 54 }, this);

	//PLAYER QUADS No Butt
	player1_quads = App->gui->createImageFromAtlas(20, 3, { 288, 518, 170,191 }, this);
	player2_quads = App->gui->createImageFromAtlas(20, 194, { 288, 518, 170,191 }, this);
	player3_quads = App->gui->createImageFromAtlas(20, 385, { 288, 518, 170,191 }, this);
	player4_quads = App->gui->createImageFromAtlas(20, 576, { 288, 518, 170,191 }, this);

	//PLAYER TEXTS
	UI_element* player1_text = App->gui->createImageFromAtlas(40, 166, { 296, 799, 117, 20 }, this);
	UI_element* player2_text = App->gui->createImageFromAtlas(40, 357, { 413, 799,120, 20 }, this);
	UI_element* player3_text = App->gui->createImageFromAtlas(40, 548, { 534, 799,120, 20 }, this);
	UI_element* player4_text = App->gui->createImageFromAtlas(40, 739, { 654, 799,120, 20 }, this);

	//PlayerStars
	star1 = std::to_string(P1stars);
	star2 = std::to_string(P2stars);
	star3 = std::to_string(P3stars);
	star4 = std::to_string(P4stars);

	UI_element* player1_stars = App->gui->createText(star1.data(), 235, 118, small_font, { 0, 0, 0, 1 });
	UI_element* player2_stars = App->gui->createText(star2.data(), 235, 312, small_font, { 0, 0, 0, 1 });
	UI_element* player3_stars = App->gui->createText(star3.data(), 235, 503, small_font, { 0, 0, 0, 1 });
	UI_element* player4_stars = App->gui->createText(star4.data(), 235, 684, small_font, { 0, 0, 0, 1 });

	//PLAYER IMAGE(?)
	mark1 = App->gui->createImageFromAtlas(38, 15, photo_back_up1, this);
	photos[0] = mark1;

	mark2 = App->gui->createImageFromAtlas(38, 206, photo_back_up2, this);
	photos[1] = mark2;

	mark3 = App->gui->createImageFromAtlas(38, 397, photo_back_up3, this);
	photos[2] = mark3;

	mark4 = App->gui->createImageFromAtlas(38, 588, photo_back_up4, this);
	photos[3] = mark4;

	//END BUTTON
	UI_element* last_button1 = App->gui->createButton(522,664, A_Butt, { 0,0,36,40 }, { 0,0,36,40 }, { 0,44,36,40 }, this);
	last_button1->function = Last_Button_1;

	UI_element* last_button2 = App->gui->createButton(522, 664, A_Butt, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, this);
	last_button2->function = Last_Button_2;

	UI_element* last_button3 = App->gui->createButton(522, 664, A_Butt, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, this);
	last_button3->function = Last_Button_3;

	UI_element* last_button4 = App->gui->createButton(522, 664, A_Butt, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, this);
	last_button4->function = Last_Button_4;

	
	finalMenu->elements.push_back(final_image);
	
	finalMenu->elements.push_back(mark1);
	finalMenu->elements.push_back(mark2);
	finalMenu->elements.push_back(mark3);
	finalMenu->elements.push_back(mark4);
	finalMenu->elements.push_back(player1_quads);
	finalMenu->elements.push_back(player2_quads);
	finalMenu->elements.push_back(player3_quads);
	finalMenu->elements.push_back(player4_quads);
	finalMenu->elements.push_back(player1_text);
	finalMenu->elements.push_back(player2_text);
	finalMenu->elements.push_back(player3_text);
	finalMenu->elements.push_back(player4_text);
	finalMenu->elements.push_back(text1);
	finalMenu->elements.push_back(text2);
	finalMenu->elements.push_back(last_button1);
	finalMenu->elements.push_back(last_button2);
	finalMenu->elements.push_back(last_button3);
	finalMenu->elements.push_back(last_button4);
	finalMenu->elements.push_back(stars1);
	finalMenu->elements.push_back(stars2);
	finalMenu->elements.push_back(stars3);
	finalMenu->elements.push_back(stars4);
	finalMenu->elements.push_back(player1_stars);
	finalMenu->elements.push_back(player2_stars);
	finalMenu->elements.push_back(player3_stars);
	finalMenu->elements.push_back(player4_stars);

	if (finalMenu->gamepad_tabs[0].empty() == true)
		AddControllerSupport(last_button1, PLAYER::P1, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[0].clear();
		AddControllerSupport(last_button1, PLAYER::P1, FINAL_MENU);
	}

	if (finalMenu->gamepad_tabs[1].empty() == true)
		AddControllerSupport(last_button2, PLAYER::P2, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[1].clear();
		AddControllerSupport(last_button2, PLAYER::P2, FINAL_MENU);
	}

	if (finalMenu->gamepad_tabs[2].empty() == true)
		AddControllerSupport(last_button3, PLAYER::P3, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[2].clear();
		AddControllerSupport(last_button3, PLAYER::P3, FINAL_MENU);
	}

	if (finalMenu->gamepad_tabs[3].empty() == true)
		AddControllerSupport(last_button4, PLAYER::P4, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[3].clear();
		AddControllerSupport(last_button4, PLAYER::P4, FINAL_MENU);
	}

	rounds++;


	LOG("%i", finalMenu->elements.size());

}

void j1UIScene::CreateFinalScoreBoard(int num)
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

	P1stars = P1stars + App->scene->player1->kills;
	P2stars = P2stars + App->scene->player2->kills;
	P3stars = P3stars + App->scene->player3->kills;
	P4stars = P4stars + App->scene->player4->kills;
	if (App->scene->GetWinner() == App->scene->player1)
	{
		P1stars += 3;
	}
	else if (App->scene->GetWinner() == App->scene->player2)
	{
		P2stars += 3;
	}
	else if (App->scene->GetWinner() == App->scene->player3)
	{
		P3stars += 3;
	}
	else if (App->scene->GetWinner() == App->scene->player4)
	{
		P4stars += 3;
	}

	//WINDOW

	UI_element* final_image = App->gui->createImage(0, 0, App->tex->Load("gui/Bg.png"), this);

	//Text
	UI_element* text1 = App->gui->createText("Winner", 347, 100, big_font, { 0, 0, 0, 1 });
	text1->setOutlined(false);

	UI_element* text2 = nullptr;

	if (P1stars > P2stars && P1stars > P3stars && P1stars > P4stars)
	{
		text2 = App->gui->createText("Player 1", 615, 100, big_font, { 0, 0, 0, 1 });
		text1->setOutlined(false);
	}
	else if (P2stars > P1stars && P2stars > P3stars && P2stars > P4stars)
	{
		text2 = App->gui->createText("Player 2", 615, 100, big_font, { 0, 0, 0, 1 });
		text1->setOutlined(false);
	}
	else if (P3stars > P2stars && P3stars > P1stars && P3stars > P4stars)
	{
		text2 = App->gui->createText("Player 3", 615, 100, big_font, { 0, 0, 0, 1 });
		text1->setOutlined(false);
	}
	else if (P4stars > P2stars && P4stars > P3stars && P4stars > P1stars)
	{
		text2 = App->gui->createText("Player 4", 615, 100, big_font, { 0, 0, 0, 1 });
		text1->setOutlined(false);
	}
	
	UI_element* text3 = App->gui->createText("Press      to continue to next round", 357, 670, small_font, { 0, 0, 0, 1 });
	text1->setOutlined(false);

	//PLAYER QUADS No Butt
	player1_quads = App->gui->createImageFromAtlas(20, 3, { 288, 518, 170,191 }, this);
	player2_quads = App->gui->createImageFromAtlas(20, 194, { 288, 518, 170,191 }, this);
	player3_quads = App->gui->createImageFromAtlas(20, 385, { 288, 518, 170,191 }, this);
	player4_quads = App->gui->createImageFromAtlas(20, 576, { 288, 518, 170,191 }, this);


	//PLAYER TEXTS
	UI_element* player1_text = App->gui->createImageFromAtlas(40, 166, { 296, 799, 117, 20 }, this);
	UI_element* player2_text = App->gui->createImageFromAtlas(40, 357, { 413, 799,120, 20 }, this);
	UI_element* player3_text = App->gui->createImageFromAtlas(40, 548, { 534, 799,120, 20 }, this);
	UI_element* player4_text = App->gui->createImageFromAtlas(40, 739, { 654, 799,120, 20 }, this);

	//Stars
	UI_element* stars1 = App->gui->createImageFromAtlas(215, 48, { 0, 874, 55, 54 }, this);
	UI_element* stars2 = App->gui->createImageFromAtlas(215, 242, { 0, 874, 55, 54 }, this);
	UI_element* stars3 = App->gui->createImageFromAtlas(215, 433, { 0, 874, 55, 54 }, this);
	UI_element* stars4 = App->gui->createImageFromAtlas(215, 614, { 0, 874, 55, 54 }, this);

	//PlayerStars
	star1 = std::to_string(P1stars);
	star2 = std::to_string(P2stars);
	star3 = std::to_string(P3stars);
	star4 = std::to_string(P4stars);

	UI_element* player1_stars = App->gui->createText(star1.data(), 235, 118, small_font, { 0, 0, 0, 1 });
	UI_element* player2_stars = App->gui->createText(star2.data(), 235, 312, small_font, { 0, 0, 0, 1 });
	UI_element* player3_stars = App->gui->createText(star3.data(), 235, 503, small_font, { 0, 0, 0, 1 });
	UI_element* player4_stars = App->gui->createText(star4.data(), 235, 684, small_font, { 0, 0, 0, 1 });


	//PLAYER IMAGE(?)
	mark1 = App->gui->createImageFromAtlas(38, 15, photo_back_up1, this);
	photos[0] = mark1;

	mark2 = App->gui->createImageFromAtlas(38, 206, photo_back_up2, this);
	photos[1] = mark2;

	mark3 = App->gui->createImageFromAtlas(38, 397, photo_back_up3, this);
	photos[2] = mark3;

	mark4 = App->gui->createImageFromAtlas(38, 588, photo_back_up4, this);
	photos[3] = mark4;

	//END BUTTON
	UI_element* last_button1 = App->gui->createButton(522, 664, A_Butt, { 0,0,36,40 }, { 0,0,36,40 }, { 0,44,36,40 }, this);
	last_button1->function = Last_Button_1;

	UI_element* last_button2 = App->gui->createButton(522, 664, A_Butt, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, this);
	last_button2->function = Last_Button_2;

	UI_element* last_button3 = App->gui->createButton(522, 664, A_Butt, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, this);
	last_button3->function = Last_Button_3;

	UI_element* last_button4 = App->gui->createButton(522, 664, A_Butt, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, this);
	last_button4->function = Last_Button_4;


	
	finalMenu->elements.push_back(final_image);
	finalMenu->elements.push_back(mark1);
	finalMenu->elements.push_back(mark2);
	finalMenu->elements.push_back(mark3);
	finalMenu->elements.push_back(mark4);
	finalMenu->elements.push_back(player1_quad);
	finalMenu->elements.push_back(player2_quad);
	finalMenu->elements.push_back(player3_quad);
	finalMenu->elements.push_back(player4_quad);
	finalMenu->elements.push_back(player1_text);
	finalMenu->elements.push_back(player2_text);
	finalMenu->elements.push_back(player3_text);
	finalMenu->elements.push_back(player4_text);
	finalMenu->elements.push_back(text1);
	finalMenu->elements.push_back(text2);
	finalMenu->elements.push_back(text3);
	finalMenu->elements.push_back(last_button1);
	finalMenu->elements.push_back(last_button2);
	finalMenu->elements.push_back(last_button3);
	finalMenu->elements.push_back(last_button4);
	finalMenu->elements.push_back(stars1);
	finalMenu->elements.push_back(stars2);
	finalMenu->elements.push_back(stars3);
	finalMenu->elements.push_back(stars4);
	finalMenu->elements.push_back(player1_stars);
	finalMenu->elements.push_back(player2_stars);
	finalMenu->elements.push_back(player3_stars);
	finalMenu->elements.push_back(player4_stars);

	if (finalMenu->gamepad_tabs[0].empty() == true)
		AddControllerSupport(last_button1, PLAYER::P1, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[0].clear();
		AddControllerSupport(last_button1, PLAYER::P1, FINAL_MENU);
	}

	if (finalMenu->gamepad_tabs[1].empty() == true)
		AddControllerSupport(last_button2, PLAYER::P2, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[1].clear();
		AddControllerSupport(last_button2, PLAYER::P2, FINAL_MENU);
	}

	if (finalMenu->gamepad_tabs[2].empty() == true)
		AddControllerSupport(last_button3, PLAYER::P3, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[2].clear();
		AddControllerSupport(last_button3, PLAYER::P3, FINAL_MENU);
	}

	if (finalMenu->gamepad_tabs[3].empty() == true)
		AddControllerSupport(last_button4, PLAYER::P4, FINAL_MENU);
	else
	{
		finalMenu->gamepad_tabs[3].clear();
		AddControllerSupport(last_button4, PLAYER::P4, FINAL_MENU);
	}

	rounds++;

	LOG("%i", finalMenu->elements.size());

}

void j1UIScene::AddControllerSupport(UI_element* element, PLAYER gamepad_supported, menu_id id_menu)
{
	std::list<menu*>::iterator curr_menu = menus.begin();

	for (; curr_menu != menus.end(); curr_menu++)
	{
		if ((*curr_menu)->id == id_menu)
		{
			(*curr_menu)->gamepad_tabs[(int)gamepad_supported].push_back(element);

			if ((*curr_menu)->gamepad_tabs[(int)gamepad_supported].size() == 1)
			{
				(*curr_menu)->gamepads_focus[(int)gamepad_supported] = (*curr_menu)->gamepad_tabs[(int)gamepad_supported].begin();
			}

			break;
		}
	}
}


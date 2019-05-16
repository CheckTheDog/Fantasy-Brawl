#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Viewport.h"
#include "j1UIScene.h"
#include "j1BuffManager.h"
#include "j1ArenaInteractions.h"

#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node stage = config.child("map_name"); stage; stage = stage.next_sibling("map_name"))
	{
		std::string* StageName = new std::string(stage.attribute("path").as_string());
		StageList.push_back(StageName);
	}

	initialposP1.x = config.child("initialposP1").attribute("x").as_float();
	initialposP1.y = config.child("initialposP1").attribute("y").as_float();

	initialposP2.x = config.child("initialposP2").attribute("x").as_float();
	initialposP2.y = config.child("initialposP2").attribute("y").as_float();

	initialposP3.x = config.child("initialposP3").attribute("x").as_float();
	initialposP3.y = config.child("initialposP3").attribute("y").as_float();

	initialposP4.x = config.child("initialposP4").attribute("x").as_float();
	initialposP4.y = config.child("initialposP4").attribute("y").as_float();

	return ret;
}

bool j1Scene::ChangeMap(int destination_map_id)
{
	App->map->CleanUp();
	App->coll->CleanUp();

	App->map->Load(StageList.at(destination_map_id)->data());

	player1->Entityinfo.entitycoll = App->coll->AddCollider(player1->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entities);
	player1->Entityinfo.entitycoll->rect.w *= player1->Entityinfo.scale;
	player1->Entityinfo.entitycoll->rect.h *= player1->Entityinfo.scale;
	player1->Entityinfo.HitBox = App->coll->AddCollider(player1->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_HITBOX, (j1Module*)App->entities);
	player1->Entityinfo.HitBox->rect.w = 20;
	player1->Entityinfo.HitBox->rect.h = 20;

	player2->Entityinfo.entitycoll = App->coll->AddCollider(player2->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entities);
	player2->Entityinfo.entitycoll->rect.w *= player2->Entityinfo.scale;
	player2->Entityinfo.entitycoll->rect.h *= player2->Entityinfo.scale;
	player2->Entityinfo.HitBox = App->coll->AddCollider(player2->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_HITBOX, (j1Module*)App->entities);
	player2->Entityinfo.HitBox->rect.w = 20;
	player2->Entityinfo.HitBox->rect.h = 20;

	player3->Entityinfo.entitycoll = App->coll->AddCollider(player3->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entities);
	player3->Entityinfo.entitycoll->rect.w *= player3->Entityinfo.scale;
	player3->Entityinfo.entitycoll->rect.h *= player3->Entityinfo.scale;
	player3->Entityinfo.HitBox = App->coll->AddCollider(player3->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_HITBOX, (j1Module*)App->entities);
	player3->Entityinfo.HitBox->rect.w = 20;
	player3->Entityinfo.HitBox->rect.h = 20;

	player4->Entityinfo.entitycoll = App->coll->AddCollider(player4->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entities);
	player4->Entityinfo.entitycoll->rect.w *= player4->Entityinfo.scale;
	player4->Entityinfo.entitycoll->rect.h *= player4->Entityinfo.scale;
	player4->Entityinfo.HitBox = App->coll->AddCollider(player4->Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_HITBOX, (j1Module*)App->entities);
	player4->Entityinfo.HitBox->rect.w = 20;
	player4->Entityinfo.HitBox->rect.h = 20;



	App->map->ColliderDrawer();

	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	// --- Loading map ---

	App->map->Load(StageList.front()->data());

	//debug_tex = App->tex->Load("maps/path2.png");
	App->map->ColliderDrawer();

	// --- Creating entities ---

	entity_info player_info;

	player_info.scale = 0.66f;

	player_info.IDCircle = App->entities->IDCircle_red;
	player_info.IDCirclesuper = App->entities->IDCirclesuper_red;
	player_info.IDCircleshield = App->entities->IDCircleshield_red;
	player_info.IDCircleboth = App->entities->IDCircleboth_red;

	player_info.position = initialposP1;
	player1 = (j1Player*)App->entities->CreateEntity(entity_type::PLAYER, player_info, &App->entities->Wendolin);
	player1->character = CHARACTER::MELIADOUL;
	player1->AssignCharacter();
	
	player_info.IDCircle = App->entities->IDCircle_blue;
	player_info.IDCirclesuper = App->entities->IDCirclesuper_blue;
	player_info.IDCircleshield = App->entities->IDCircleshield_blue;
	player_info.IDCircleboth = App->entities->IDCircleboth_blue;

	player_info.position = initialposP2;
	player2 = (j1Player*)App->entities->CreateEntity(entity_type::PLAYER, player_info, &App->entities->Simon);
	player2->character = CHARACTER::SIMON;
	player2->AssignCharacter();

	player_info.IDCircle = App->entities->IDCircle_yellow;
	player_info.IDCirclesuper = App->entities->IDCirclesuper_yellow;
	player_info.IDCircleshield = App->entities->IDCircleshield_yellow;
	player_info.IDCircleboth = App->entities->IDCircleboth_yellow;

	player_info.position = initialposP3;
	player3 = (j1Player*)App->entities->CreateEntity(entity_type::PLAYER, player_info, &App->entities->Trakt);
	player3->character = CHARACTER::TRAKT;
	player3->AssignCharacter();

	player_info.IDCircle = App->entities->IDCircle_green;
	player_info.IDCirclesuper = App->entities->IDCirclesuper_green;
	player_info.IDCircleshield = App->entities->IDCircleshield_green;
	player_info.IDCircleboth = App->entities->IDCircleboth_green;

	player_info.position = initialposP4;
	player4 = (j1Player*)App->entities->CreateEntity(entity_type::PLAYER, player_info, &App->entities->Meliadoul);
	player4->character = CHARACTER::MELIADOUL;
	player4->AssignCharacter();

	return true;
}

void j1Scene::ResetAll()
{
	// --- Activate all players ---
	player1->active = true;
	player2->active = true;
	player3->active = true;
	player4->active = true;

	// --- Reset all players attributes ---
	player1->Entityinfo.health = App->buff->GetMaxHealth();
	player2->Entityinfo.health = App->buff->GetMaxHealth();
	player3->Entityinfo.health = App->buff->GetMaxHealth();
	player4->Entityinfo.health = App->buff->GetMaxHealth();

	// --- Put players back to initial position ---
	player1->Future_position = initialposP1;
	player2->Future_position = initialposP2;
	player3->Future_position = initialposP3;
	player4->Future_position = initialposP4;

	// --- Put player's colliders back to initial position ---
	player1->Entityinfo.entitycoll->rect.x = initialposP1.x;
	player2->Entityinfo.entitycoll->rect.x = initialposP2.x;
	player3->Entityinfo.entitycoll->rect.x = initialposP3.x;
	player4->Entityinfo.entitycoll->rect.x = initialposP4.x;

	player1->Entityinfo.entitycoll->rect.y = initialposP1.y;
	player2->Entityinfo.entitycoll->rect.y = initialposP2.y;
	player3->Entityinfo.entitycoll->rect.y = initialposP3.y;
	player4->Entityinfo.entitycoll->rect.y = initialposP4.y;

	// --- Put all players to idleDown animation ---
	player1->CurrentAnimation = &player1->playerinfo.idleDown;
	player2->CurrentAnimation = &player2->playerinfo.idleDown;
	player3->CurrentAnimation = &player3->playerinfo.idleDown;
	player4->CurrentAnimation = &player4->playerinfo.idleDown;

	// --- Reset Players State and Movement status ---
	player1->EntityMovement = MOVEMENT::STATIC;
	player2->EntityMovement = MOVEMENT::STATIC;
	player3->EntityMovement = MOVEMENT::STATIC;
	player4->EntityMovement = MOVEMENT::STATIC;

	player1->PlayerState = PSTATE::IDLE;
	player2->PlayerState = PSTATE::IDLE;
	player3->PlayerState = PSTATE::IDLE;
	player4->PlayerState = PSTATE::IDLE;

	// --- Reset Players Scores ---
	player1->kills = 0;
	player2->kills = 0;
	player3->kills = 0;
	player4->kills = 0;

	player1->P_rank = RANK::CONTENDER;
	player2->P_rank = RANK::CONTENDER;
	player3->P_rank = RANK::CONTENDER;
	player4->P_rank = RANK::CONTENDER;
}

j1Player* j1Scene::GetWinner()
{
	j1Player* winner = nullptr;

	if (player1->P_rank == RANK::CONTENDER)
	{
		if (player2->P_rank == RANK::LOSER
			&& player3->P_rank == RANK::LOSER
			&& player4->P_rank == RANK::LOSER)
			winner = player1;
	}

	else if (player2->P_rank == RANK::CONTENDER)
	{
		if (player1->P_rank == RANK::LOSER
			&& player3->P_rank == RANK::LOSER
			&& player4->P_rank == RANK::LOSER)
			winner = player2;
	}

	else if (player3->P_rank == RANK::CONTENDER)
	{
		if (player1->P_rank == RANK::LOSER
			&& player2->P_rank == RANK::LOSER
			&& player4->P_rank == RANK::LOSER)
			winner = player3;
	}

	else if (player4->P_rank == RANK::CONTENDER)
	{
		if (player1->P_rank == RANK::LOSER
			&& player2->P_rank == RANK::LOSER
			&& player3->P_rank == RANK::LOSER)
			winner = player4;
	}

	return winner;
}


// Called each loop iteration
bool j1Scene::PreUpdate()
{

	BROFILER_CATEGORY("Scene_Pre_Update", Profiler::Color::BlanchedAlmond);

	// debug pathfing ------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	// --- Center Cameras on respective player ---
	if(player1->Entityinfo.position.x != 0 && player1->Entityinfo.position.y != 0)
	App->view->CenterScreen(1, player1->Entityinfo.position.x + player1->Entityinfo.entitycoll->rect.w / 2, player1->Entityinfo.position.y);
	if (player2->Entityinfo.position.x != 0 && player2->Entityinfo.position.y != 0)
	App->view->CenterScreen(2, player2->Entityinfo.position.x + player2->Entityinfo.entitycoll->rect.w / 2, player2->Entityinfo.position.y);
	if (player3->Entityinfo.position.x != 0 && player3->Entityinfo.position.y != 0)
	App->view->CenterScreen(3, player3->Entityinfo.position.x + player3->Entityinfo.entitycoll->rect.w / 2, player3->Entityinfo.position.y);
	if (player4->Entityinfo.position.x != 0 && player4->Entityinfo.position.y != 0)
	App->view->CenterScreen(4, player4->Entityinfo.position.x + player4->Entityinfo.entitycoll->rect.w / 2, player4->Entityinfo.position.y);

	// --- Prevent cameras from leaving map boundaries --- 
	App->view->KeepCameraOnBounds(1);
	App->view->KeepCameraOnBounds(2);
	App->view->KeepCameraOnBounds(3);
	App->view->KeepCameraOnBounds(4);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Crimson);

	//if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//	App->LoadGame("save_game.xml");

	//if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	//	App->SaveGame("save_game.xml");


	//Make the camera movement independent of framerate
	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) 
	//{
	//	App->view->ScreenMove(1, 0, ceil(150.0*dt));
	//	App->view->ScreenMove(2, 0, ceil(150.0*dt));
	//	App->view->ScreenMove(3, 0, ceil(150.0*dt));
	//	App->view->ScreenMove(4, 0, ceil(150.0*dt));
	//}

	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//{
	//	App->view->ScreenMove(1, 0, -ceil(150.0*dt));
	//	App->view->ScreenMove(2, 0, -ceil(150.0*dt));
	//	App->view->ScreenMove(3, 0, -ceil(150.0*dt));
	//	App->view->ScreenMove(4, 0, -ceil(150.0*dt));
	//}
	//	
	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//{
	//	App->view->ScreenMove(1, ceil(150.0*dt), 0);
	//	App->view->ScreenMove(2, ceil(150.0*dt), 0);
	//	App->view->ScreenMove(3, ceil(150.0*dt), 0);
	//	App->view->ScreenMove(4, ceil(150.0*dt), 0);
	//}
	//	
	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//{
	//	App->view->ScreenMove(1, -ceil(150.0*dt), 0);
	//	App->view->ScreenMove(2, -ceil(150.0*dt), 0);
	//	App->view->ScreenMove(3, -ceil(150.0*dt), 0);
	//	App->view->ScreenMove(4, -ceil(150.0*dt), 0);
	//}

	/*App->view->ScreenPosition(1, player1->Entityinfo.position.x + player1->Entityinfo.entitycoll->rect.w/2 - App->view., player1->Entityinfo.position.y);
	App->view->ScreenPosition(2, player2->Entityinfo.position.x, player2->Entityinfo.position.y);
	App->view->ScreenPosition(3, player3->Entityinfo.position.x, player3->Entityinfo.position.y);
	App->view->ScreenPosition(4, player4->Entityinfo.position.x, player4->Entityinfo.position.y);*/

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		if (App->view->number_of_views == 4) 
		{
			App->view->number_of_views = 1;
		}
		else
		{
			App->view->number_of_views = 4;
		}
	}

	/*if (App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT)
		App->view->ScreenMove(1, ceil(150.0*dt), 0);

	if (App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT)
		App->view->ScreenMove(1, -ceil(150.0*dt), 0);*/

	//Testing Haptic features (Vibration)
	//if (App->input->GetButton(PLAYER::P1, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
	//{
	//	App->input->ShakeController(PLAYER::P1,1.0, 3000);
	//}

	//if (App->input->GetButton(PLAYER::P1, SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN)
	//{
	//	App->input->StopControllerShake(PLAYER::P1);
	//}

	//if (App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
	//{
	//	App->input->ShakeController(PLAYER::P2, 1.0, 3000);
	//}

	//if (App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN)
	//{
	//	App->input->StopControllerShake(PLAYER::P2);
	//}

	//if (App->input->GetButton(PLAYER::P3, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
	//{
	//	App->input->ShakeController(PLAYER::P3, 1.0, 3000);
	//}

	//if (App->input->GetButton(PLAYER::P3, SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN)
	//{
	//	App->input->StopControllerShake(PLAYER::P3);
	//}

	//if (App->input->GetButton(PLAYER::P4, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
	//{
	//	App->input->ShakeController(PLAYER::P4, 1.0, 3000);
	//}

	//if (App->input->GetButton(PLAYER::P4, SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN)
	//{
	//	App->input->StopControllerShake(PLAYER::P4);
	//}
	

	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		ChangeMap(1);

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene_Post_Update", Profiler::Color::Aquamarine);

	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	std::vector<std::string*>().swap(StageList);

	return true;
}

void j1Scene::LoadLvl(int num)
{

	App->ui_scene->loadMenu(START_MENU);
}


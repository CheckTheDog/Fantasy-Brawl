#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Collision.h"

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
		//StageList.add(StageName);
		StageList.push_back(StageName);
	}

	return ret;
}

bool j1Scene::SetWalkabilityMap()
{
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	return true;
}

bool j1Scene::ChangeMap(int destination_map_id)
{
	App->map->CleanUp();
	App->coll->CleanUp();
	App->map->ColliderDrawer();

	if(App->map->Load(StageList.at(destination_map_id)->data()))
	{
		SetWalkabilityMap();
	}

	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	// --- Loading map ---

	if(App->map->Load(StageList.front()->data()) == true)
	{
		SetWalkabilityMap();
	}

	debug_tex = App->tex->Load("maps/path2.png");
	App->map->ColliderDrawer();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	BROFILER_CATEGORY("Scene_Pre_Update", Profiler::Color::BlanchedAlmond);

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene_Update", Profiler::Color::Crimson);

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		ChangeMap(1);

	//Make the camera movement independent of framerate
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += ceil(150.0*dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= ceil(150.0*dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += ceil(150.0*dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= ceil(150.0*dt);

	//Gamepad Test. Demonstration on how to use the functions for the gamepads
	if (IN_RANGE(App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTY), -40000,-10000))
		App->render->camera.y += ceil(150.0*dt);

	if (App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTY) > 10000)
		App->render->camera.y -= ceil(150.0*dt);

	if (App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT)
		App->render->camera.x += ceil(150.0*dt);

	if (App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT)
		App->render->camera.x -= ceil(150.0*dt);
	

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	/*std::string title;
	title.assign("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.size(),
					map_coordinates.x, map_coordinates.y);*/

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const std::vector<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->size(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->at(i).x, path->at(i).y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

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


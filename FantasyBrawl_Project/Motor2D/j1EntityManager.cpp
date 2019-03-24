// ----------------------------------------------------
// Controls all logic entities (enemies/player/etc.) --
// ----------------------------------------------------

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{}


// Called before render is available
bool j1EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Setting up Entity manager");
	bool ret = true;

	update_ms_cycle = 1.0f / (float)App->framerate_cap;

	LOG("Loading Player Parser");

	pugi::xml_node playernode = config.child("player");

	// --- Player Awake ---
	playerinfo.folder.assign(playernode.child("folder").child_value());
	playerinfo.Texture.assign(playernode.child("texture").child_value());

	//playerinfo.idleRight = LoadAnimation(playerinfo.folder.GetString(), "idle right");
	//playerinfo.idleLeft = LoadAnimation(playerinfo.folder.GetString(), "idle left");
	//playerinfo.runRight = LoadAnimation(playerinfo.folder.GetString(), "run right");
	//playerinfo.runLeft = LoadAnimation(playerinfo.folder.GetString(), "run left");
	//playerinfo.jumpingRight = LoadAnimation(playerinfo.folder.GetString(), "jump right");
	//playerinfo.jumpingLeft = LoadAnimation(playerinfo.folder.GetString(), "jump left");
	//playerinfo.fallingRight = LoadAnimation(playerinfo.folder.GetString(), "air right");
	//playerinfo.fallingLeft = LoadAnimation(playerinfo.folder.GetString(), "air left");
	//playerinfo.deathRight = LoadAnimation(playerinfo.folder.GetString(), "dead right");
	//playerinfo.deathLeft = LoadAnimation(playerinfo.folder.GetString(), "dead left");
	//playerinfo.airRight = LoadAnimation(playerinfo.folder.GetString(), "air right");
	//playerinfo.airLeft = LoadAnimation(playerinfo.folder.GetString(), "air left");
	//int x = playernode.child("collider").attribute("x").as_int();
	//int y = playernode.child("collider").attribute("y").as_int();
	//int w = playernode.child("collider").attribute("width").as_int();
	//int h = playernode.child("collider").attribute("height").as_int();
	//playerinfo.playerrect = { x,y,w,h };

	// --- Player main variables ---

	//playerinfo.Velocity.x = playernode.child("Velocity").attribute("x").as_float();
	//playerinfo.Velocity.y = playernode.child("Velocity").attribute("y").as_float();
	//playerinfo.gravity = playernode.child("gravity").attribute("value").as_float();
	//playerinfo.jump_force = playernode.child("Velocity").attribute("jump_force").as_float();

	//playerinfo.idleRight->speed = 10.0f;
	//playerinfo.idleLeft->speed = 10.0f;
	//playerinfo.runRight->speed = 10.0f;
	//playerinfo.runLeft->speed = 10.0f;
	//playerinfo.jumpingRight->speed = 10.0f;
	//playerinfo.jumpingLeft->speed = 10.0f;
	//playerinfo.fallingRight->speed = 10.0f;
	//playerinfo.fallingLeft->speed = 10.0f;
	//playerinfo.deathRight->speed = 10.0f;
	//playerinfo.deathLeft->speed = 10.0f;
	//playerinfo.airRight->speed = 10.0f;
	//playerinfo.airLeft->speed = 10.0f;

	//playerinfo.deathRight->loop = false;
	//playerinfo.deathLeft->loop = false;

	// ---------------------

	return ret;
}

// Called before the first frame
bool j1EntityManager::Start()
{
	LOG("start j1EntityManager");
	bool ret = true;
	return ret;
}

// Called each loop iteration
bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntityManager_Pre_Update", Profiler::Color::Chartreuse);

	//do_logic = false;
	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManager_Update", Profiler::Color::Chocolate);

	if (dt<update_ms_cycle*1.25f && dt > 0.0f)
		UpdateEntity(dt);

	return true;
}

void j1EntityManager::UpdateEntity(float dt)
{
	std::list<j1Entity*>::iterator entity = entities.begin();

	while (entity != entities.end())
	{
		(*entity)->LogicUpdate(dt);

		++entity;
	}
}

bool j1EntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("EntityManager_Post_Update", Profiler::Color::Coral);

	std::list<j1Entity*>::iterator entity = entities.begin();

	while (entity != entities.end())
	{
		(*entity)->FixedUpdate(dt);

		++entity;
	}
	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("cleanup j1EntityManager");

	// release all entities
	std::list<j1Entity*>::iterator entity = entities.begin();

	while (entity != entities.end())
	{
		(*entity)->CleanUp();
		delete *entity;
		entity++;
	}

	entities.clear();
	return true;
}

bool j1EntityManager::Load(pugi::xml_node &file)
{
	std::list <j1Entity*>::const_iterator entity = entities.begin();

	while (entity != entities.end())
	{
		(*entity)->Load(file);

		entity++;
	}

	return true;
}

bool j1EntityManager::Save(pugi::xml_node &file) const
{
	std::list <j1Entity*>::const_iterator entity = entities.begin();

	while (entity != entities.end())
	{
		(*entity)->Save(file);

		entity++;
	}

	return true;
}

// Create a new empty entity
j1Entity* const j1EntityManager::CreateEntity(entity_type entitytype, entity_info entityinfo)
{
	j1Entity* entity = nullptr;

	switch (entitytype)
	{
	case entity_type::PLAYER:
		entity = (j1Entity*) new j1Player(entityinfo);
		break;
	}

	entity->Init(this);
	entity->Start();
	entities.push_back(entity);
	return(entity);
}


void j1EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	std::list <j1Entity*>::const_iterator entity = entities.begin();

	while (entity != entities.end())
	{
		if ((*entity)->Entityinfo.entitycoll == c1)
		{
			(*entity)->OnCollision(c1, c2);
			break;
		}

		entity++;
	}
}

Animation* j1EntityManager::LoadAnimation(const char* animationPath, const char* animationName) {

	Animation* animation = new Animation();

	bool anim = false;

	pugi::xml_document animationDocument;
	pugi::xml_parse_result result = animationDocument.load_file(animationPath);


	if (result == NULL)
	{
		LOG("Issue loading animation");
	}

	pugi::xml_node objgroup;
	for (objgroup = animationDocument.child("map").child("objectgroup"); objgroup; objgroup = objgroup.next_sibling("objectgroup"))
	{
		std::string name = objgroup.attribute("name").as_string();
		if (name == animationName)
		{
			anim = true;
			int x, y, h, w;

			for (pugi::xml_node sprite = objgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				animation->PushBack({ x, y, w, h });
			}

		}
	}
	if (anim = true)
		return animation;
	else
		return nullptr;

}



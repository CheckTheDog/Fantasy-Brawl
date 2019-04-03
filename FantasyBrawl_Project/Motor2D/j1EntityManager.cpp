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

	int x = playernode.child("collider").attribute("x").as_int();
	int y = playernode.child("collider").attribute("y").as_int();
	int w = playernode.child("collider").attribute("width").as_int();
	int h = playernode.child("collider").attribute("height").as_int();

	// --- Player 1 Awake ---
	player1info.folder.assign(playernode.child("folder").child_value());
	player1info.Texture.assign(playernode.child("texture1").child_value());

	player1info.idleRight = LoadAnimation(player1info.folder.data(), "idleRight");
	player1info.idleLeft = LoadAnimation(player1info.folder.data(), "idleLeft");
	player1info.idleUp = LoadAnimation(player1info.folder.data(), "idleUp");
	player1info.idleDown = LoadAnimation(player1info.folder.data(), "idleDown");
	player1info.idleUpright = LoadAnimation(player1info.folder.data(), "idleUpright");
	player1info.idleUpleft = LoadAnimation(player1info.folder.data(), "idleUpleft");
	player1info.idleDownright = LoadAnimation(player1info.folder.data(), "idleDownright");
	player1info.idleDownleft = LoadAnimation(player1info.folder.data(), "idleDownleft");

	player1info.moveRight = LoadAnimation(player1info.folder.data(), "moveRight");
	player1info.moveLeft = LoadAnimation(player1info.folder.data(), "moveLeft");
	player1info.moveUp = LoadAnimation(player1info.folder.data(), "moveUp");
	player1info.moveDown = LoadAnimation(player1info.folder.data(), "moveDown");
	player1info.moveUpright = LoadAnimation(player1info.folder.data(), "moveUpright");
	player1info.moveUpleft = LoadAnimation(player1info.folder.data(), "moveUpleft");
	player1info.moveDownright = LoadAnimation(player1info.folder.data(), "moveDownright");
	player1info.moveDownleft = LoadAnimation(player1info.folder.data(), "moveDownleft");

	player1info.attackRight = LoadAnimation(player1info.folder.data(), "attackRight");
	player1info.attackLeft = LoadAnimation(player1info.folder.data(), "attackLeft");
	player1info.attackUp = LoadAnimation(player1info.folder.data(), "attackUp");
	player1info.attackDown = LoadAnimation(player1info.folder.data(), "attackDown");
	player1info.attackUpright = LoadAnimation(player1info.folder.data(), "attackUpright");
	player1info.attackUpleft = LoadAnimation(player1info.folder.data(), "attackUpleft");
	player1info.attackDownright = LoadAnimation(player1info.folder.data(), "attackDownright");
	player1info.attackDownleft = LoadAnimation(player1info.folder.data(), "attackDownleft");

	player1info.playerrect = { x,y,w,h };

	// --- Player 1 main variables ---

	// --------------------

	// --- Player 2,3,4 Awake... ---

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
j1Entity* const j1EntityManager::CreateEntity(entity_type entitytype, entity_info entityinfo, Playerdata * player_info)
{
	j1Entity* entity = nullptr;

	switch (entitytype)
	{
	case entity_type::PLAYER:

		entity = (j1Entity*) new j1Player(entityinfo,player_info);
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



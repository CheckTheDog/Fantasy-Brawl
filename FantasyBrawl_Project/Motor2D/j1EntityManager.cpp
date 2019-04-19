// ----------------------------------------------------
// Controls all logic entities (enemies/player/etc.) --
// ----------------------------------------------------

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "j1BuffManager.h"
#include "j1Textures.h"

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

	shield_texturepath = playernode.child("shield").child_value();

	// --- Animation Ranges --- 
	animranges.AnimationRangeRight_start = playernode.child("AnimationRangeRight").attribute("range_start").as_float();
	animranges.AnimationRangeRight_end = playernode.child("AnimationRangeRight").attribute("range_end").as_float();
	animranges.AnimationRangeLeft_start = playernode.child("AnimationRangeLeft").attribute("range_start").as_float();
	animranges.AnimationRangeLeft_end = playernode.child("AnimationRangeLeft").attribute("range_end").as_float();
	animranges.AnimationRangeLeft_start2= playernode.child("AnimationRangeLeft2").attribute("range_start").as_float();
	animranges.AnimationRangeLeft_end2 = playernode.child("AnimationRangeLeft2").attribute("range_end").as_float();

	animranges.AnimationRangeUp_start= playernode.child("AnimationRangeUp").attribute("range_start").as_float();
	animranges.AnimationRangeUp_end = playernode.child("AnimationRangeUp").attribute("range_end").as_float();
	animranges.AnimationRangeUpright_start= playernode.child("AnimationRangeUpright").attribute("range_start").as_float();
	animranges.AnimationRangeUpright_end = playernode.child("AnimationRangeUpright").attribute("range_end").as_float();
	animranges.AnimationRangeUpleft_start = playernode.child("AnimationRangeUpleft").attribute("range_start").as_float();
	animranges.AnimationRangeUpleft_end = playernode.child("AnimationRangeUpleft").attribute("range_end").as_float();

	animranges.AnimationRangeDown_start = playernode.child("AnimationRangeDown").attribute("range_start").as_float();
	animranges.AnimationRangeDown_end = playernode.child("AnimationRangeDown").attribute("range_end").as_float();
	animranges.AnimationRangeDownright_start= playernode.child("AnimationRangeDownright").attribute("range_start").as_float();
	animranges.AnimationRangeDownright_end = playernode.child("AnimationRangeDownright").attribute("range_end").as_float();
	animranges.AnimationRangeDownleft_start = playernode.child("AnimationRangeDownleft").attribute("range_start").as_float();
	animranges.AnimationRangeDownleft_end = playernode.child("AnimationRangeDownleft").attribute("range_end").as_float();

	// --- Player 1 Awake ---
	player1info.folder.assign(playernode.child("folder1").child_value());
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

	// --- P1 Particles ---
	fPoint particle_speed = { 250.0f, 250.0f };
	player1info.characterdata.basic_attack.anim.PushBack({ 0,0,28,18 });
	player1info.characterdata.basic_attack.anim.loop = true;
	player1info.characterdata.basic_attack.life = 2500;
	player1info.characterdata.basic_attack.speed = particle_speed;
	player1info.characterdata.basic_attack.particle_effect = &App->buff->effects[3];

	// --------------------

	// --- Player 2 Awake ---
	player2info.folder.assign(playernode.child("folder2").child_value());
	player2info.Texture.assign(playernode.child("texture2").child_value());
		  
	player2info.idleRight = LoadAnimation(player2info.folder.data(), "idleRight");
	player2info.idleLeft = LoadAnimation(player2info.folder.data(), "idleLeft");
	player2info.idleUp = LoadAnimation(player2info.folder.data(), "idleUp");
	player2info.idleDown = LoadAnimation(player2info.folder.data(), "idleDown");
	player2info.idleUpright = LoadAnimation(player2info.folder.data(), "idleUpright");
	player2info.idleUpleft = LoadAnimation(player2info.folder.data(), "idleUpleft");
	player2info.idleDownright = LoadAnimation(player2info.folder.data(), "idleDownright");
	player2info.idleDownleft = LoadAnimation(player2info.folder.data(), "idleDownleft");
		  
	player2info.moveRight = LoadAnimation(player2info.folder.data(), "moveRight");
	player2info.moveLeft = LoadAnimation(player2info.folder.data(), "moveLeft");
	player2info.moveUp = LoadAnimation(player2info.folder.data(), "moveUp");
	player2info.moveDown = LoadAnimation(player2info.folder.data(), "moveDown");
	player2info.moveUpright = LoadAnimation(player2info.folder.data(), "moveUpright");
	player2info.moveUpleft = LoadAnimation(player2info.folder.data(), "moveUpleft");
	player2info.moveDownright = LoadAnimation(player2info.folder.data(), "moveDownright");
	player2info.moveDownleft = LoadAnimation(player2info.folder.data(), "moveDownleft");
		  
	player2info.attackRight = LoadAnimation(player2info.folder.data(), "attackRight");
	player2info.attackLeft = LoadAnimation(player2info.folder.data(), "attackLeft");
	player2info.attackUp = LoadAnimation(player2info.folder.data(), "attackUp");
	player2info.attackDown = LoadAnimation(player2info.folder.data(), "attackDown");
	player2info.attackUpright = LoadAnimation(player2info.folder.data(), "attackUpright");
	player2info.attackUpleft = LoadAnimation(player2info.folder.data(), "attackUpleft");
	player2info.attackDownright = LoadAnimation(player2info.folder.data(), "attackDownright");
	player2info.attackDownleft = LoadAnimation(player2info.folder.data(), "attackDownleft");

	player2info.playerrect = { x,y,w,h };

	// --- P2 Particles ---
	player2info.characterdata.basic_attack.anim.PushBack({ 0,0,28,18 });
	player2info.characterdata.basic_attack.anim.loop = true;
	player2info.characterdata.basic_attack.life = 2500;
	player2info.characterdata.basic_attack.speed = particle_speed;
	player2info.characterdata.basic_attack.particle_effect = &App->buff->effects[3];

	// --------------------

	// --- Player 3 Awake ---
	player3info.folder.assign(playernode.child("folder3").child_value());
	player3info.Texture.assign(playernode.child("texture3").child_value());
		  
	player3info.idleRight = LoadAnimation(player3info.folder.data(), "idleRight");
	player3info.idleLeft = LoadAnimation(player3info.folder.data(), "idleLeft");
	player3info.idleUp = LoadAnimation(player3info.folder.data(), "idleUp");
	player3info.idleDown = LoadAnimation(player3info.folder.data(), "idleDown");
	player3info.idleUpright = LoadAnimation(player3info.folder.data(), "idleUpright");
	player3info.idleUpleft = LoadAnimation(player3info.folder.data(), "idleUpleft");
	player3info.idleDownright = LoadAnimation(player3info.folder.data(), "idleDownright");
	player3info.idleDownleft = LoadAnimation(player3info.folder.data(), "idleDownleft");
		  
	player3info.moveRight = LoadAnimation(player3info.folder.data(), "moveRight");
	player3info.moveLeft = LoadAnimation(player3info.folder.data(), "moveLeft");
	player3info.moveUp = LoadAnimation(player3info.folder.data(), "moveUp");
	player3info.moveDown = LoadAnimation(player3info.folder.data(), "moveDown");
	player3info.moveUpright = LoadAnimation(player3info.folder.data(), "moveUpright");
	player3info.moveUpleft = LoadAnimation(player3info.folder.data(), "moveUpleft");
	player3info.moveDownright = LoadAnimation(player3info.folder.data(), "moveDownright");
	player3info.moveDownleft = LoadAnimation(player3info.folder.data(), "moveDownleft");
		  
	player3info.attackRight = LoadAnimation(player3info.folder.data(), "attackRight");
	player3info.attackLeft = LoadAnimation(player3info.folder.data(), "attackLeft");
	player3info.attackUp = LoadAnimation(player3info.folder.data(), "attackUp");
	player3info.attackDown = LoadAnimation(player3info.folder.data(), "attackDown");
	player3info.attackUpright = LoadAnimation(player3info.folder.data(), "attackUpright");
	player3info.attackUpleft = LoadAnimation(player3info.folder.data(), "attackUpleft");
	player3info.attackDownright = LoadAnimation(player3info.folder.data(), "attackDownright");
	player3info.attackDownleft = LoadAnimation(player3info.folder.data(), "attackDownleft");
		  
	player3info.playerrect = { x,y,w,h };

	// --- P3 Particles ---
	player3info.characterdata.basic_attack.anim.PushBack({ 0,0,28,18 });
	player3info.characterdata.basic_attack.anim.loop = true;
	player3info.characterdata.basic_attack.life = 2500;
	player3info.characterdata.basic_attack.speed = particle_speed;
	player3info.characterdata.basic_attack.particle_effect = &App->buff->effects[3];


	// --------------------

	// --- Player 4 Awake ---
	player4info.folder.assign(playernode.child("folder4").child_value());
	player4info.Texture.assign(playernode.child("texture4").child_value());
		  
	player4info.idleRight = LoadAnimation(player4info.folder.data(), "idleRight");
	player4info.idleLeft = LoadAnimation(player4info.folder.data(), "idleLeft");
	player4info.idleUp = LoadAnimation(player4info.folder.data(), "idleUp");
	player4info.idleDown = LoadAnimation(player4info.folder.data(), "idleDown");
	player4info.idleUpright = LoadAnimation(player4info.folder.data(), "idleUpright");
	player4info.idleUpleft = LoadAnimation(player4info.folder.data(), "idleUpleft");
	player4info.idleDownright = LoadAnimation(player4info.folder.data(), "idleDownright");
	player4info.idleDownleft = LoadAnimation(player4info.folder.data(), "idleDownleft");
		  
	player4info.moveRight = LoadAnimation(player4info.folder.data(), "moveRight");
	player4info.moveLeft = LoadAnimation(player4info.folder.data(), "moveLeft");
	player4info.moveUp = LoadAnimation(player4info.folder.data(), "moveUp");
	player4info.moveDown = LoadAnimation(player4info.folder.data(), "moveDown");
	player4info.moveUpright = LoadAnimation(player4info.folder.data(), "moveUpright");
	player4info.moveUpleft = LoadAnimation(player4info.folder.data(), "moveUpleft");
	player4info.moveDownright = LoadAnimation(player4info.folder.data(), "moveDownright");
	player4info.moveDownleft = LoadAnimation(player4info.folder.data(), "moveDownleft");
		  
	player4info.attackRight = LoadAnimation(player4info.folder.data(), "attackRight");
	player4info.attackLeft = LoadAnimation(player4info.folder.data(), "attackLeft");
	player4info.attackUp = LoadAnimation(player4info.folder.data(), "attackUp");
	player4info.attackDown = LoadAnimation(player4info.folder.data(), "attackDown");
	player4info.attackUpright = LoadAnimation(player4info.folder.data(), "attackUpright");
	player4info.attackUpleft = LoadAnimation(player4info.folder.data(), "attackUpleft");
	player4info.attackDownright = LoadAnimation(player4info.folder.data(), "attackDownright");
	player4info.attackDownleft = LoadAnimation(player4info.folder.data(), "attackDownleft");
		  
	player4info.playerrect = { x,y,w,h };

	// --- P4 Particles ---
	player4info.characterdata.basic_attack.anim.PushBack({ 0,0,28,18 });
	player4info.characterdata.basic_attack.anim.loop = true;
	player4info.characterdata.basic_attack.life = 2500;
	player4info.characterdata.basic_attack.speed = particle_speed;
	player4info.characterdata.basic_attack.particle_effect = &App->buff->effects[3];

	// --------------------

	return ret;
}

// Called before the first frame
bool j1EntityManager::Start()
{
	LOG("start j1EntityManager");
	bool ret = true;

	shield_texture = App->tex->Load(shield_texturepath.data());

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
		if((*entity)->active)
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
		if ((*entity)->active)
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
	entity->Entityinfo.my_j1Entity = entity; // Assign our own j1Entity for buff manager purposes
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

			break;
		}
	}
	if (anim = true)
		return animation;
	else
	{
		delete animation;
		return nullptr;
	}

}



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
#include "j1Audio.h"

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

	// --- Shield ---
	shield_texturepath = playernode.child("shield").child_value();
	shield_anim = *LoadAnimation(playernode.child("foldershield").child_value(), "shield");
	shieldEnd_anim = *LoadAnimation(playernode.child("foldershield").child_value(), "shieldKill");
	shield_anim.loop = false;
	shield_anim.speed = 16.0f;
	shieldEnd_anim.loop = false;
	shieldEnd_anim.speed = 16.0f;

	// --- IDCircle ---
	circle_texturepath = playernode.child("IDCircle").child_value();

	IDCircle_red = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleRed");
	IDCirclesuper_red = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleRedSuper");
	IDCircleshield_red = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleRedShield");
	IDCircleboth_red = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleRedBoth");

	IDCircle_red.speed = 10.0f;
	IDCirclesuper_red.speed = 10.0f;
	IDCircleshield_red.speed = 10.0f;
	IDCircleboth_red.speed = 10.0f;

	IDCircle_green = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleGreen");
	IDCirclesuper_green	= *LoadAnimation(playernode.child("foldercircle").child_value(), "circleGreenSuper");
	IDCircleshield_green = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleGreenShield");
	IDCircleboth_green = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleGreenBoth");

	IDCircle_green.speed = 10.0f;
	IDCirclesuper_green.speed = 10.0f;
	IDCircleshield_green.speed = 10.0f;
	IDCircleboth_green.speed = 10.0f;

	IDCircle_blue = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleBlue");
	IDCirclesuper_blue = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleBlueSuper");
	IDCircleshield_blue = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleBlueShield");
	IDCircleboth_blue = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleBlueBoth");

	IDCircle_blue.speed = 10.0f;
	IDCirclesuper_blue.speed = 10.0f;
	IDCircleshield_blue.speed = 10.0f;
	IDCircleboth_blue.speed = 10.0f;

	IDCircle_yellow = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleYellow");
	IDCirclesuper_yellow = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleYellowSuper");
	IDCircleshield_yellow = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleYellowShield");
	IDCircleboth_yellow = *LoadAnimation(playernode.child("foldercircle").child_value(), "circleYellowBoth");

	IDCircle_yellow.speed = 10.0f;
	IDCirclesuper_yellow.speed = 10.0f;
	IDCircleshield_yellow.speed = 10.0f;
	IDCircleboth_yellow.speed = 10.0f;

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

	// --- Wendolin Awake ---
	Wendolin.folder.assign(playernode.child("folder1").child_value());
	Wendolin.Texture.assign(playernode.child("texture1").child_value());

	Wendolin.idleRight = *LoadAnimation(Wendolin.folder.data(), "idleRight");
	Wendolin.idleLeft = *LoadAnimation(Wendolin.folder.data(), "idleLeft");
	Wendolin.idleUp = *LoadAnimation(Wendolin.folder.data(), "idleUp");
	Wendolin.idleDown = *LoadAnimation(Wendolin.folder.data(), "idleDown");
	Wendolin.idleUpright = *LoadAnimation(Wendolin.folder.data(), "idleUpright");
	Wendolin.idleUpleft = *LoadAnimation(Wendolin.folder.data(), "idleUpleft");
	Wendolin.idleDownright = *LoadAnimation(Wendolin.folder.data(), "idleDownright");
	Wendolin.idleDownleft = *LoadAnimation(Wendolin.folder.data(), "idleDownleft");

	Wendolin.moveRight = *LoadAnimation(Wendolin.folder.data(), "moveRight");
	Wendolin.moveLeft = *LoadAnimation(Wendolin.folder.data(), "moveLeft");
	Wendolin.moveUp = *LoadAnimation(Wendolin.folder.data(), "moveUp");
	Wendolin.moveDown = *LoadAnimation(Wendolin.folder.data(), "moveDown");
	Wendolin.moveUpright = *LoadAnimation(Wendolin.folder.data(), "moveUpright");
	Wendolin.moveUpleft = *LoadAnimation(Wendolin.folder.data(), "moveUpleft");
	Wendolin.moveDownright = *LoadAnimation(Wendolin.folder.data(), "moveDownright");
	Wendolin.moveDownleft = *LoadAnimation(Wendolin.folder.data(), "moveDownleft");

	Wendolin.attackRight = *LoadAnimation(Wendolin.folder.data(), "attackRight");
	Wendolin.attackLeft = *LoadAnimation(Wendolin.folder.data(), "attackLeft");
	Wendolin.attackUp = *LoadAnimation(Wendolin.folder.data(), "attackUp");
	Wendolin.attackDown = *LoadAnimation(Wendolin.folder.data(), "attackDown");
	Wendolin.attackUpright = *LoadAnimation(Wendolin.folder.data(), "attackUpright");
	Wendolin.attackUpleft = *LoadAnimation(Wendolin.folder.data(), "attackUpleft");
	Wendolin.attackDownright = *LoadAnimation(Wendolin.folder.data(), "attackDownright");
	Wendolin.attackDownleft = *LoadAnimation(Wendolin.folder.data(), "attackDownleft");

	Wendolin.playerrect = { x,y,w,h };

	// --- Wendolin Particles ---
	fPoint particle_speed = { 175.0f, 175.0f };
	Wendolin.basic_attack.anim.PushBack({ 0,0,28,18 });
	Wendolin.basic_attack.anim.loop = true;
	Wendolin.basic_attack.life = 2000;
	Wendolin.basic_attack.speed = particle_speed;
	Wendolin.basic_attack.particle_effect = &App->buff->effects[3];

	// --------------------

	// --- Simon Awake ---
	Simon.folder.assign(playernode.child("folder2").child_value());
	Simon.Texture.assign(playernode.child("texture2").child_value());
		  
	Simon.idleRight = *LoadAnimation(Simon.folder.data(), "idleRight");
	Simon.idleLeft = *LoadAnimation(Simon.folder.data(), "idleLeft");
	Simon.idleUp = *LoadAnimation(Simon.folder.data(), "idleUp");
	Simon.idleDown = *LoadAnimation(Simon.folder.data(), "idleDown");
	Simon.idleUpright = *LoadAnimation(Simon.folder.data(), "idleUpright");
	Simon.idleUpleft = *LoadAnimation(Simon.folder.data(), "idleUpleft");
	Simon.idleDownright = *LoadAnimation(Simon.folder.data(), "idleDownright");
	Simon.idleDownleft = *LoadAnimation(Simon.folder.data(), "idleDownleft");
		  
	Simon.moveRight = *LoadAnimation(Simon.folder.data(), "moveRight");
	Simon.moveLeft = *LoadAnimation(Simon.folder.data(), "moveLeft");
	Simon.moveUp = *LoadAnimation(Simon.folder.data(), "moveUp");
	Simon.moveDown = *LoadAnimation(Simon.folder.data(), "moveDown");
	Simon.moveUpright = *LoadAnimation(Simon.folder.data(), "moveUpright");
	Simon.moveUpleft = *LoadAnimation(Simon.folder.data(), "moveUpleft");
	Simon.moveDownright = *LoadAnimation(Simon.folder.data(), "moveDownright");
	Simon.moveDownleft = *LoadAnimation(Simon.folder.data(), "moveDownleft");
		  
	Simon.attackRight = *LoadAnimation(Simon.folder.data(), "attackRight");
	Simon.attackLeft = *LoadAnimation(Simon.folder.data(), "attackLeft");
	Simon.attackUp = *LoadAnimation(Simon.folder.data(), "attackUp");
	Simon.attackDown = *LoadAnimation(Simon.folder.data(), "attackDown");
	Simon.attackUpright = *LoadAnimation(Simon.folder.data(), "attackUpright");
	Simon.attackUpleft = *LoadAnimation(Simon.folder.data(), "attackUpleft");
	Simon.attackDownright = *LoadAnimation(Simon.folder.data(), "attackDownright");
	Simon.attackDownleft = *LoadAnimation(Simon.folder.data(), "attackDownleft");

	Simon.playerrect = { x,y,w,h };

	// --- Simon Particles ---
	Simon.basic_attack.anim.PushBack({ 0,0,24,36 });
	Simon.basic_attack.anim.loop = true;
	Simon.basic_attack.life = 2000;
	Simon.basic_attack.speed = particle_speed;
	Simon.basic_attack.particle_effect = &App->buff->effects[3];
	Simon.basic_attack.ghost = true;

	// --------------------

	// --- Trakt Awake ---
	Trakt.folder.assign(playernode.child("folder3").child_value());
	Trakt.Texture.assign(playernode.child("texture3").child_value());
		  
	Trakt.idleRight = *LoadAnimation(Trakt.folder.data(), "idleRight");
	Trakt.idleLeft = *LoadAnimation(Trakt.folder.data(), "idleLeft");
	Trakt.idleUp = *LoadAnimation(Trakt.folder.data(), "idleUp");
	Trakt.idleDown = *LoadAnimation(Trakt.folder.data(), "idleDown");
	Trakt.idleUpright = *LoadAnimation(Trakt.folder.data(), "idleUpright");
	Trakt.idleUpleft = *LoadAnimation(Trakt.folder.data(), "idleUpleft");
	Trakt.idleDownright = *LoadAnimation(Trakt.folder.data(), "idleDownright");
	Trakt.idleDownleft = *LoadAnimation(Trakt.folder.data(), "idleDownleft");
		  
	Trakt.moveRight = *LoadAnimation(Trakt.folder.data(), "moveRight");
	Trakt.moveLeft = *LoadAnimation(Trakt.folder.data(), "moveLeft");
	Trakt.moveUp = *LoadAnimation(Trakt.folder.data(), "moveUp");
	Trakt.moveDown = *LoadAnimation(Trakt.folder.data(), "moveDown");
	Trakt.moveUpright = *LoadAnimation(Trakt.folder.data(), "moveUpright");
	Trakt.moveUpleft = *LoadAnimation(Trakt.folder.data(), "moveUpleft");
	Trakt.moveDownright = *LoadAnimation(Trakt.folder.data(), "moveDownright");
	Trakt.moveDownleft = *LoadAnimation(Trakt.folder.data(), "moveDownleft");
		  
	Trakt.attackRight = *LoadAnimation(Trakt.folder.data(), "attackRight");
	Trakt.attackLeft = *LoadAnimation(Trakt.folder.data(), "attackLeft");
	Trakt.attackUp = *LoadAnimation(Trakt.folder.data(), "attackUp");
	Trakt.attackDown = *LoadAnimation(Trakt.folder.data(), "attackDown");
	Trakt.attackUpright = *LoadAnimation(Trakt.folder.data(), "attackUpright");
	Trakt.attackUpleft = *LoadAnimation(Trakt.folder.data(), "attackUpleft");
	Trakt.attackDownright = *LoadAnimation(Trakt.folder.data(), "attackDownright");
	Trakt.attackDownleft = *LoadAnimation(Trakt.folder.data(), "attackDownleft");
		  
	Trakt.playerrect = { x,y,w,h };

	// --- Trakt Particles ---
	Trakt.basic_attack.anim.PushBack({ 0,0,30,30 });
	Trakt.basic_attack.anim.loop = true;
	Trakt.basic_attack.life = 2000;
	Trakt.basic_attack.speed = particle_speed;
	Trakt.basic_attack.particle_effect = &App->buff->effects[3];

	// --------------------

	// --- Meliadoul Awake ---
	Meliadoul.folder.assign(playernode.child("folder4").child_value());
	Meliadoul.Texture.assign(playernode.child("texture4").child_value());
		  
	Meliadoul.idleRight = *LoadAnimation(Meliadoul.folder.data(), "idleRight");
	Meliadoul.idleLeft = *LoadAnimation(Meliadoul.folder.data(), "idleLeft");
	Meliadoul.idleUp = *LoadAnimation(Meliadoul.folder.data(), "idleUp");
	Meliadoul.idleDown = *LoadAnimation(Meliadoul.folder.data(), "idleDown");
	Meliadoul.idleUpright = *LoadAnimation(Meliadoul.folder.data(), "idleUpright");
	Meliadoul.idleUpleft = *LoadAnimation(Meliadoul.folder.data(), "idleUpleft");
	Meliadoul.idleDownright = *LoadAnimation(Meliadoul.folder.data(), "idleDownright");
	Meliadoul.idleDownleft = *LoadAnimation(Meliadoul.folder.data(), "idleDownleft");
		  
	Meliadoul.moveRight = *LoadAnimation(Meliadoul.folder.data(), "moveRight");
	Meliadoul.moveLeft = *LoadAnimation(Meliadoul.folder.data(), "moveLeft");
	Meliadoul.moveUp = *LoadAnimation(Meliadoul.folder.data(), "moveUp");
	Meliadoul.moveDown = *LoadAnimation(Meliadoul.folder.data(), "moveDown");
	Meliadoul.moveUpright = *LoadAnimation(Meliadoul.folder.data(), "moveUpright");
	Meliadoul.moveUpleft = *LoadAnimation(Meliadoul.folder.data(), "moveUpleft");
	Meliadoul.moveDownright = *LoadAnimation(Meliadoul.folder.data(), "moveDownright");
	Meliadoul.moveDownleft = *LoadAnimation(Meliadoul.folder.data(), "moveDownleft");
		  
	Meliadoul.attackRight = *LoadAnimation(Meliadoul.folder.data(), "attackRight");
	Meliadoul.attackLeft = *LoadAnimation(Meliadoul.folder.data(), "attackLeft");
	Meliadoul.attackUp = *LoadAnimation(Meliadoul.folder.data(), "attackUp");
	Meliadoul.attackDown = *LoadAnimation(Meliadoul.folder.data(), "attackDown");
	Meliadoul.attackUpright = *LoadAnimation(Meliadoul.folder.data(), "attackUpright");
	Meliadoul.attackUpleft = *LoadAnimation(Meliadoul.folder.data(), "attackUpleft");
	Meliadoul.attackDownright = *LoadAnimation(Meliadoul.folder.data(), "attackDownright");
	Meliadoul.attackDownleft = *LoadAnimation(Meliadoul.folder.data(), "attackDownleft");
		  
	Meliadoul.playerrect = { x,y,w,h };

	// --- Meliadoul Particles ---
	Meliadoul.basic_attack.anim.PushBack({ 0,0,40,34 });
	Meliadoul.basic_attack.anim.loop = true;
	Meliadoul.basic_attack.life = 2000;
	Meliadoul.basic_attack.speed = particle_speed;
	Meliadoul.basic_attack.particle_effect = &App->buff->effects[3];

	// --------------------

	return ret;
}

// Called before the first frame
bool j1EntityManager::Start()
{
	LOG("start j1EntityManager");
	bool ret = true;

	// --- Loading Particle Textures ---
	Dagger_texture = App->tex->Load("particles/Wendolin Red Dagger.png");
	budu_texture = App->tex->Load("particles/Simon yellow budu.png");
	axe_texture = App->tex->Load("particles/Meliadoul green axe.png");
	inkball_texture = App->tex->Load("particles/Trakt ink ball.png");

	// --- Loading Character Specific Textures ---
	Wendolin.tex = App->tex->Load(Wendolin.Texture.data());
	Wendolin.basic_attack.tex = Dagger_texture;
	WendolinSuper_aimpath = App->tex->Load("textures/wendolinSuper_path.png");

	Simon.tex = App->tex->Load(Simon.Texture.data());
	Simon.basic_attack.tex = budu_texture;
	SimonSuper_aimpath = App->tex->Load("textures/simonSuper_path.png");

	Trakt.tex = App->tex->Load(Trakt.Texture.data());
	Trakt.basic_attack.tex = inkball_texture;
	TraktSuper_aimpath = App->tex->Load("textures/traktSuper_path.png");

	Meliadoul.tex = App->tex->Load(Meliadoul.Texture.data());
	Meliadoul.basic_attack.tex = axe_texture;
	MeliadoulSuper_aimpath = App->tex->Load("textures/meliadoulSuper_path.png");

	// --- Loading Character Specific FX ---
	Wendolin.basic_fx = App->audio->fxWendolinBasic;
	Wendolin.super_fx = App->audio->fxWendolinSpecial;

	Meliadoul.basic_fx = App->audio->fxMeliadoulBasic;
	Meliadoul.super_fx = App->audio->fxMeliadoulSpecial;

	Trakt.basic_fx = App->audio->fxTraktBasic;
	Trakt.super_fx = App->audio->fxTraktSpecial;

	Simon.basic_fx = App->audio->fxSimonBasic;
	Simon.super_fx = App->audio->fxSimonSpecial;

	circlesprites = App->tex->Load(circle_texturepath.data());
	aimpath = App->tex->Load("textures/aimpath.png");
	shield_texture = App->tex->Load(shield_texturepath.data());
	arrows_tex = App->tex->Load("textures/Arrows.png");

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

	//if (dt<update_ms_cycle*1.25f && dt > 0.0f)
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



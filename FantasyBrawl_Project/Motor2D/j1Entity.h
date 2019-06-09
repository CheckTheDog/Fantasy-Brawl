#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

class j1EntityManager;
class j1Entity;
struct Collider;
struct SDL_Texture;

enum class entity_type
{
	NONE,
	PLAYER,
	MAX
};

struct entity_info
{
	// --- Basic ---
	fPoint			position = { 0,0 };
	float          Speed = 110.0f;
	float			health = 300;
	float			strength = 1;
	float			og_health = 300;
	float			og_speed = 110.0f;
	float			og_strength = 1;

	//Pointer to the j1Entity belonging to the entity
	j1Entity* my_j1Entity = nullptr;

	// HEALING EFFECT
	j1Timer		healing;
	bool		heal_active = false;

	// EXHAUST EFFECT
	j1Timer		exhausting;
	bool		exhaust_active = false;

	//POISON --- PER TICK EFFECT --- reduces live each tick
	j1Timer		poison_tick;
	bool		poison_tick_active = false;
	int			poison_tick_iterator = 0;

	// WAR CRY --- TEMPORARY --- gain extra strength for a limited time
	j1Timer		war_cry;
	bool		war_cry_active = false;

	//SPEED UP ---TEMPORARY --- Gain extra speed for a limited time
	j1Timer     speed_up;
	bool		speed_up_active = false;

	// --- Collider data ---
	Collider*     entitycoll = nullptr;
	Collider* HitBox = nullptr;
	SDL_Rect entitycollrect = { 0,0,0,0 };

	// --- IDCircle anims ---
	Animation IDCircle;
	Animation IDCirclesuper;
	Animation IDCircleshield;
	Animation IDCircleboth;

	float scale = 1.0f;

};


class j1Entity
{
public:

	j1Entity(entity_type entitytype, entity_info info) : entitytype(entitytype), Entityinfo(info)
	{
	}

	void Init(j1EntityManager* manager)
	{
		this->manager = manager;
	}


	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual void FixedUpdate(float dt)
	{}

	// Called each logic iteration
	virtual void LogicUpdate(float dt)
	{}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnCollision(Collider* c1, Collider* c2)
	{}


public:

	// --- Entity ---
	entity_type  entitytype;
	entity_info  Entityinfo;

	//--- Active or inactive ----
	bool active = false;

	// --- Spritesheet ---
	SDL_Texture* spritesheet = nullptr;

	// --- Animation ---
	Animation* CurrentAnimation = nullptr;

	j1EntityManager*	manager = nullptr;
};


#endif // __J1ENTITY_H__


#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__

#include "p2Point.h"
#include <vector>
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"

class j1EntityManager;
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
	int          Speed = 0;

	// --- Collider data ---
	Collider*     entitycoll = nullptr;
	SDL_Rect entitycollrect = { 0,0,0,0 };
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


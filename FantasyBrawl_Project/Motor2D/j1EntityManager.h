// ----------------------------------------------------
// Controls all logic entities (enemies/player/etc.) --
// ----------------------------------------------------

#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include <list>
#include <string>

class j1Entity;

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	virtual ~j1EntityManager();

	// --- Called before render is available ---
	bool Awake(pugi::xml_node&);

	// --- Called before the first frame ---
	bool Start();

	// --- Called each loop iteration ---
	bool PreUpdate();
	bool Update(float dt);
	void UpdateEntity(float dt);
	bool PostUpdate(float dt);

	// --- Called before quitting ---
	bool CleanUp();

	// --- Entities management ---
	j1Entity * const CreateEntity(entity_type entitytype, entity_info entityinfo, Playerdata * player_info);
	void OnCollision(Collider* c1, Collider* c2);
	Animation* LoadAnimation(const char* animationPath, const char* animationName);

	// --- Save & Load ---
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public:

	std::list<j1Entity*>	entities;
	float				update_ms_cycle = 0;
	uint playerid = 0;
	// --- Player ---
	Playerdata playerinfo;
	// add playerinfo2 ...
};

#endif // __J1ENTITYMANAGER_H__

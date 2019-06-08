#ifndef __J1ITEMMANAGER_H__
#define __J1ITEMMANAGER_H__

#include "j1Module.h"
#include <list>

struct Item
{};

class j1ItemManager : public j1Module
{
public:
	j1ItemManager();
	~j1ItemManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	

private:
	std::list<Item*> items;
};
#endif

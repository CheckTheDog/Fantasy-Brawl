#include "j1ItemManager.h"

j1ItemManager::j1ItemManager()
{
}

j1ItemManager::~j1ItemManager()
{
}

bool j1ItemManager::Awake(pugi::xml_node &)
{
	return true;
}

bool j1ItemManager::Update(float dt)
{
	return true;
}

bool j1ItemManager::CleanUp()
{
	return true;
}

void j1ItemManager::OnCollision(Collider * c1, Collider * c2)
{
}

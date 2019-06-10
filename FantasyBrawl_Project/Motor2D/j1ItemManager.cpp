#include "j1App.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Viewport.h"
#include "j1Map.h"
#include "p2Log.h"
#include <time.h>
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

bool j1ItemManager::Start()
{
	items_tex = App->tex->Load("textures/crystals_spritesheet.png");
	App->item_manager->active = false;
	return true;
}

bool j1ItemManager::StartItemManager()
{
	App->item_manager->active = true;

	for (std::list<iPoint>::iterator spawner = spawners_pos.begin(); spawner != spawners_pos.end(); spawner++)
	{
		iPoint pos = App->map->MapToWorld((*spawner).x, (*spawner).y);
		pos.x -= 5;
		pos.y -= 40;
		CreateItem(RandomItemType(), pos );
	}
	CreateItem(ItemType::LIFE, { 600,700 });
	CreateItem(ItemType::SUPER_CD, { 650,700 });
	CreateItem(ItemType::SPEED, { 700,700 });

	return true;
}

bool j1ItemManager::CloseItemManager()
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		delete (*curr_item);
	}

	items.clear();

	App->item_manager->active = false;
	return true;
}

bool j1ItemManager::Update(float dt)
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		if ((*curr_item)->spawned == false && (*curr_item)->time_inactive.ReadSec() > 10.0f)
		{
			ReSpawnItem((*curr_item));
		}
	}
	return true;
}

bool j1ItemManager::PostUpdate(float dt)
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		if ((*curr_item)->animation != nullptr && (*curr_item)->spawned == true)
			App->view->PushQueue(9,items_tex, (*curr_item)->Pos.x, (*curr_item)->Pos.y, (*curr_item)->animation[(int)(*curr_item)->type]->GetCurrentFrame(dt));
	}

	return true;
}

bool j1ItemManager::CleanUp()
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		delete (*curr_item);
	}

	items.clear();

	App->tex->UnLoad(items_tex);
	items_tex = nullptr;
	return true;
}

Item* j1ItemManager::CreateItem(ItemType type, iPoint position)
{
	Item* ret = new Item(type,position);


	ret->col = App->coll->AddCollider({0,0,20,40}, COLLIDER_TYPE::COLLIDER_ITEM, (j1Module*)App->entities);
	ret->col->SetPos(ret->Pos.x + 8, ret->Pos.y + 10);

	ret->spawned = true;

	ret->animation[0] = LoadAnimation("Animations/crystals.tmx", "BlueCrystals");
	ret->animation[0]->speed = 6.0f;
	ret->animation[1] = LoadAnimation("Animations/crystals.tmx", "RedCrystals");
	ret->animation[1]->speed = 6.0f;
	ret->animation[2] = LoadAnimation("Animations/crystals.tmx", "YellowCrystals");
	ret->animation[2]->speed = 6.0f;

	items.push_back(ret);

	return ret;
}

void j1ItemManager::DeSpawnItem(Item * item)
{
	item->col->to_delete = true;
	item->spawned = false;
	item->time_inactive.Start();
}

void j1ItemManager::ReSpawnItem(Item * item)
{
	item->spawned = true;
	item->time_inactive.Stop();
	item->col = App->coll->AddCollider({ 0,0,20,40 }, COLLIDER_TYPE::COLLIDER_ITEM, (j1Module*)App->entities);
	item->col->SetPos(item->Pos.x + 8, item->Pos.y + 10);
	item->type = RandomItemType();
}

ItemType j1ItemManager::RandomItemType()
{
	ItemType type = ItemType::NONE;

	int t = (rand() % 1000);
	srand(t);
	type = (ItemType)(rand() % 3);

	return type;
}

Item* j1ItemManager::GetItemWithCollider(const Collider* c) const
{
	if (c == nullptr)
	{
		return nullptr;
	}

	for (std::list<Item*>::const_iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		if ((*curr_item)->col != nullptr && (*curr_item)->col == c)
		{
			return (*curr_item);
		}
	}
}

void j1ItemManager::PauseItemManager()
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		if((*curr_item)->spawned == false)
		(*curr_item)->time_inactive.Stop();
	}

	App->item_manager->active = false;
}

void j1ItemManager::ContinueItemManager()
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		if ((*curr_item)->spawned == false)
			(*curr_item)->time_inactive.Continue();
	}

	App->item_manager->active = true;
}

void j1ItemManager::ReceiveSpawnersPositions(const std::list<iPoint> to_copy)
{
	spawners_pos.clear();
	for (std::list<iPoint>::const_iterator item = to_copy.begin(); item != to_copy.end(); item++)
	{
		spawners_pos.push_back((*item));
	}
}

Animation* j1ItemManager::LoadAnimation(const char* animationPath, const char* animationName) {

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

void j1ItemManager::GetSpawnersFromMap()
{

}

#include "j1App.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Viewport.h"
#include "p2Log.h"
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
	CreateItem(ItemType::LIFE, { 600,600 });

	return true;
}

bool j1ItemManager::StartItemManager()
{
	return true;
}

bool j1ItemManager::CloseItemManager()
{
	return true;
}

bool j1ItemManager::Update(float dt)
{
	return true;
}

bool j1ItemManager::PostUpdate(float dt)
{
	for (std::list<Item*>::iterator curr_item = items.begin(); curr_item != items.end(); curr_item++)
	{
		if ((*curr_item)->animation != nullptr)
			App->view->PushQueue(9,items_tex, (*curr_item)->Pos.x, (*curr_item)->Pos.y, (*curr_item)->animation->GetCurrentFrame(dt));
	}

	return true;
}

bool j1ItemManager::CleanUp()
{
	return true;
}

Item* j1ItemManager::CreateItem(ItemType type, iPoint position)
{
	Item* ret = new Item(type,position);


	ret->col = App->coll->AddCollider({0,0,40,40}, COLLIDER_TYPE::COLLIDER_ITEM, App->item_manager);


	switch (type)
	{
	case ItemType::LIFE:
		ret->animation = LoadAnimation("Animations/crystals.tmx", "BlueCrystals");
		break;

	case ItemType::SUPER_CD:
		ret->animation = LoadAnimation("Animations/crystals.tmx", "RedCrystals");
		break;

	case ItemType::SPEED:
		ret->animation = LoadAnimation("Animations/crystals.tmx", "YellowCrystals");
		break;
	}


	ret->animation->speed = 6.0f;

	items.push_back(ret);

	return ret;
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
}

void j1ItemManager::ContinueItemManager()
{
}

void j1ItemManager::OnCollision(Collider * c1, Collider * c2)
{
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
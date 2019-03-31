#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"


j1Collision::j1Collision()
{
	name.assign("collision");

	matrix[static_cast<int>(COLLIDER_TYPE::COLLIDER_FLOOR)][static_cast<int>(COLLIDER_TYPE::COLLIDER_PLAYER)] = false;
	matrix[static_cast<int>(COLLIDER_TYPE::COLLIDER_FLOOR)][static_cast<int>(COLLIDER_TYPE::COLLIDER_FLOOR)] = false;

	matrix[static_cast<int>(COLLIDER_TYPE::COLLIDER_PLAYER)][static_cast<int>(COLLIDER_TYPE::COLLIDER_FLOOR)] = true;
	matrix[static_cast<int>(COLLIDER_TYPE::COLLIDER_PLAYER)][static_cast<int>(COLLIDER_TYPE::COLLIDER_PLAYER)] = false;
}

j1Collision::~j1Collision()
{
}

bool j1Collision::PreUpdate()
{
	BROFILER_CATEGORY("Collision_PreUpdate", Profiler::Color::DarkGoldenRod);

	bool ret = true;

	// Remove all colliders scheduled for deletion

	std::list <Collider*>::iterator item = colliders.begin();

	while (item != colliders.end())
	{
		if ((*item)->to_delete == true)
			colliders.erase(item);

		item++;
	}


	return ret;
}

bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Collision_Update", Profiler::Color::DarkCyan);


	return true;
}

bool j1Collision::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Collision_Post_Update", Profiler::Color::DarkGray);

	DebugDraw();

	return true;
}


bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	std::list <Collider*>::iterator item = colliders.begin();

	while (item != colliders.end())
	{
		delete *item;
		item++;
	}
	colliders.clear();

	return true;
}

Collider * j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module * callback)
{

	Collider * to_Add = new Collider(rect, type, callback);

	colliders.push_back(to_Add);

	return to_Add;
}

void j1Collision::DebugDraw()
{

	BROFILER_CATEGORY("Collision_Debug_Draw", Profiler::Color::BurlyWood);

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) //collider draw
		debug = !debug;

	if (debug == false)
		return;

	std::list <Collider*>::const_iterator item = colliders.begin();

	Uint8 alpha = 50;

	while (item != colliders.end())
	{

		switch ((*item)->type)
		{
		case COLLIDER_TYPE::COLLIDER_NONE: // white
			App->render->DrawQuad((*item)->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_TYPE::COLLIDER_FLOOR: // red
			App->render->DrawQuad((*item)->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_TYPE::COLLIDER_PLAYER: // green
			App->render->DrawQuad((*item)->rect, 0, 255, 0, alpha);
			break;

		}
		item++;
	}

}

void j1Collision::QueryCollisions(Collider & to_check) const
{
	BROFILER_CATEGORY("Collision_Query", Profiler::Color::CornflowerBlue);

	std::list <Collider*>::const_iterator collider_node = colliders.begin();

	while (collider_node != colliders.end())
	{
		//--- Only check area near entity ---

		// Target Collision    ------------------------------   Set Area surrounding Entity
		if (&to_check != *collider_node &&
			(*collider_node)->rect.x <= to_check.rect.x + AREAOFCOLLISION &&
			(*collider_node)->rect.x + (*collider_node)->rect.w >= to_check.rect.x - AREAOFCOLLISION &&
			(*collider_node)->rect.y <= to_check.rect.y + to_check.rect.h + AREAOFCOLLISION &&
			(*collider_node)->rect.y + (*collider_node)->rect.h >= to_check.rect.y - AREAOFCOLLISION)
		{

			if (to_check.CheckCollision((*collider_node)->rect) == true)
			{
				if (to_check.callback && matrix[static_cast<int>(to_check.type)][static_cast<int>((*collider_node)->type)])
				{
					to_check.callback->OnCollision(&to_check, *collider_node);
				}

				if ((*collider_node)->callback && matrix[static_cast<int>((*collider_node)->type)][static_cast<int>(to_check.type)])
				{
					(*collider_node)->callback->OnCollision(*collider_node, &to_check);
				}
			}

		}
		collider_node++;
	}

}


bool Collider::CheckCollision(const SDL_Rect & r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);

	//	return SDL_HasIntersection(&rect, &r);

	// between argument "r" and property "rect"
}

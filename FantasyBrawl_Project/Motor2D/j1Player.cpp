#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1EntityManager.h"
#include "j1Audio.h"


j1Player::j1Player(entity_info entityinfo, Playerdata * player_info) : j1Entity(entity_type::PLAYER, entityinfo), playerinfo(*player_info)
{

}

j1Player::~j1Player()
{

}

bool j1Player::Start()
{
	LOG("--- Loading player ---");

	// --- Player's bounding box ---
	Entityinfo.entitycollrect = playerinfo.playerrect;
	Entityinfo.entitycoll = App->coll->AddCollider(Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)manager);

	// --- Current Player Position ---
	Entityinfo.position.x = 0;
	Entityinfo.position.y = 0;

	Entityinfo.entitycoll->SetPos(Entityinfo.position.x, Entityinfo.position.y);

	Future_position.x = Entityinfo.position.x;
	Future_position.y = Entityinfo.position.y;

	// --- Currently playing Animation ---
	CurrentAnimation = playerinfo.idleRight;

	// --- Entity Spritesheet ---
	if (spritesheet == nullptr)
	spritesheet = App->tex->Load(playerinfo.Texture.data());

	//-- active ----
	active = true;

	Axis_range = AXISMAX;

	return true;
}

void j1Player::UpdateEntityMovement(float dt)
{

	switch (EntityMovement)
	{
	case MOVEMENT::RIGHTWARDS:

		break;

	case MOVEMENT::LEFTWARDS:

		break;

	case MOVEMENT::UPWARDS:

		break;

	case MOVEMENT::DOWNWARDS:

		break;

	}

	Entityinfo.entitycoll->SetPos(Future_position.x, Future_position.y);

	App->coll->QueryCollisions(*Entityinfo.entitycoll);

	MOVEMENT EntityMovement = MOVEMENT::STATIC;
}


void j1Player::HandleAnimations()
{
	// --- Handling Ground Animations ---


	//--------------    ---------------
}

bool j1Player::Update(float dt)
{
	// --- LOGIC --------------------

	Axisx_value = App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTX);
	Axisy_value = App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTY);

	if (Axisx_value > 0)
	multiplier_x = (Axisx_value) / Axis_range;
	else
	multiplier_x = (Axisx_value) / Axis_range;

	if (Axisy_value > 0)
	multiplier_y = (Axisy_value) / Axis_range;
	else
	multiplier_y = (Axisy_value) / Axis_range;
	
	LOG("multiplierx: %f", multiplier_x);
	LOG("multipliery: %f", multiplier_y);

	// --- RIGHTWARDS --
	if (multiplier_x > 0 && multiplier_y > 0 /*App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTX)> AXISRANGESTART*/)
	{
		EntityMovement = MOVEMENT::RIGHTWARDS;
	}

	if (EntityMovement != MOVEMENT::STATIC)
		UpdateEntityMovement(dt);

	// --- LEFTWARDS ---
	if (multiplier_x < 0 && multiplier_y > 0 /*IN_RANGE(App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTX), AXISMIN, -AXISRANGESTART)*/)
	{
		EntityMovement = MOVEMENT::LEFTWARDS;
	}

	if (EntityMovement != MOVEMENT::STATIC)
		UpdateEntityMovement(dt);

	// --- UPWARDS ---
	if (multiplier_y < 0  /*IN_RANGE(App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTY), AXISMIN, -AXISRANGESTART)*/)
	{
		EntityMovement = MOVEMENT::UPWARDS;
		LOG("down");
	}

	if (EntityMovement != MOVEMENT::STATIC)
		UpdateEntityMovement(dt);

	// --- RIGHTWARDS --
	if (multiplier_x > 0 && multiplier_y < 0 /*App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTX)> AXISRANGESTART*/)
	{
		EntityMovement = MOVEMENT::RIGHTWARDS;
	}

	if (EntityMovement != MOVEMENT::STATIC)
		UpdateEntityMovement(dt);

	// --- LEFTWARDS ---
	if (multiplier_x < 0 && multiplier_y < 0 /*IN_RANGE(App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTX), AXISMIN, -AXISRANGESTART)*/)
	{
		EntityMovement = MOVEMENT::LEFTWARDS;
	}

	if (EntityMovement != MOVEMENT::STATIC)
		UpdateEntityMovement(dt);


	// --- DOWNWARDS --- 
	if (multiplier_y > 0 /*App->input->GetAxis(PLAYER::P1, SDL_CONTROLLER_AXIS_LEFTY) > AXISRANGESTART*/)
	{
		EntityMovement = MOVEMENT::DOWNWARDS;
	}

	if (EntityMovement != MOVEMENT::STATIC)
		UpdateEntityMovement(dt);

	//-------------------------------

	// --- Handling animations ---

	HandleAnimations();

	if (abs(multiplier_x) > multipliermin)
	{
		multiplier_x *= Entityinfo.Speed*dt;
		Future_position.x += multiplier_x;
	}

	if (abs(multiplier_y) > multipliermin)
	{
		multiplier_y *= Entityinfo.Speed*dt;
		Future_position.y += multiplier_y;
	}

	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;

	// ---------------------- //

	// --- Blitting player ---

	App->render->Blit(spritesheet, Future_position.x, Future_position.y/*, &CurrentAnimation->GetCurrentFrame(dt)*/);

	// ---------------------- //

	return ret;
}

void j1Player::OnCollision(Collider * entitycollider, Collider * to_check)
{
	
		switch (EntityMovement)
		{
		case MOVEMENT::RIGHTWARDS:
			Right_Collision(entitycollider, to_check);
			break;
		case MOVEMENT::LEFTWARDS:
			Left_Collision(entitycollider, to_check);
			break;
		case MOVEMENT::UPWARDS:
			Up_Collision(entitycollider, to_check);
			break;
		case MOVEMENT::DOWNWARDS:
			Down_Collision(entitycollider, to_check);
			break;
		}

		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
	

}

void j1Player::Right_Collision(Collider * entitycollider, const Collider * to_check)
{
	SDL_IntersectRect(&entitycollider->rect, &to_check->rect, &Intersection);

	switch (to_check->type)
	{
	case COLLIDER_TYPE::COLLIDER_FLOOR:
		entitycollider->rect.x -= Intersection.w;
		break;
	}
}

void j1Player::Left_Collision(Collider * entitycollider, const Collider * to_check)
{
	SDL_IntersectRect(&entitycollider->rect, &to_check->rect, &Intersection);

	switch (to_check->type)
	{
	case COLLIDER_TYPE::COLLIDER_FLOOR:
		entitycollider->rect.x += Intersection.w;
		break;
	}
}

void j1Player::Up_Collision(Collider * entitycollider, const Collider * to_check)
{
	SDL_IntersectRect(&entitycollider->rect, &to_check->rect, &Intersection);

	coll_up = true;

	switch (to_check->type)
	{
	case COLLIDER_TYPE::COLLIDER_FLOOR:
		entitycollider->rect.y += Intersection.h;
		break;
	}
}

void j1Player::Down_Collision(Collider * entitycollider, const Collider * to_check)
{
	SDL_IntersectRect(&entitycollider->rect, &to_check->rect, &Intersection);

	switch (to_check->type)
	{
	case COLLIDER_TYPE::COLLIDER_FLOOR:
		entitycollider->rect.y -= Intersection.h;
		break;
	}

	coll_up = false;
}

bool j1Player::Load(pugi::xml_node &config)
{
	Future_position.x = config.child("Player").child("Playerx").attribute("value").as_float();
	Future_position.y = config.child("Player").child("Playery").attribute("value").as_float();

	return true;
}

bool j1Player::Save(pugi::xml_node &config) const
{
	config.append_child("Player").append_child("Playerx").append_attribute("value") = Future_position.x;
	config.child("Player").append_child("Playery").append_attribute("value") = Future_position.y;

	return true;
}

bool j1Player::CleanUp()
{
	/*App->tex->UnLoad(spritesheet);
*/
	if (Entityinfo.entitycoll != nullptr)
		Entityinfo.entitycoll = nullptr;

	return true;
}

void j1Player::FixedUpdate(float dt)
{
	// --- Update we do every frame ---

	PostUpdate(dt);

}

void j1Player::LogicUpdate(float dt)
{
	// --- Update we may not do every frame ---

	EntityMovement = MOVEMENT::STATIC;

	Update(dt);
}


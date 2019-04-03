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


j1Player::j1Player(entity_info entityinfo, Playerdata * player_info) : j1Entity(entity_type::PLAYER, entityinfo), player1info(*player_info)
{

}

j1Player::~j1Player()
{

}

bool j1Player::Start()
{
	LOG("--- Loading player ---");

	active = true;

	// --- Entity Spritesheet ---
	if (spritesheet == nullptr)
		spritesheet = App->tex->Load(player1info.Texture.data());

	// --- Currently playing Animation ---
	CurrentAnimation = player1info.idleDown;

	// --- Current Movement State (for collisions) ---
	MOVEMENT EntityMovement = MOVEMENT::STATIC;

	// --- Current Player Position ---
	Entityinfo.position.x = 100;
	Entityinfo.position.y = 0;
	Future_position.x = Entityinfo.position.x;
	Future_position.y = Entityinfo.position.y;

	// --- Player's bounding box ---
	Entityinfo.entitycollrect = player1info.playerrect;
	Entityinfo.entitycoll = App->coll->AddCollider(Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)manager);
	Entityinfo.entitycoll->SetPos(Entityinfo.position.x, Entityinfo.position.y);

	// --- Check Player ID to assign gamepad ---
	if (manager->playerid == 0)
		ID = PLAYER::P1;
	else if (manager->playerid == 1)
		ID = PLAYER::P2;
	else if (manager->playerid == 2)
		ID = PLAYER::P3;
	else
		ID = PLAYER::P4;

	manager->playerid++;

	return true;
}

void j1Player::HandleAnimations()
{
	// --- Handling Animations ---

	bool attacking = false;

	if (App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
		attacking = true;

	/*if ((CurrentAnimation == player1info.attackRight
		|| CurrentAnimation == player1info.attackLeft
		|| CurrentAnimation == player1info.attackUp
		|| CurrentAnimation == player1info.attackUpright
		|| CurrentAnimation == player1info.attackUpleft
		|| CurrentAnimation == player1info.attackDown
		|| CurrentAnimation == player1info.attackDownright
		|| CurrentAnimation == player1info.attackDownleft)
		&& CurrentAnimation->Finished() == true)
		attacking = false;*/

	// --- Moving Right ---

	if (multiplier_x > multipliermin)
	{
		if (multiplier_y > multipliermin)
		{
			if (!attacking)
				CurrentAnimation = player1info.moveDownright;
			else
				CurrentAnimation = player1info.attackDownright;
		}
		else if (multiplier_y < -multipliermin)
		{
			if (!attacking)
				CurrentAnimation = player1info.moveUpright;
			else
				CurrentAnimation = player1info.attackUpright;
		}
		else
		{
			if (!attacking)
				CurrentAnimation = player1info.moveRight;
			else
				CurrentAnimation = player1info.attackRight;
		}
	}

	// --- Moving Left ---

	else if (multiplier_x < -multipliermin)
	{
		if (multiplier_y > multipliermin)
		{
			if (!attacking)
				CurrentAnimation = player1info.moveDownleft;
			else
				CurrentAnimation = player1info.attackDownleft;
		}
		else if (multiplier_y < -multipliermin)
		{
			if (!attacking)
				CurrentAnimation = player1info.moveUpleft;
			else
				CurrentAnimation = player1info.attackUpleft;
		}
		else
		{
			if (!attacking)
				CurrentAnimation = player1info.moveLeft;
			else
				CurrentAnimation = player1info.attackLeft;
		}
	}
	else
	{
		if (multiplier_y > multipliermin)
		{
			if (!attacking)
				CurrentAnimation = player1info.moveDown;
			else
				CurrentAnimation = player1info.attackDown;
		}
		else if (multiplier_y < -multipliermin)
		{
			if (!attacking)
				CurrentAnimation = player1info.moveUp;
			else
				CurrentAnimation = player1info.attackUp;
		}

		// --- Not moving at all ---

		else 
		{
			if (CurrentAnimation == player1info.moveRight 
				|| CurrentAnimation == player1info.idleRight
				|| CurrentAnimation == player1info.attackRight)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleRight;
				else
					CurrentAnimation = player1info.attackRight;
			}
			else if (CurrentAnimation == player1info.moveLeft 
				|| CurrentAnimation == player1info.idleLeft
				|| CurrentAnimation == player1info.attackLeft)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleLeft;
				else
					CurrentAnimation = player1info.attackLeft;
			}
			else if (CurrentAnimation == player1info.moveDown 
				|| CurrentAnimation == player1info.idleDown
				|| CurrentAnimation == player1info.attackDown)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleDown;
				else
					CurrentAnimation = player1info.attackDown;
			}
			else if (CurrentAnimation == player1info.moveDownright 
				|| CurrentAnimation == player1info.idleDownright
				|| CurrentAnimation == player1info.attackDownright)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleDownright;
				else
					CurrentAnimation = player1info.attackDownright;
			}
			else if (CurrentAnimation == player1info.moveDownleft 
				|| CurrentAnimation == player1info.idleDownleft
				|| CurrentAnimation == player1info.attackDownleft)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleDownleft;
				else
					CurrentAnimation = player1info.attackDownleft;
			}
			else if (CurrentAnimation == player1info.moveUp 
				|| CurrentAnimation == player1info.idleUp
				|| CurrentAnimation == player1info.attackUp)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleUp;
				else
					CurrentAnimation = player1info.attackUp;
			}
			else if (CurrentAnimation == player1info.moveUpright 
				|| CurrentAnimation == player1info.idleUpright
				|| CurrentAnimation == player1info.attackUpright)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleUpright;
				else
					CurrentAnimation = player1info.attackUpright;
			}
			else if (CurrentAnimation == player1info.moveUpleft 
				|| CurrentAnimation == player1info.idleUpleft
				|| CurrentAnimation == player1info.attackUpleft)
			{
				if (!attacking)
					CurrentAnimation = player1info.idleUpleft;
				else
					CurrentAnimation = player1info.attackUpleft;
			}
		}
	}

	// ---------------------------
}

void j1Player::MoveX(float dt)
{
	// --- Performing Movement X ---

	if (abs(multiplier_x) > multipliermin)
	{
		multiplier_x *= Entityinfo.Speed*dt;
		Future_position.x += multiplier_x;
	}

	direction = AXISDIRECTION::AXIS_X;

	CheckCollision();
}

void j1Player::MoveY(float dt)
{
	// --- Performing Movement Y ---

	if (abs(multiplier_y) > multipliermin)
	{
		multiplier_y *= Entityinfo.Speed*dt;
		Future_position.y += multiplier_y;
	}

	direction = AXISDIRECTION::AXIS_Y;

	CheckCollision();
}

void j1Player::HandleInput()
{
	// --- LOGIC ---

	Axisx_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_LEFTX);
	Axisy_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_LEFTY);

	if (Axisx_value > 0)
		multiplier_x = (Axisx_value) / AXISMAX;
	else
		multiplier_x = (Axisx_value) / AXISMAX;

	if (Axisy_value > 0)
		multiplier_y = (Axisy_value) / AXISMAX;
	else
		multiplier_y = (Axisy_value) / AXISMAX;

	//--------------
}

bool j1Player::Update(float dt)
{
	HandleInput();

	HandleAnimations();

	MoveX(dt);

	MoveY(dt);

	// --- Adjust Player's Position ---
	this->Entityinfo.position = Future_position;

	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;

	App->render->Blit(spritesheet, this->Entityinfo.position.x - 7, this->Entityinfo.position.y, &CurrentAnimation->GetCurrentFrame(dt));

	return ret;
}

void j1Player::CheckCollision()
{
	Entityinfo.entitycoll->SetPos(Future_position.x, Future_position.y);

	App->coll->QueryCollisions(*Entityinfo.entitycoll);

	Entityinfo.entitycoll->SetPos(Future_position.x, Future_position.y);
}

void j1Player::OnCollision(Collider * entitycollider, Collider * to_check)
{
	switch (direction)
	{
	case AXISDIRECTION::AXIS_X:

		if (multiplier_x > 0.0f)
			EntityMovement = MOVEMENT::RIGHTWARDS;
		else if (multiplier_x < 0.0f)
			EntityMovement = MOVEMENT::LEFTWARDS;
		break;

	case AXISDIRECTION::AXIS_Y:

		if (multiplier_y > 0.0f)
			EntityMovement = MOVEMENT::DOWNWARDS;
		else if (multiplier_y < 0.0f)
			EntityMovement = MOVEMENT::UPWARDS;
		break;
	}

	
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


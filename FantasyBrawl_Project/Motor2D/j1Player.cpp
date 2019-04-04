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

	active = true;

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

	switch (ID)
	{
	case PLAYER::P1:
		playerinfo = manager->player1info;
		break;

	case PLAYER::P2:
		playerinfo = manager->player2info;
		break;

	case PLAYER::P3:
		playerinfo = manager->player3info;
		break;

	case PLAYER::P4:
		playerinfo = manager->player4info;
		break;
	}

	// --- Entity Spritesheet ---
	if (spritesheet == nullptr)
		spritesheet = App->tex->Load(playerinfo.Texture.data());

	// --- Currently playing Animation ---
	CurrentAnimation = playerinfo.idleDown;

	// --- Current Movement State (for collisions) ---
	EntityMovement = MOVEMENT::STATIC;
	PlayerState = PSTATE::IDLE;

	// --- Current Player Position ---
	Entityinfo.position.x = 100;
	Entityinfo.position.y = 0;
	Future_position.x = Entityinfo.position.x;
	Future_position.y = Entityinfo.position.y;

	// --- Player's bounding box ---
	Entityinfo.entitycollrect = playerinfo.playerrect;
	Entityinfo.entitycoll = App->coll->AddCollider(Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)manager);
	Entityinfo.entitycoll->SetPos(Entityinfo.position.x, Entityinfo.position.y);

	return true;
}

void j1Player::HandleAnimations()
{
	if ((App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
		&& (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin))
		PlayerState = PSTATE::ATTACKING;
	else if ((abs(LJdirection_x) > multipliermin || abs(LJdirection_y) > multipliermin))
		PlayerState = PSTATE::MOVING;
	else
		PlayerState = PSTATE::IDLE;

	switch (PlayerState)
	{
	case PSTATE::MOVING:
		HandleMovementAnimations();
		break;

	case PSTATE::ATTACKING:
		HandleAttackAnimations();
		break;

	case PSTATE::IDLE:
		HandleIdleAnimations();
		break;
	}
}

void j1Player::HandleAttackAnimations()
{
	// --- Attacking Right ---

	if (RJdirection_x > multipliermin)
	{
		if (RJdirection_y > multipliermin)
		{
			CurrentAnimation = playerinfo.attackDownright;
		}
		else if (RJdirection_y < -multipliermin)
		{
			CurrentAnimation = playerinfo.attackUpright;
		}
		else
		{
			CurrentAnimation = playerinfo.attackRight;
		}
	}

	// --- Attacking Left ---

	else if (RJdirection_x < -multipliermin)
	{
		if (RJdirection_y > multipliermin)
		{
			CurrentAnimation = playerinfo.attackDownleft;
		}
		else if (RJdirection_y < -multipliermin)
		{
			CurrentAnimation = playerinfo.attackUpleft;
		}
		else
		{
			CurrentAnimation = playerinfo.attackLeft;
		}
	}

	// --- Attacking UP/Down ---

	else
	{
		if (RJdirection_y > multipliermin)
		{
			CurrentAnimation = playerinfo.attackDown;
		}
		else if (RJdirection_y < -multipliermin)
		{
			CurrentAnimation = playerinfo.attackUp;
		}
	}
}

void j1Player::HandleMovementAnimations()
{
	// --- Moving Right ---

	if (LJdirection_x > multipliermin)
	{
		if (LJdirection_y > multipliermin)
		{
			CurrentAnimation = playerinfo.moveDownright;
		}
		else if (LJdirection_y < -multipliermin)
		{
			CurrentAnimation = playerinfo.moveUpright;
		}
		else
		{
			CurrentAnimation = playerinfo.moveRight;
		}
	}

	// --- Moving Left ---

	else if (LJdirection_x < -multipliermin)
	{
		if (LJdirection_y > multipliermin)
		{
			CurrentAnimation = playerinfo.moveDownleft;
		}
		else if (LJdirection_y < -multipliermin)
		{
			CurrentAnimation = playerinfo.moveUpleft;
		}
		else
		{
			CurrentAnimation = playerinfo.moveLeft;
		}
	}

	// --- Moving UP/Down ---

	else
	{
		if (LJdirection_y > multipliermin)
		{
			CurrentAnimation = playerinfo.moveDown;
		}
		else if (LJdirection_y < -multipliermin)
		{
			CurrentAnimation = playerinfo.moveUp;
		}
	}
}

void j1Player::HandleIdleAnimations()
{
	// --- Not moving at all ---

	if (CurrentAnimation == playerinfo.moveRight
		|| CurrentAnimation == playerinfo.attackRight)
	{
		CurrentAnimation = playerinfo.idleRight;
	}
	else if (CurrentAnimation == playerinfo.moveLeft
		|| CurrentAnimation == playerinfo.attackLeft)
	{
		CurrentAnimation = playerinfo.idleLeft;
	}
	else if (CurrentAnimation == playerinfo.moveDown
		|| CurrentAnimation == playerinfo.attackDown)
	{
		CurrentAnimation = playerinfo.idleDown;
	}
	else if (CurrentAnimation == playerinfo.moveDownright
		|| CurrentAnimation == playerinfo.attackDownright)
	{
		CurrentAnimation = playerinfo.idleDownright;
	}
	else if (CurrentAnimation == playerinfo.moveDownleft
		|| CurrentAnimation == playerinfo.attackDownleft)
	{
		CurrentAnimation = playerinfo.idleDownleft;
	}
	else if (CurrentAnimation == playerinfo.moveUp
		|| CurrentAnimation == playerinfo.attackUp)
	{
		CurrentAnimation = playerinfo.idleUp;
	}
	else if (CurrentAnimation == playerinfo.moveUpright
		|| CurrentAnimation == playerinfo.attackUpright)
	{
		CurrentAnimation = playerinfo.idleUpright;
	}
	else if (CurrentAnimation == playerinfo.moveUpleft
		|| CurrentAnimation == playerinfo.attackUpleft)
	{
		CurrentAnimation = playerinfo.idleUpleft;
	}
}

void j1Player::MoveX(float dt)
{
	// --- Performing Movement X ---

	if (abs(LJdirection_x) > multipliermin)
	{
		LJdirection_x *= Entityinfo.Speed*dt;
		Future_position.x += LJdirection_x;
	}

	direction = AXISDIRECTION::AXIS_X;

	CheckCollision();
}

void j1Player::MoveY(float dt)
{
	// --- Performing Movement Y ---

	if (abs(LJdirection_y) > multipliermin)
	{
		LJdirection_y *= Entityinfo.Speed*dt;
		Future_position.y += LJdirection_y;
	}

	direction = AXISDIRECTION::AXIS_Y;

	CheckCollision();
}

void j1Player::HandleInput()
{
	// --- LOGIC: Left Joystick ---

	LJAxisx_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_LEFTX);
	LJAxisy_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_LEFTY);

	if (LJAxisx_value > 0)
		LJdirection_x = (LJAxisx_value) / AXISMAX;
	else
		LJdirection_x = (LJAxisx_value) / AXISMAX;

	if (LJAxisy_value > 0)
		LJdirection_y = (LJAxisy_value) / AXISMAX;
	else
		LJdirection_y = (LJAxisy_value) / AXISMAX;

	// --- LOGIC: Right Joystick ---

	RJAxisx_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_RIGHTX);
	RJAxisy_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_RIGHTY);

	if (RJAxisx_value > 0)
		RJdirection_x = (RJAxisx_value) / AXISMAX;
	else
		RJdirection_x = (RJAxisx_value) / AXISMAX;

	if (RJAxisy_value > 0)
		RJdirection_y = (RJAxisy_value) / AXISMAX;
	else
		RJdirection_y = (RJAxisy_value) / AXISMAX;

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

		if (LJdirection_x > 0.0f)
			EntityMovement = MOVEMENT::RIGHTWARDS;
		else if (LJdirection_x < 0.0f)
			EntityMovement = MOVEMENT::LEFTWARDS;
		break;

	case AXISDIRECTION::AXIS_Y:

		if (LJdirection_y > 0.0f)
			EntityMovement = MOVEMENT::DOWNWARDS;
		else if (LJdirection_y < 0.0f)
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


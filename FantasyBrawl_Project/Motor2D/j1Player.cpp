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
#include "j1ParticleSystem.h"
#include "j1Audio.h"
#include "j1Viewport.h"
#include "j1BuffManager.h"


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

	// --- Animations ---
	CurrentAnimation = playerinfo.idleDown;

	// --- Current Movement State (for collisions) ---
	EntityMovement = MOVEMENT::STATIC;
	PlayerState = PSTATE::IDLE;

	// --- Current Player Position ---
	Future_position.x = Entityinfo.position.x;
	Future_position.y = Entityinfo.position.y;

	// --- Player's bounding box ---
	Entityinfo.entitycollrect = playerinfo.playerrect;
	Entityinfo.entitycoll = App->coll->AddCollider(Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)manager);
	Entityinfo.entitycoll->SetPos(Entityinfo.position.x, Entityinfo.position.y);

	//// --- P1 Particles ---
	//playerinfo.characterdata.basic_attack.anim.PushBack({ 0,0,28,18 });
	//playerinfo.characterdata.basic_attack.anim.loop = true;
	//playerinfo.characterdata.basic_attack.life = 2500;
	//playerinfo.characterdata.basic_attack.particle_effect = &App->buff->effects[3];

	return true;
}

void j1Player::HandleAnimations()
{
	if ((App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
		&& (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin))
		PlayerState = PSTATE::ATTACKING;
	else if ((abs(LJdirection_x) > multipliermin || abs(LJdirection_y) > multipliermin))
		PlayerState = PSTATE::MOVING;

	switch (PlayerState)
	{
	case PSTATE::MOVING:
		GetMovementAnimation();
		break;

	case PSTATE::ATTACKING:
		GetAttackAnimation();
		break;
	}
}

bool j1Player::InRange(float axisX, float axisY, float range_start, float range_end)
{
	float angle = std::atan2(axisY, axisX) * (180.0f / M_PI);

	/*if (axisX == 0 || axisY == 0)
	angle = 0.0f;*/

	//LOG("angle: %f", angle);

	return (angle > range_start && angle <= range_end);
}

void j1Player::GetAttackAnimation()
{
	if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeRight_start, manager->animranges.AnimationRangeRight_end))
		CurrentAnimation = playerinfo.attackRight;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeLeft_start, manager->animranges.AnimationRangeLeft_end)
		|| InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeLeft_end2, manager->animranges.AnimationRangeLeft_start2))
		CurrentAnimation = playerinfo.attackLeft;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeUp_start, manager->animranges.AnimationRangeUp_end))
		CurrentAnimation = playerinfo.attackUp;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeUpright_start, manager->animranges.AnimationRangeUpright_end))
		CurrentAnimation = playerinfo.attackUpright;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeUpleft_start, manager->animranges.AnimationRangeUpleft_end))
		CurrentAnimation = playerinfo.attackUpleft;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeDown_start, manager->animranges.AnimationRangeDown_end))
		CurrentAnimation = playerinfo.attackDown;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeDownright_start, manager->animranges.AnimationRangeDownright_end))
		CurrentAnimation = playerinfo.attackDownright;

	else if (InRange(RJdirection_x, RJdirection_y, manager->animranges.AnimationRangeDownleft_start, manager->animranges.AnimationRangeDownleft_end))
		CurrentAnimation = playerinfo.attackDownleft;
}

void j1Player::GetMovementAnimation()
{
	if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeRight_start, manager->animranges.AnimationRangeRight_end))
		CurrentAnimation = playerinfo.moveRight;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeLeft_start, manager->animranges.AnimationRangeLeft_end)
		|| InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeLeft_end2, manager->animranges.AnimationRangeLeft_start2))
		 	CurrentAnimation = playerinfo.moveLeft;
		 
	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeUp_start, manager->animranges.AnimationRangeUp_end))
			CurrentAnimation = playerinfo.moveUp;
		
	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeUpright_start, manager->animranges.AnimationRangeUpright_end))
			CurrentAnimation = playerinfo.moveUpright;
		
	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeUpleft_start, manager->animranges.AnimationRangeUpleft_end))
			CurrentAnimation = playerinfo.moveUpleft;
		
	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeDown_start, manager->animranges.AnimationRangeDown_end))
		 	CurrentAnimation = playerinfo.moveDown;
		 
	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeDownright_start, manager->animranges.AnimationRangeDownright_end))
		 	CurrentAnimation = playerinfo.moveDownright;
		 
	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeDownleft_start, manager->animranges.AnimationRangeDownleft_end))
		CurrentAnimation = playerinfo.moveDownleft;
}

void j1Player::GetIdleAnimation()
{
	if (CurrentAnimation == playerinfo.moveRight || CurrentAnimation == playerinfo.attackRight)
		CurrentAnimation = playerinfo.idleRight;

	else if (CurrentAnimation == playerinfo.moveLeft || CurrentAnimation == playerinfo.attackLeft)
		CurrentAnimation = playerinfo.idleLeft;

	else if (CurrentAnimation == playerinfo.moveUp || CurrentAnimation == playerinfo.attackUp)
		CurrentAnimation = playerinfo.idleUp;

	else if (CurrentAnimation == playerinfo.moveUpright || CurrentAnimation == playerinfo.attackUpright)
		CurrentAnimation = playerinfo.idleUpright;

	else if (CurrentAnimation == playerinfo.moveUpleft || CurrentAnimation == playerinfo.attackUpleft)
		CurrentAnimation = playerinfo.idleUpleft;

	else if (CurrentAnimation == playerinfo.moveDown || CurrentAnimation == playerinfo.attackDown)
		CurrentAnimation = playerinfo.idleDown;

	else if (CurrentAnimation == playerinfo.moveDownright || CurrentAnimation == playerinfo.attackDownright)
		CurrentAnimation = playerinfo.idleDownright;

	else if (CurrentAnimation == playerinfo.moveDownleft || CurrentAnimation == playerinfo.attackDownleft)
		CurrentAnimation = playerinfo.idleDownleft;

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

	// --- Assign speed to all particles ---
	//playerinfo.characterdata.basic_attack.speed.x = RJdirection_x * 300;
	//playerinfo.characterdata.basic_attack.speed.y = RJdirection_y * 300;
	playerinfo.characterdata.basic_attack.direction.x = RJdirection_x;
	playerinfo.characterdata.basic_attack.direction.y = RJdirection_y;
	playerinfo.characterdata.basic_attack.angle = std::atan2(RJdirection_y, RJdirection_x) /** (180.0f / M_PI)*/;

	//LOG("angle: %f", playerinfo.characterdata.basic_attack.angle);

	//LOG("direction_x: %f", RJdirection_x);
	//LOG("direction_y: %f", RJdirection_y);
}

void j1Player::HandleAttacks(PLAYER ID)
{
	switch (ID)
	{
	case PLAYER::P1:

		break;
	case PLAYER::P2:

		break;
	case PLAYER::P3:

		break;
	case PLAYER::P4:

		break;
	default:
		break;
	}

}

bool j1Player::Update(float dt)
{
	HandleInput();

	if (abs(LJAxisx_value) > JOYSTICK_DEAD_ZONE 
		|| abs(LJAxisy_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisx_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisy_value) > JOYSTICK_DEAD_ZONE
		)
		HandleAnimations();
	
	if (PlayerState == PSTATE::IDLE)
		GetIdleAnimation();

	MoveX(dt);

	MoveY(dt);


	// --- Adjust Player's Position ---
	this->Entityinfo.position = Future_position;

	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;

	App->view->PushQueue(4,spritesheet, this->Entityinfo.position.x, this->Entityinfo.position.y - 65, CurrentAnimation->GetCurrentFrame(dt));
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

		if (to_check->type == COLLIDER_TYPE::COLLIDER_PARTICLE)
			CheckParticleCollision(entitycollider, to_check);

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

void j1Player::CheckParticleCollision(Collider * entitycollider, const Collider * to_check)
{
	Particle* pcollided = App->particlesys->GetCollidedParticle(entitycollider,to_check);

	if (pcollided && pcollided->originplayer != this)
	{
		App->buff->ApplyEffect(pcollided->particle_effect, this);
		LOG("player life: %f", this->Entityinfo.health);
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
	/*App->tex->UnLoad(spritesheet);*/
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

	if ((App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_UP) && PlayerState == PSTATE::ATTACKING)
	{
		App->particlesys->AddParticle(playerinfo.characterdata.basic_attack, this->Entityinfo.position.x, this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
	}

	PlayerState = PSTATE::IDLE;

	Update(dt);
}


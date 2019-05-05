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
#include "j1BuffManager.h"
#include "j1ArenaInteractions.h"
#include "j1Viewport.h"
#include "j1BuffManager.h"
#include "j1Map.h"
#include <queue>

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

	// --- Animations ---
	CurrentAnimation = &playerinfo.idleDown;
	CurrentIDCircleAnimation = &this->Entityinfo.IDCircle;

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


	superTimer.Start();
	shieldTimer.Start();
	basicTimer.Start();

	return true;
}

void j1Player::HandleAnimations()
{
	if (((App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
		&& basicTimer.ReadSec() > 0.5f)
		|| attackanimTimer.ReadSec() < 0.2)
		PlayerState = PSTATE::ATTACKING;
	else if (abs(LJdirection_x) > multipliermin || abs(LJdirection_y) > multipliermin)
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

	if (PlayerState == PSTATE::NONE && (abs(LJdirection_x) > multipliermin || abs(LJdirection_y) > multipliermin))
	{
		GetMovementAnimation();
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
	// --- Player Auto aim attack ---
	playerinfo.basic_attack.direction.x = GetNearestPlayerDirection().x / Aim_Radius;
	playerinfo.basic_attack.direction.y = GetNearestPlayerDirection().y / Aim_Radius;

	if (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin)
	{
		// --- Player Manually directed Attack ---
		playerinfo.basic_attack.direction.x = RJdirection_x;
		playerinfo.basic_attack.direction.y = RJdirection_y;
		playerinfo.basic_attack.angle = std::atan2(RJdirection_y, RJdirection_x);
	}

	float direction_x = playerinfo.basic_attack.direction.x;
	float direction_y = playerinfo.basic_attack.direction.y;
	playerinfo.basic_attack.angle = std::atan2(direction_y, direction_x);

	if (playerinfo.basic_attack.direction.x != 0.0f && playerinfo.basic_attack.direction.x != 0.0f)
	{
		if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeRight_start, manager->animranges.AnimationRangeRight_end))
			CurrentAnimation = &playerinfo.attackRight;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeLeft_start, manager->animranges.AnimationRangeLeft_end)
			|| InRange(direction_x, direction_y, manager->animranges.AnimationRangeLeft_end2, manager->animranges.AnimationRangeLeft_start2))
			CurrentAnimation = &playerinfo.attackLeft;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeUp_start, manager->animranges.AnimationRangeUp_end))
			CurrentAnimation = &playerinfo.attackUp;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeUpright_start, manager->animranges.AnimationRangeUpright_end))
			CurrentAnimation = &playerinfo.attackUpright;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeUpleft_start, manager->animranges.AnimationRangeUpleft_end))
			CurrentAnimation = &playerinfo.attackUpleft;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeDown_start, manager->animranges.AnimationRangeDown_end))
			CurrentAnimation = &playerinfo.attackDown;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeDownright_start, manager->animranges.AnimationRangeDownright_end))
			CurrentAnimation = &playerinfo.attackDownright;

		else if (InRange(direction_x, direction_y, manager->animranges.AnimationRangeDownleft_start, manager->animranges.AnimationRangeDownleft_end))
			CurrentAnimation = &playerinfo.attackDownleft;
	}
	else
		PlayerState = PSTATE::NONE;
}

void j1Player::GetMovementAnimation()
{
	if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeRight_start, manager->animranges.AnimationRangeRight_end))
		CurrentAnimation = &playerinfo.moveRight;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeLeft_start, manager->animranges.AnimationRangeLeft_end)
		|| InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeLeft_end2, manager->animranges.AnimationRangeLeft_start2))
		CurrentAnimation = &playerinfo.moveLeft;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeUp_start, manager->animranges.AnimationRangeUp_end))
		CurrentAnimation = &playerinfo.moveUp;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeUpright_start, manager->animranges.AnimationRangeUpright_end))
		CurrentAnimation = &playerinfo.moveUpright;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeUpleft_start, manager->animranges.AnimationRangeUpleft_end))
		CurrentAnimation = &playerinfo.moveUpleft;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeDown_start, manager->animranges.AnimationRangeDown_end))
		CurrentAnimation = &playerinfo.moveDown;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeDownright_start, manager->animranges.AnimationRangeDownright_end))
		CurrentAnimation = &playerinfo.moveDownright;

	else if (InRange(LJdirection_x, LJdirection_y, manager->animranges.AnimationRangeDownleft_start, manager->animranges.AnimationRangeDownleft_end))
		CurrentAnimation = &playerinfo.moveDownleft;
}

void j1Player::GetIdleAnimation()
{
	if (CurrentAnimation == &playerinfo.moveRight || CurrentAnimation == &playerinfo.attackRight)
		CurrentAnimation = &playerinfo.idleRight;

	else if (CurrentAnimation == &playerinfo.moveLeft || CurrentAnimation == &playerinfo.attackLeft)
		CurrentAnimation = &playerinfo.idleLeft;

	else if (CurrentAnimation == &playerinfo.moveUp || CurrentAnimation == &playerinfo.attackUp)
		CurrentAnimation = &playerinfo.idleUp;

	else if (CurrentAnimation == &playerinfo.moveUpright || CurrentAnimation == &playerinfo.attackUpright)
		CurrentAnimation = &playerinfo.idleUpright;

	else if (CurrentAnimation == &playerinfo.moveUpleft || CurrentAnimation == &playerinfo.attackUpleft)
		CurrentAnimation = &playerinfo.idleUpleft;

	else if (CurrentAnimation == &playerinfo.moveDown || CurrentAnimation == &playerinfo.attackDown)
		CurrentAnimation = &playerinfo.idleDown;

	else if (CurrentAnimation == &playerinfo.moveDownright || CurrentAnimation == &playerinfo.attackDownright)
		CurrentAnimation = &playerinfo.idleDownright;

	else if (CurrentAnimation == &playerinfo.moveDownleft || CurrentAnimation == &playerinfo.attackDownleft)
		CurrentAnimation = &playerinfo.idleDownleft;

}


void j1Player::MoveX(float dt)
{
	// --- Performing Movement X ---

	if (abs(LJdirection_x) > multipliermin)
	{
		LJdirection_x *= Entityinfo.Speed*dt;

		if (RJinverted)
		{
			LJdirection_x *= -1;
		}

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

		if (RJinverted)
		{
			LJdirection_y *= -1;
		}

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

	//LOG("angle: %f", playerinfo.characterdata.basic_attack.angle);
	//LOG("direction_x: %f", RJdirection_x);
	//LOG("direction_y: %f", RJdirection_y);
}

void j1Player::HandleAttacks()
{
	// --- Attack according to input ---
	if (PlayerState == PSTATE::ATTACKING && basicTimer.ReadSec() > 0.5f)
	{
		// --- We reset the basic attack timer and the attack animation timer ---
		attackanimTimer.Start();
		basicTimer.Start();

		// --- Firing the particle and playing character specific fx ---
		last_particle = App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + 20, this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		App->audio->PlayFx(this->playerinfo.basic_fx);
	}

	if (App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_DOWN)
		superON = true;

	else if (superON && App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_UP)
	{
		superON = false;
		HandleSuperAttacks();
	}
}

void j1Player::HandleShield()
{
	// --- Shield according to input ---
	if ((App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN) && shieldTimer.ReadSec() > 10.0f)
	{
		shieldTimer.Start();
		shieldDuration.Start();
		shieldON = true;
		//LOG("shield on");
		GetIdleAnimation();
		CurrentShieldAnimation = &manager->shield_anim;
		manager->shieldEnd_anim.Reset();
	}
	else if ((App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN) && shieldON)
	{
		//LOG("shield off");
		CurrentShieldAnimation = &manager->shieldEnd_anim;
		manager->shield_anim.Reset();
		shieldON = false;
	}
	else if (shieldDuration.ReadSec() > 2.5f && shieldON)
	{
		//LOG("shield off");
		CurrentShieldAnimation = &manager->shieldEnd_anim;
		manager->shield_anim.Reset();
		shieldON = false;
	}
}

void j1Player::HandleSuperAttacks()
{
	switch (character)
	{
	case CHARACTER::WENDOLIN:
		Launch1stSuper();
		break;
	case  CHARACTER::SIMON:
		Launch2ndSuper();
		break;
	case  CHARACTER::TRAKT:
		Launch3rdSuper();
		break;
	case  CHARACTER::MELIADOUL:
		Launch4thSuper();
		break;
	default:
		break;
	}
}

void j1Player::BlitSuperAimPaths(float dt)
{
	switch (character)
	{
	case CHARACTER::WENDOLIN:
		App->view->PushQueue(3, manager->WendolinSuper_aimpath, this->Entityinfo.position.x - 107, this->Entityinfo.position.y - 120, SDL_Rect{0,0,260,260});
		break;
	case  CHARACTER::SIMON:
	
		break;
	case  CHARACTER::TRAKT:
		
		break;
	case  CHARACTER::MELIADOUL:
		
		break;
	default:
		break;
	}
}

void j1Player::Launch1stSuper()
{
	if (superTimer.ReadSec() > 5.0f)
	{
		for (int i = 1; i < 17; ++i)
		{
			playerinfo.basic_attack.angle = 22.5f*(M_PI / 180.0f)*i;
			App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + 10, this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		}

		superTimer.Start();
		App->audio->PlayFx(this->playerinfo.super_fx);
	}
}

void j1Player::Launch2ndSuper()
{
	if (superTimer.ReadSec() > 5.0f)
	{
		float damage_radius = 150.0f;

		if (last_particle != nullptr && last_particle->toDelete != true)
		{
			superTimer.Start();
			App->audio->PlayFx(this->playerinfo.super_fx);
			this->Future_position.x = last_particle->pos.x;
			this->Future_position.y = last_particle->pos.y;

			ComputeDistance2players();

			if (absoluteDistanceP1 < damage_radius && this != App->scene->player1)
				App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player1);

			if (absoluteDistanceP2 < damage_radius && this != App->scene->player2)
				App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player2);

			if (absoluteDistanceP3 < damage_radius && this != App->scene->player3)
				App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player3);

			if (absoluteDistanceP4 < damage_radius && this != App->scene->player4)
				App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player4);
		}
	}
}

void j1Player::Launch3rdSuper()
{
	superTimer.Start();
	App->audio->PlayFx(this->playerinfo.super_fx);

	if (this != App->scene->player1)
	{
		App->scene->player1->RJinverted = true;
		App->scene->player1->RJinversion.Start();
	}

	if (this != App->scene->player2)
	{
		App->scene->player2->RJinverted = true;
		App->scene->player2->RJinversion.Start();
	}

	if (this != App->scene->player3)
	{
		App->scene->player3->RJinverted = true;
		App->scene->player3->RJinversion.Start();
	}

	if (this != App->scene->player4)
	{
		App->scene->player4->RJinverted = true;
		App->scene->player4->RJinversion.Start();
	}
}

void j1Player::Launch4thSuper()
{
	if ((abs(RJAxisx_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisy_value) > JOYSTICK_DEAD_ZONE)
		&& superTimer.ReadSec() > 5.0f)
	{
		superTimer.Start();
		App->audio->PlayFx(this->playerinfo.super_fx);

		float angle = std::atan2(RJdirection_y, RJdirection_x) - 45.0f;

		// --- First round of axes ---
		for (int i = 1; i < 4; ++i)
		{
			playerinfo.basic_attack.angle = angle + 22.5f*(M_PI / 180.0f)*i;
			App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + 20, this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		}

		// --- 2nd round of axes ---
		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x / 1.5f;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y / 1.5f;

		playerinfo.basic_attack.angle = angle + 33.75*(M_PI / 180.0f);
		App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + 20, this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		playerinfo.basic_attack.angle = angle + 56.25*(M_PI / 180.0f);
		App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + 20, this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);

		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x * 1.5f;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y * 1.5f;

	}
}

bool j1Player::Update(float dt)
{
	HandleInput();

	if (abs(LJAxisx_value) > JOYSTICK_DEAD_ZONE
		|| abs(LJAxisy_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisx_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisy_value) > JOYSTICK_DEAD_ZONE
		|| App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT
		)
		HandleAnimations();

	if (PlayerState == PSTATE::IDLE)
		GetIdleAnimation();

	MoveX(dt);

	MoveY(dt);

	if (RJinversion.ReadSec() > 2.5f && RJinverted)
		RJinverted = false;

	// --- Adjust Player's Position ---
	this->Entityinfo.position = Future_position;


	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;

	if (PlayerPrintOnTop == true)
		App->view->PushQueue(7, this->playerinfo.tex, this->Entityinfo.position.x, this->Entityinfo.position.y - 44, CurrentAnimation->GetCurrentFrame(dt));
	else
		App->view->PushQueue(5, this->playerinfo.tex, this->Entityinfo.position.x, this->Entityinfo.position.y - 44, CurrentAnimation->GetCurrentFrame(dt));
	

	// --- IDCircle Animations ---
	if (!shieldON && superTimer.ReadSec() < 5 && shieldTimer.ReadSec() > 10.0f)
		App->view->PushQueue(4, this->manager->circlesprites, this->Entityinfo.position.x - 18, this->Entityinfo.position.y - 10, this->Entityinfo.IDCircleshield.GetCurrentFrame(dt));

	else if (!shieldON && superTimer.ReadSec() > 5 && shieldTimer.ReadSec() > 10.0f)
		App->view->PushQueue(4, this->manager->circlesprites, this->Entityinfo.position.x - 18, this->Entityinfo.position.y - 10, this->Entityinfo.IDCircleboth.GetCurrentFrame(dt));

	else if (shieldTimer.ReadSec() < 10.0f && superTimer.ReadSec() > 5)
		App->view->PushQueue(4, this->manager->circlesprites, this->Entityinfo.position.x - 18, this->Entityinfo.position.y - 10, this->Entityinfo.IDCirclesuper.GetCurrentFrame(dt));

	else
		App->view->PushQueue(4, this->manager->circlesprites, this->Entityinfo.position.x - 6, this->Entityinfo.position.y, this->Entityinfo.IDCircle.GetCurrentFrame(dt));


	if (shieldON || (CurrentShieldAnimation == &manager->shieldEnd_anim && CurrentShieldAnimation->Finished() == false))
	App->view->PushQueue(10, manager->shield_texture, this->Entityinfo.position.x - 12, this->Entityinfo.position.y - 44, CurrentShieldAnimation->GetCurrentFrame(dt));

	// --- Basic Attack aim path ---
	if(abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin)
	App->view->PushQueue(3, manager->aimpath, this->Entityinfo.position.x - 4, this->Entityinfo.position.y + 12, SDL_Rect{ 0,0,55,263 }, 0, 0, std::atan2(RJdirection_y, RJdirection_x) * (180.0f / M_PI) - 90.0f, 27.5, 0);

	// --- Super Attack ---
	if(superON)
	BlitSuperAimPaths(dt);


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

	if (!shieldON)
	{
		switch (to_check->type)
		{
		case COLLIDER_TYPE::COLLIDER_PARTICLE:
			CheckParticleCollision(entitycollider, to_check);
			break;
		case COLLIDER_TYPE::COLLIDER_STORM:
			float damage = (float)App->arena_interactions->GetStormDamage(int(ID));
			App->buff->ApplyEffect(&App->buff->effects[STORM], this->Entityinfo.my_j1Entity, damage);
			break;
		}
	}
	else
	{
		int shield_fx = rand() % 2 + 1;
		switch (shield_fx)
		{
		case 1:
			App->audio->PlayFx(App->audio->fxShieldHit1);
			break;
		case 2:
			App->audio->PlayFx(App->audio->fxShieldHit2);
			break;
		}
	}

	// --- On player death, deactivate it ---
	if (this->Entityinfo.health <= 0.0f)
	{
		P_rank = RANK::LOSER;
		this->active = false;
		this->Entityinfo.entitycoll->rect.x = 0;
		this->Entityinfo.entitycoll->rect.y = 0;

		App->audio->PlayFx(this->playerinfo.basic_fx);
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
	case COLLIDER_TYPE::COLLIDER_WATER:
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
	case COLLIDER_TYPE::COLLIDER_WATER:
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
	case COLLIDER_TYPE::COLLIDER_WATER:
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
	case COLLIDER_TYPE::COLLIDER_WATER:
		entitycollider->rect.y -= Intersection.h;
		break;
	}

}

void j1Player::CheckParticleCollision(Collider * entitycollider, const Collider * to_check)
{
	Particle* pcollided = App->particlesys->GetCollidedParticle(entitycollider, to_check);

	if (pcollided && pcollided->originplayer != this)
	{
		App->buff->ApplyEffect(pcollided->particle_effect, this);
		App->input->ShakeController(ID, 0.5, 100);
		App->buff->LimitAttributes(this);
		LOG("player life: %f", this->Entityinfo.health);

		// Play a random hurt effect
		int hurt_effect = rand() % 3 + 1;

		switch (hurt_effect)
		{
		case 1:
			App->audio->PlayFx(App->audio->fxHit1);
			break;
		case 2:
			App->audio->PlayFx(App->audio->fxHit2);
			break;
		case 3:
			App->audio->PlayFx(App->audio->fxHit3);
			break;
		}

		if (this->Entityinfo.health <= 0.0f)
		{
			pcollided->originplayer->kills++;
			App->input->ShakeController(ID, 1.0, 1000);
		}

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

void j1Player::AssignCharacter()
{
	switch (this->character)
	{
	case CHARACTER::WENDOLIN:
		this->playerinfo = manager->Wendolin;
		break;
	case CHARACTER::MELIADOUL:
		this->playerinfo = manager->Meliadoul;
		break;
	case CHARACTER::SIMON:
		this->playerinfo = manager->Simon;
		break;
	case CHARACTER::TRAKT:
		this->playerinfo = manager->Trakt;
		break;
	default:
		break;
	}
}

void j1Player::ComputeDistance2players()
{
	directionP1.x = App->scene->player1->Future_position.x - this->Future_position.x;
	directionP1.y = App->scene->player1->Future_position.y - this->Future_position.y;
	absoluteDistanceP1 = sqrtf(pow(directionP1.x, 2.0f) + pow(directionP1.y, 2.0f));

	directionP2.x = App->scene->player2->Future_position.x - this->Future_position.x;
	directionP2.y = App->scene->player2->Future_position.y - this->Future_position.y;
	absoluteDistanceP2 = sqrtf(pow(directionP2.x, 2.0f) + pow(directionP2.y, 2.0f));

	directionP3.x = App->scene->player3->Future_position.x - this->Future_position.x;
	directionP3.y = App->scene->player3->Future_position.y - this->Future_position.y;
	absoluteDistanceP3 = sqrtf(pow(directionP3.x, 2.0f) + pow(directionP3.y, 2.0f));

	directionP4.x = App->scene->player4->Future_position.x - this->Future_position.x;
	directionP4.y = App->scene->player4->Future_position.y - this->Future_position.y;
	absoluteDistanceP4 = sqrtf(pow(directionP4.x, 2.0f) + pow(directionP4.y, 2.0f));
}

const fPoint j1Player::GetNearestPlayerDirection()
{
	fPoint direction = { 0.0f,0.0f };
	directionP1 = { 0.0f,0.0f };
	directionP2 = { 0.0f,0.0f };
	directionP3 = { 0.0f,0.0f };
	directionP4 = { 0.0f,0.0f };

	float absoluteDistance = 0.0f;
	absoluteDistanceP1 = 0.0f;
	absoluteDistanceP2 = 0.0f;
	absoluteDistanceP3 = 0.0f;
	absoluteDistanceP4 = 0.0f;

	// --- Distances to all players ---
	ComputeDistance2players();

	// --- We need Absolute Distance to nearest player ---
	absoluteDistance = absoluteDistanceP1;

	if ((absoluteDistanceP2 < absoluteDistance && absoluteDistanceP2 != 0.0f) || absoluteDistance == 0.0f)
		absoluteDistance = absoluteDistanceP2;
	if (absoluteDistanceP3 < absoluteDistance && absoluteDistanceP3 != 0.0f)
		absoluteDistance = absoluteDistanceP3;
	if (absoluteDistanceP4 < absoluteDistance && absoluteDistanceP4 != 0.0f)
		absoluteDistance = absoluteDistanceP4;

	// --- If the nearest player is in the auto aim radius ---
	if (absoluteDistance < Aim_Radius)
	{
		if (absoluteDistance == absoluteDistanceP1)
			direction = directionP1;

		else if (absoluteDistance == absoluteDistanceP2)
			direction = directionP2;

		else if (absoluteDistance == absoluteDistanceP3)
			direction = directionP3;

		else if (absoluteDistance == absoluteDistanceP4)
			direction = directionP4;
	}

	return direction;
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

	HandleAttacks();

	HandleShield();

	PlayerState = PSTATE::IDLE;

	if (!shieldON)
		Update(dt);

}
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
#include "j1UIScene.h"
#include "j1Gui.h"
#include "j1FadeToBlack.h"
#include "UI_element.h"
#include "j1Transition.h"

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
	shieldAnim = manager->shield_anim;
	shieldendAnim = manager->shieldEnd_anim;

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

	Entityinfo.entitycoll->rect.w *= Entityinfo.scale;
	Entityinfo.entitycoll->rect.h *= Entityinfo.scale;

	Entityinfo.HitBox = App->coll->AddCollider(Entityinfo.entitycollrect, COLLIDER_TYPE::COLLIDER_HITBOX, (j1Module*)manager);
	Entityinfo.HitBox->SetPos(Entityinfo.position.x, Entityinfo.position.y);
	Entityinfo.HitBox->rect.w = 20;
	Entityinfo.HitBox->rect.h = 20;

	superTimer.Start();
	shieldTimer.Start();
	basicTimer.Start();
	Traktpulsation.Start();
	ghostTimer.Start();

	current_step = fade_step::none;
	current_stepD = fade_step::none;
	current_stepHP = fade_step::none;
	colA = { 255,0,0,75 };
	colB = { 0,0,0,255 };
	colC = { 255,255,255,255 };

	parryP.anim.PushBack(manager->parrytex_rect);
	parryP.tex = manager->parry_texture;
	parryP.life = manager->parryPlife;

	return true;
}

void j1Player::HandleAnimations()
{
	if (((App->input->GetButton(ID, BUTTON_BIND::BASIC_ATTACK) == KEY_REPEAT)
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

	LJdirection_x = (LJAxisx_value) / AXISMAX;
	LJdirection_y = (LJAxisy_value) / AXISMAX;

	// --- LOGIC: Right Joystick ---

	RJAxisx_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_RIGHTX);
	RJAxisy_value = App->input->GetAxis(ID, SDL_CONTROLLER_AXIS_RIGHTY);

	RJdirection_x = (RJAxisx_value) / AXISMAX;
	RJdirection_y = (RJAxisy_value) / AXISMAX;

	//--------------

	//LOG("angle: %f", playerinfo.characterdata.basic_attack.angle);
	//LOG("direction_x: %f", RJdirection_x);
	//LOG("direction_y: %f", RJdirection_y);

	if (App->input->GetButton(ID, SDL_CONTROLLER_BUTTON_START) == BUTTON_DOWN && (App->ui_scene->current_menu->id == INGAME_MENU 
		|| App->ui_scene->current_menu->id == INGAMESETTINGS_MENU))
	{
		App->input->ForceKeyboardKeyState(SDL_SCANCODE_ESCAPE, KEY_DOWN);
	}

	
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

		last_particle = App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + (int)(8 * Entityinfo.scale), this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);

		App->audio->PlayFx(this->playerinfo.basic_fx);
	}


	if (App->input->GetButton(ID, BUTTON_BIND::SUPER_ATTACK) == KEY_DOWN)
	{
		superON = true;
	}
	else if (superON && App->input->GetButton(ID, BUTTON_BIND::SUPER_ATTACK) == KEY_UP && superTimer.ReadSec() >= SuperCooldown)
	{
		superON = false;
		HandleSuperAttacks();
	}
	else if (superON && App->input->GetButton(ID, BUTTON_BIND::SUPER_ATTACK) != KEY_REPEAT)
		superON = false;

	LOG("t: %f", superTimer.ReadSec());
	// --- Special ability ---
	superTimer.Limit(SuperCooldown);

	if (App->input->GetButton(ID, BUTTON_BIND::SPECIAL_ATTACK) == KEY_DOWN)
		specialON = true;

	else if (specialON && App->input->GetButton(ID, BUTTON_BIND::SPECIAL_ATTACK) == KEY_UP)
	{
		specialON = false;
		HandleSpecialAttacks();
	}

	if (super_available == false && superTimer.ReadSec() >= SuperCooldown)
	{
		App->audio->PlayFx(App->audio->fxPowerUpActivate);
		super_available = true;
	}
}

void j1Player::HandleShield()
{
	bool previous_shield_available = shield_available;
	// --- Shield according to input ---

	if ((App->input->GetButton(ID, BUTTON_BIND::SHIELD) == KEY_DOWN) && shieldTimer.ReadSec() > 10.0f)
	{
		shieldTimer.Start();
		shieldDuration.Start();
		shieldON = true;
		//LOG("shield on");
		GetIdleAnimation();
		CurrentShieldAnimation = &shieldAnim;
		shieldendAnim.Reset();

		App->audio->PlayFx(App->audio->fxPowerUpAppear1);
		shield_available = false;
	}
	else if ((App->input->GetButton(ID, BUTTON_BIND::SHIELD) == KEY_DOWN) && shieldON)
	{
		//LOG("shield off");
		CurrentShieldAnimation = &shieldendAnim;
		shieldAnim.Reset();
		shieldON = false;
		App->audio->PlayFx(App->audio->fxCancel);
	}
	else if (shieldDuration.ReadSec() > 2.5f && shieldON)
	{
		//LOG("shield off");
		CurrentShieldAnimation = &shieldendAnim;
		shieldAnim.Reset();
		shieldON = false;
		App->audio->PlayFx(App->audio->fxCancel);
	}

	if (shield_available == false && shieldTimer.ReadSec() >= manager->ShieldCooldown)
	{
		App->audio->PlayFx(App->audio->fxPowerUpPick);
		shield_available = true;
	}
}

void j1Player::HandleSuperAttacks()
{
	switch (character)
	{
	case CHARACTER::WENDOLIN:
		Launch1stSuper();
		break;
	case CHARACTER::SIMON:
		Launch2ndSuper();
		break;
	case CHARACTER::TRAKT:
		Launch3rdSuper();
		break;
	case CHARACTER::MELIADOUL:
		Launch4thSuper();
		break;
	default:
		break;
	}
}

void j1Player::HandleSpecialAttacks()
{

	switch (character)
	{
	case CHARACTER::WENDOLIN:
		Launch1stSP();
		break;
	case CHARACTER::SIMON:
		Launch2ndSP();
		break;
	case CHARACTER::TRAKT:
		Launch3rdSP();
		break;
	case CHARACTER::MELIADOUL:
		Launch4thSP();
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

		break;
	case CHARACTER::SIMON:
		if (last_particle && !last_particle->toDelete)
		{
			if (App->ui_scene->actual_menu == SELECTION_MENU)
			App->view->PushQueue(5, manager->SimonSuper_aimpath, last_particle->pos.x - (int)(76 * Entityinfo.scale), last_particle->pos.y - (int)(76 * Entityinfo.scale), SDL_Rect{ 0,0,50,50 }, 1, 0, 0, 0, 0, 2);
			else
			App->view->PushQueue(5, manager->SimonSuper_aimpath, last_particle->pos.x - (int)(76 * Entityinfo.scale), last_particle->pos.y - (int)(76 * Entityinfo.scale), SDL_Rect{ 0,0,50,50 }, ((int)ID) + 1, 0, 0, 0, 0, 2);
		}
		break;
	case CHARACTER::TRAKT:
		if (App->ui_scene->actual_menu == SELECTION_MENU)
		App->view->PushQueue(5, manager->TraktSuper_aimpath, this->Entityinfo.position.x - (int)(240 * Entityinfo.scale), this->Entityinfo.position.y - (int)(250 * Entityinfo.scale), SDL_Rect{ 0,0,350,350 },1, 0, 0, 0, 0);
		else
		App->view->PushQueue(5, manager->TraktSuper_aimpath, this->Entityinfo.position.x - (int)(240 * Entityinfo.scale), this->Entityinfo.position.y - (int)(250 * Entityinfo.scale), SDL_Rect{ 0,0,350,350 }, ((int)ID) + 1, 0, 0, 0, 0);

		// --- All gamepads vibrate on trakt's super (heart pulsation) ---
		ComputeDistance2players();

		if (Traktpulsation.ReadSec() > 2.0f && superTimer.ReadSec() > SuperCooldown)
		{
			Traktpulsation.Start();
			if (absoluteDistanceP1 < 265.0f * Entityinfo.scale)
			App->input->ShakeController(PLAYER::P1, 0.25, 750);
			if (absoluteDistanceP2 < 265.0f * Entityinfo.scale)
			App->input->ShakeController(PLAYER::P2, 0.25, 750);
			if (absoluteDistanceP3 < 265.0f * Entityinfo.scale)
			App->input->ShakeController(PLAYER::P3, 0.25, 750);
			if (absoluteDistanceP4 < 265.0f * Entityinfo.scale)
			App->input->ShakeController(PLAYER::P4, 0.25, 750);
		}

		break;
	case CHARACTER::MELIADOUL:
	
		break;
	default:
		break;
	}
}

void j1Player::BlitSPAimPaths(float dt)
{
	switch (character)
	{
	case CHARACTER::WENDOLIN:
		if (App->ui_scene->actual_menu == SELECTION_MENU)
		App->view->PushQueue(5, manager->WendolinSuper_aimpath, this->Entityinfo.position.x - (int)(107.0f * Entityinfo.scale), this->Entityinfo.position.y - (int)(120.0f * Entityinfo.scale), SDL_Rect{ 0,0,260,260 }, 1, 0, 0, 0, 0, Entityinfo.scale, alpha);
		else
		App->view->PushQueue(5, manager->WendolinSuper_aimpath, this->Entityinfo.position.x - (int)(107.0f * Entityinfo.scale), this->Entityinfo.position.y - (int)(120.0f * Entityinfo.scale), SDL_Rect{0,0,260,260}, ((int)ID) + 1,0,0,0,0, Entityinfo.scale, alpha);
		break;
	case CHARACTER::SIMON:
		if (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin)
		{
			parryP.direction.x = (RJAxisx_value) / AXISMAX;
			parryP.direction.y = (RJAxisy_value) / AXISMAX;
			parryP.angle = std::atan2(parryP.direction.y, parryP.direction.x)*(180.0f / M_PI) + 135.0f;
			parryP.pos.x = this->Entityinfo.position.x - 20;
			parryP.pos.y = this->Entityinfo.position.y - 35;

			if (App->ui_scene->actual_menu == SELECTION_MENU)
				App->view->PushQueue(5, manager->parry_texture, this->Entityinfo.position.x - 20, this->Entityinfo.position.y - 35, manager->parrytex_rect, 1, 0, parryP.angle, manager->parrytex_rect.w/1.5, manager->parrytex_rect.h/1.5, Entityinfo.scale, alpha);
			else
				App->view->PushQueue(5, manager->parry_texture, this->Entityinfo.position.x - 20, this->Entityinfo.position.y - 35, manager->parrytex_rect, ((int)ID) + 1, 0, parryP.angle, manager->parrytex_rect.w / 1.5, manager->parrytex_rect.h / 1.5, Entityinfo.scale, alpha);
		}
		break;
	case CHARACTER::TRAKT:

		if (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin)
		{
			traktSPAngle = std::atan2(RJdirection_y, RJdirection_x)*(180.0f / M_PI) + 90.0f;

			TraktSPradius = 200;

			fPoint circle_pos;

			circle_pos.x = this->Entityinfo.position.x + cos((traktSPAngle - 90.0f) * (M_PI / 180.0f)) * abs(TraktSPradius);

			circle_pos.y = this->Entityinfo.position.y + sin((traktSPAngle - 90.0f) *(M_PI / 180.0f))* abs(TraktSPradius);

			if (App->ui_scene->actual_menu == SELECTION_MENU)
				App->view->PushQueue(5, manager->SimonSuper_aimpath, circle_pos.x - 35, circle_pos.y - 50, SDL_Rect{ 0,0,50,50 }, 1,0, 0, 0, 0, 2);
			else
				App->view->PushQueue(5, manager->SimonSuper_aimpath, circle_pos.x - 35, circle_pos.y - 50, SDL_Rect{ 0,0,50,50 }, ((int)ID) + 1, 0, 0, 0, 0, 2);

		}

		break;
	case CHARACTER::MELIADOUL:
		if (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin)
		{
			if (App->ui_scene->actual_menu == SELECTION_MENU)
				App->view->PushQueue(5, manager->MeliadoulSuper_aimpath, this->Entityinfo.position.x - (int)(75.0f * Entityinfo.scale), this->Entityinfo.position.y - (int)(200.0f * Entityinfo.scale), SDL_Rect{ 0,0,200,204 }, 1, 0, std::atan2(RJdirection_y, RJdirection_x) * (180.0f / M_PI) + 90.0f, 100 * Entityinfo.scale, 204 * Entityinfo.scale, Entityinfo.scale, alpha);
			else
				App->view->PushQueue(5, manager->MeliadoulSuper_aimpath, this->Entityinfo.position.x - (int)(75.0f * Entityinfo.scale), this->Entityinfo.position.y - (int)(200.0f * Entityinfo.scale), SDL_Rect{ 0,0,200,204 }, ((int)ID) + 1, 0, std::atan2(RJdirection_y, RJdirection_x) * (180.0f / M_PI) + 90.0f, 100 * Entityinfo.scale, 204 * Entityinfo.scale, Entityinfo.scale, alpha);
		}
		break;
	default:
		break;
	}
}

void j1Player::Launch1stSuper()
{
	if (superTimer.ReadSec() > SuperCooldown)
	{
		ghost = true;

		ghostTimer.Start();
		superTimer.Start();
		App->audio->PlayFx(this->playerinfo.super_fx);
		super_available = false;
	}
}

void j1Player::Launch2ndSuper()
{
	if (superTimer.ReadSec() > SuperCooldown)
	{
		float damage_radius = 76.0f * Entityinfo.scale;

		if (last_particle != nullptr && last_particle->toDelete != true)
		{
			teleported = true;
			superTimer.Start();
			App->audio->PlayFx(this->playerinfo.super_fx);
			this->Future_position.x = last_particle->pos.x;
			this->Future_position.y = last_particle->pos.y;

			ComputeDistance2players();

			if (App->ui_scene->actual_menu != SELECTION_MENU)
			{
				if (absoluteDistanceP1 < damage_radius && this != App->scene->player1 && !App->scene->player1->shieldON)
				{
					App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player1);
					App->scene->player1->damage_received = true;
				}
			
				if (App->scene->player1->Entityinfo.health <= 0 && App->scene->player1->active)
					this->kills++;

				if (absoluteDistanceP2 < damage_radius && this != App->scene->player2 && !App->scene->player2->shieldON)
				{
					App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player2);
					App->scene->player2->damage_received = true;
				}

				if (App->scene->player2->Entityinfo.health <= 0 && App->scene->player2->active)
					this->kills++;

				if (absoluteDistanceP3 < damage_radius && this != App->scene->player3 && !App->scene->player3->shieldON)
				{
					App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player3);
					App->scene->player3->damage_received = true;
				}

				if (App->scene->player3->Entityinfo.health <= 0 && App->scene->player3->active)
					this->kills++;

				if (absoluteDistanceP4 < damage_radius && this != App->scene->player4 && !App->scene->player4->shieldON)
				{
					App->buff->ApplyEffect(&App->buff->effects[Effects::SIMON_SUPER], App->scene->player4);
					App->scene->player4->damage_received = true;
				}

				if (App->scene->player4->Entityinfo.health <= 0 && App->scene->player4->active)
					this->kills++;

			}
			super_available = false;
		}
	}
}

void j1Player::Launch3rdSuper()
{
	if (superTimer.ReadSec() > SuperCooldown)
	{
		superTimer.Start();
		App->audio->PlayFx(this->playerinfo.super_fx);

		float radius = 265.0f * Entityinfo.scale;

		ComputeDistance2players();

		if (this != App->scene->player1 && absoluteDistanceP1 < radius && !App->scene->player1->shieldON && App->scene->player1->active)
		{
			App->scene->player1->RJinverted = true;
			App->scene->player1->RJinversion.Start();
		}

		if (this != App->scene->player2 && absoluteDistanceP2 < radius && !App->scene->player2->shieldON && App->scene->player2->active)
		{
			App->scene->player2->RJinverted = true;
			App->scene->player2->RJinversion.Start();
		}

		if (this != App->scene->player3 && absoluteDistanceP3 < radius && !App->scene->player3->shieldON && App->scene->player3->active)
		{
			App->scene->player3->RJinverted = true;
			App->scene->player3->RJinversion.Start();
		}

		if (this != App->scene->player4 && absoluteDistanceP4 < radius && !App->scene->player4->shieldON && App->scene->player4->active)
		{
			App->scene->player4->RJinverted = true;
			App->scene->player4->RJinversion.Start();
		}
		super_available = false;
	}
}

void j1Player::Launch4thSuper()
{
	if (superTimer.ReadSec() > SuperCooldown)
	{

		std::list<Particle*>::iterator item = MeliadoulAXES.begin();

		if (MeliadoulAXES.size() > 0)
		{
			superTimer.Start();
			App->audio->PlayFx(this->playerinfo.super_fx);

			while (item != MeliadoulAXES.end())
			{
				if (*item)
				{
					(*item)->life = 2000;
					(*item)->direction.x = this->Entityinfo.position.x - (*item)->pos.x;
					(*item)->direction.y = this->Entityinfo.position.y - (*item)->pos.y;
					(*item)->particle_effect = &App->buff->effects[3];
					(*item)->speed.x = 200.0f;
					(*item)->speed.y = 200.0f;
					(*item)->angle = std::atan2((*item)->direction.y, (*item)->direction.x);

					if ((*item)->pCol && !(*item)->pCol->to_delete)
						(*item)->pCol->type = COLLIDER_TYPE::COLLIDER_PARTICLE;
					else
					{
						(*item)->pCol = App->coll->AddCollider((*item)->anim.GetCurrentFrame(0), COLLIDER_TYPE::COLLIDER_PARTICLE, App->particlesys);
						(*item)->pCol->rect.x = (*item)->pos.x;
						(*item)->pCol->rect.y = (*item)->pos.y;
						(*item)->pCol->rect.w *= App->particlesys->scale;
						(*item)->pCol->rect.h *= App->particlesys->scale;
					}
					(*item)->returned = true;
					(*item)->born = SDL_GetTicks();
				}

				item++;
			}
			super_available = false;
		}

	}
}

void j1Player::Launch1stSP()
{
	if (superTimer.ReadSec() > SuperCooldown/2)
	{
		superTimer.Subtract(SuperCooldown/2);

		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x * manager->WendolinSP_speedmultiplier;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y * manager->WendolinSP_speedmultiplier;

		for (int i = 1; i < manager->WendolinSP_NumDaggers + 1; ++i)
		{
			playerinfo.basic_attack.angle = manager->WendolinSP_DaggerAngle*(M_PI / 180.0f)*i;
			App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + (int)(8.0f * Entityinfo.scale), this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		}

		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x / manager->WendolinSP_speedmultiplier;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y / manager->WendolinSP_speedmultiplier;
	}
}

void j1Player::Launch2ndSP()
{
	if (superTimer.ReadSec() > SuperCooldown / 2 && (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin))
	{
		superTimer.Subtract(SuperCooldown / 2);

		App->particlesys->AddParticle(parryP, parryP.pos.x, parryP.pos.y, COLLIDER_TYPE::COLLIDER_BOUNCE, 0, this);
	}
}

void j1Player::Launch3rdSP()
{
	if (superTimer.ReadSec() > SuperCooldown / 2)
	{
		superTimer.Subtract(SuperCooldown / 2);

		fPoint circle_pos;

		circle_pos.x = this->Entityinfo.position.x + cos((traktSPAngle - 90.0f) * (M_PI / 180.0f)) * abs(TraktSPradius) - 35 + 50;

		circle_pos.y = this->Entityinfo.position.y + sin((traktSPAngle - 90.0f) *(M_PI / 180.0f))* abs(TraktSPradius) - 50 + 50;

		ComputeDistance2players(circle_pos);

		if (this != App->scene->player1 && absoluteDistanceP1 < 50 && !App->scene->player1->shieldON && App->scene->player1->active)
		{
			App->buff->ApplyEffect(&App->buff->effects[Effects::EXHAUST], App->scene->player1);
		}

		if (this != App->scene->player2 && absoluteDistanceP2 < 50 && !App->scene->player2->shieldON && App->scene->player2->active)
		{
			App->buff->ApplyEffect(&App->buff->effects[Effects::EXHAUST], App->scene->player2);
		}

		if (this != App->scene->player3 && absoluteDistanceP3 < 50 && !App->scene->player3->shieldON && App->scene->player3->active)
		{
			App->buff->ApplyEffect(&App->buff->effects[Effects::EXHAUST], App->scene->player3);
		}

		if (this != App->scene->player4 && absoluteDistanceP4 < 50 && !App->scene->player4->shieldON && App->scene->player4->active)
		{
			App->buff->ApplyEffect(&App->buff->effects[Effects::EXHAUST], App->scene->player4);
		}

		ComputeDistance2players();
	}
}

void j1Player::Launch4thSP()
{
	if (superTimer.ReadSec() > SuperCooldown / 2)
	{
		superTimer.Subtract(SuperCooldown / 2);

		fPoint direction = { 0.0f,0.0f };

		if (!(abs(RJAxisx_value) > JOYSTICK_DEAD_ZONE
			|| abs(RJAxisy_value) > JOYSTICK_DEAD_ZONE))
		{
			direction = GetNearestPlayerDirection();
		}
		else
		{
			direction.x = RJdirection_x;
			direction.y = RJdirection_y;
		}

		float angle = std::atan2(direction.y, direction.x) - 45.0f*(M_PI / 180.0f);

		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x * 1.5f;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y * 1.5f;

		// --- First round of axes ---
		for (int i = 1; i < 4; ++i)
		{
			playerinfo.basic_attack.angle = angle + 22.5f*(M_PI / 180.0f)*i;
			App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + (int)(8.0f*Entityinfo.scale), this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		}

		// --- 2nd round of axes ---
		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x / 1.5f;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y / 1.5f;

		playerinfo.basic_attack.angle = angle + 33.75*(M_PI / 180.0f);
		App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + (int)(8.0f*Entityinfo.scale), this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);
		playerinfo.basic_attack.angle = angle + 56.25*(M_PI / 180.0f);
		App->particlesys->AddParticle(playerinfo.basic_attack, this->Entityinfo.position.x + (int)(8.0f*Entityinfo.scale), this->Entityinfo.position.y, COLLIDER_TYPE::COLLIDER_PARTICLE, 0, this);

		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x * 1.5f;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y * 1.5f;

		playerinfo.basic_attack.speed.x = playerinfo.basic_attack.speed.x / 1.5f;
		playerinfo.basic_attack.speed.y = playerinfo.basic_attack.speed.y / 1.5f;
	}
}

bool j1Player::Update(float dt)
{
	HandleInput();

	if (abs(LJAxisx_value) > JOYSTICK_DEAD_ZONE
		|| abs(LJAxisy_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisx_value) > JOYSTICK_DEAD_ZONE
		|| abs(RJAxisy_value) > JOYSTICK_DEAD_ZONE
		|| App->input->GetButton(ID, BUTTON_BIND::BASIC_ATTACK) == KEY_REPEAT
		)
		HandleAnimations();

	if (PlayerState == PSTATE::IDLE)
		GetIdleAnimation();

	MoveX(dt);

	MoveY(dt);

	Entityinfo.HitBox->SetPos(this->Entityinfo.entitycoll->rect.x + Entityinfo.HitBox->rect.w / 4, this->Entityinfo.entitycoll->rect.y - Entityinfo.HitBox->rect.h / 2);
	App->coll->QueryCollisions(*Entityinfo.HitBox);

	if (RJinversion.ReadSec() > 2.5f && RJinverted)
		RJinverted = false;


	// --- Adjust Player's Position ---
	this->Entityinfo.position = Future_position;
	teleported = false;

	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;


	// --- Adapt to Transitions ---
	if (App->transition->doingMenuTransition)
	{
		SDL_SetTextureAlphaMod(this->playerinfo.tex, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->circlesprites, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->arrows_tex, App->gui->alpha_value);

		SDL_SetTextureAlphaMod(this->manager->MeliadoulSuper_aimpath, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->WendolinSuper_aimpath, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->TraktSuper_aimpath, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->SimonSuper_aimpath, App->gui->alpha_value);

		SDL_SetTextureAlphaMod(this->manager->Dagger_texture, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->axe_texture, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->inkball_texture, App->gui->alpha_value);
		SDL_SetTextureAlphaMod(this->manager->budu_texture, App->gui->alpha_value);

		alpha = App->gui->alpha_value;
	}

	// --- Blit player ---

	if (ghost)
		alpha = manager->Wendolin_ghostalpha;
	else if (!App->transition->doingMenuTransition)
		alpha = 255;

	if (!ghost)
	{
		if (PlayerPrintOnTop == true)
			App->view->PushQueue(9, this->playerinfo.tex, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(50 * Entityinfo.scale), CurrentAnimation->GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale,alpha);
		else
			App->view->PushQueue(7, this->playerinfo.tex, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(50 * Entityinfo.scale), CurrentAnimation->GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale,alpha);
	}

		if (App->view->number_of_views == 1)
		{
			if (PlayerPrintOnTop == true)
				App->view->PushQueue(9, this->playerinfo.tex, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(50 * Entityinfo.scale), CurrentAnimation->GetCurrentFrame(dt),0, 0, 0, 0, 0, Entityinfo.scale, alpha);
			else
				App->view->PushQueue(7, this->playerinfo.tex, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(50 * Entityinfo.scale), CurrentAnimation->GetCurrentFrame(dt),0, 0, 0, 0, 0, Entityinfo.scale, alpha);
		}
		else if ((App->view->number_of_views == 4))
		{
			if (PlayerPrintOnTop == true)
				App->view->PushQueue(9, this->playerinfo.tex, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(50 * Entityinfo.scale), CurrentAnimation->GetCurrentFrame(dt), (int)(ID)+1, 0, 0, 0, 0, Entityinfo.scale, alpha);
			else
				App->view->PushQueue(7, this->playerinfo.tex, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(50 * Entityinfo.scale), CurrentAnimation->GetCurrentFrame(dt), (int)(ID)+1, 0, 0, 0, 0, Entityinfo.scale, alpha);
		}
	
		if (!ghost)
		{
			// --- IDCircle Animations ---
			if (!shieldON && shieldTimer.ReadSec() > manager->ShieldCooldown)
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(23 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(15 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircleshield.GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale*1.2f,alpha);
			else
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), (int)(ID)+1, 0, 0, 0, 0, Entityinfo.scale*1.2f,alpha);



			switch (ID)
			{
			case PLAYER::P1:
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 1, 0, 0, 0, 0, Entityinfo.scale*1.2f,alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 2, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 3, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				break;
			case PLAYER::P2:
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 2, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 3, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				break;
			case PLAYER::P3:
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 1, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 3, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				break;
			case PLAYER::P4:
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 1, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				App->view->PushQueue(6, this->manager->circlesprites, this->Entityinfo.position.x - (int)(10 * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(6 * Entityinfo.scale*1.2f), this->Entityinfo.IDCircle.GetCurrentFrame(dt), 2, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);
				break;
			default:
				break;
			}


			if (shieldON || (CurrentShieldAnimation == &shieldendAnim && CurrentShieldAnimation->Finished() == false))
				App->view->PushQueue(10, manager->shield_texture, this->Entityinfo.position.x - (int)(17.0f * Entityinfo.scale*1.2f), this->Entityinfo.position.y - (int)(44.0f * Entityinfo.scale*1.2f), CurrentShieldAnimation->GetCurrentFrame(dt), 0, 0, 0, 0, 0, Entityinfo.scale*1.2f, alpha);

		}

	// --- Basic Attack aim path ---
	if (abs(RJdirection_x) > multipliermin || abs(RJdirection_y) > multipliermin)
	{
		
			bool blit_aimpath = true;

			if (specialON && this->character != CHARACTER::SIMON)
				blit_aimpath = false;

			if (blit_aimpath)
			{
				if (App->ui_scene->actual_menu == SELECTION_MENU)
				App->view->PushQueue(5, manager->aimpath, this->Entityinfo.position.x - (int)(4 * Entityinfo.scale), this->Entityinfo.position.y + (int)(12 * Entityinfo.scale), SDL_Rect{ 0,0,55,263 }, 1, 0, std::atan2(RJdirection_y, RJdirection_x) * (180.0f / M_PI) - 90.0f, 27.5 * Entityinfo.scale, 0, Entityinfo.scale, alpha);
				else
				App->view->PushQueue(5, manager->aimpath, this->Entityinfo.position.x - (int)(4 * Entityinfo.scale), this->Entityinfo.position.y + (int)(12 * Entityinfo.scale), SDL_Rect{ 0,0,55,263 }, ((int)ID) + 1, 0, std::atan2(RJdirection_y, RJdirection_x) * (180.0f / M_PI) - 90.0f, 27.5 * Entityinfo.scale, 0, Entityinfo.scale, alpha);
			}
	}
	// --- Super Attack ---
	if(superON)
		BlitSuperAimPaths(dt);

	if (specialON)
		BlitSPAimPaths(dt);

	BlitArrows();

	SDL_Rect tmp;

	switch (ID)
	{
	case PLAYER::P1:
		App->fade->PostUpdate(App->view->four_views_1, start_time, total_time, current_step,colA,alphaA);

		tmp = App->ui_scene->hp_bar1->section;
		tmp.x = App->ui_scene->hp_bar1->localPosition.x;
		tmp.y = App->ui_scene->hp_bar1->localPosition.y;

		App->fade->PostUpdate(tmp, start_timeHP, total_timeHP, current_stepHP, colC, alphaC,nullptr,0);

		break;
	case PLAYER::P2:
		App->fade->PostUpdate(App->view->four_views_2, start_time, total_time, current_step, colA, alphaA);

		tmp = App->ui_scene->hp_bar2->section;
		tmp.x = App->ui_scene->hp_bar2->localPosition.x;
		tmp.y = App->ui_scene->hp_bar2->localPosition.y;

		App->fade->PostUpdate(tmp, start_timeHP, total_timeHP, current_stepHP, colC, alphaC, nullptr, 0);
		break;
	case PLAYER::P3:
		App->fade->PostUpdate(App->view->four_views_3, start_time, total_time, current_step, colA, alphaA);

		tmp = App->ui_scene->hp_bar3->section;
		tmp.x = App->ui_scene->hp_bar3->localPosition.x;
		tmp.y = App->ui_scene->hp_bar3->localPosition.y;

		App->fade->PostUpdate(tmp, start_timeHP, total_timeHP, current_stepHP, colC, alphaC, nullptr, 0);
		break;
	case PLAYER::P4:
		App->fade->PostUpdate(App->view->four_views_4, start_time, total_time, current_step, colA, alphaA);

		tmp = App->ui_scene->hp_bar4->section;
		tmp.x = App->ui_scene->hp_bar4->localPosition.x;
		tmp.y = App->ui_scene->hp_bar4->localPosition.y;

		App->fade->PostUpdate(tmp, start_timeHP, total_timeHP, current_stepHP, colC, alphaC, nullptr, 0);
		break;
	default:
		break;
	}

	// --- Updating SP Effects ---
	switch (character)
	{
	case CHARACTER::WENDOLIN:
		if (ghost && ghostTimer.ReadSec() >= manager->Wendolin_ghostTime)
		{
			ghost = false;
		}
		break;

	default:
		break;
	}

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

	switch (entitycollider->type)
	{
	case COLLIDER_TYPE::COLLIDER_PLAYER:

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

		break;

	case COLLIDER_TYPE::COLLIDER_HITBOX:

		if (!shieldON)
		{
			switch (to_check->type)
			{
			case COLLIDER_TYPE::COLLIDER_PARTICLE:
				CheckParticleCollision(Entityinfo.HitBox, to_check);
				break;
			case COLLIDER_TYPE::COLLIDER_STORM:
				float damage = (float)App->arena_interactions->GetStormDamage(int(ID));
				App->buff->ApplyEffect(&App->buff->effects[STORM], this->Entityinfo.my_j1Entity, damage);

				if (this->Entityinfo.health <= 0.0f)
				{
					if (last_hitP)
						last_hitP->kills++;

					App->input->ShakeController(ID, 1.0, 1000);
				}

				if(damage != 0)
				damage_received = true;
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


		break;
	}

}

void j1Player::Right_Collision(Collider * entitycollider, const Collider * to_check)
{
	SDL_IntersectRect(&entitycollider->rect, &to_check->rect, &Intersection);

	switch (to_check->type)
	{
	case COLLIDER_TYPE::COLLIDER_FLOOR:
		entitycollider->rect.x -= Intersection.w;
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_FALLENAXE:
		entitycollider->rect.x -= Intersection.w;
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_WATER:
		if (teleported)
		{
			Future_position.x = Entityinfo.position.x;
			Future_position.y = Entityinfo.position.y;
		}
		else
		{
			entitycollider->rect.x -= Intersection.w;
			Future_position.x = entitycollider->rect.x;
			Future_position.y = entitycollider->rect.y;
		}

		break;
	default:

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
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_FALLENAXE:
		entitycollider->rect.x += Intersection.w;
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_WATER:
		if (teleported)
		{
			Future_position.x = Entityinfo.position.x;
			Future_position.y = Entityinfo.position.y;
		}
		else
		{
			entitycollider->rect.x += Intersection.w;
			Future_position.x = entitycollider->rect.x;
			Future_position.y = entitycollider->rect.y;
		}
		break;
	default:

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
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_FALLENAXE:
		entitycollider->rect.y += Intersection.h;
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_WATER:
		if (teleported)
		{
			Future_position.x = Entityinfo.position.x;
			Future_position.y = Entityinfo.position.y;
		}
		else
		{
			entitycollider->rect.y += Intersection.h;
			Future_position.x = entitycollider->rect.x;
			Future_position.y = entitycollider->rect.y;
		}
		break;
	default:

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
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_FALLENAXE:
		entitycollider->rect.y -= Intersection.h;
		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
		break;
	case COLLIDER_TYPE::COLLIDER_WATER:
		if (teleported)
		{
			Future_position.x = Entityinfo.position.x;
			Future_position.y = Entityinfo.position.y;
		}
		else
		{
			entitycollider->rect.y -= Intersection.h;
			Future_position.x = entitycollider->rect.x;
			Future_position.y = entitycollider->rect.y;
		}
		break;
	default:

		break;
	}

}

void j1Player::CheckParticleCollision(Collider * hitbox, const Collider * to_check)
{
	Particle* pcollided = nullptr;

	pcollided = App->particlesys->GetCollidedParticle(Entityinfo.HitBox, to_check);

	if (pcollided && pcollided->originplayer != this && this->active)
	{
		if (this->Entityinfo.health > 0.0f && !AreOtherPlayersDead())
		{
			if (App->ui_scene->actual_menu != SELECTION_MENU)
			{
				damage_received = true;
				App->buff->ApplyEffect(pcollided->particle_effect, this);
				last_hitP = pcollided->originplayer;
			}

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
	ghost = false;

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

void j1Player::ComputeDistance2players(fPoint pos)
{
	if (pos.x == -1 && pos.y == -1)
	{

		directionP1.x = App->scene->player1->Future_position.x + App->scene->player1->Entityinfo.HitBox->rect.w / 2 - (this->Future_position.x + Entityinfo.HitBox->rect.w / 2);
		directionP1.y = App->scene->player1->Future_position.y + App->scene->player1->Entityinfo.HitBox->rect.h / 2 - (this->Future_position.y + Entityinfo.HitBox->rect.h / 2);
		absoluteDistanceP1 = sqrtf(pow(directionP1.x, 2.0f) + pow(directionP1.y, 2.0f));

		directionP2.x = App->scene->player2->Future_position.x + App->scene->player2->Entityinfo.HitBox->rect.w / 2 - (this->Future_position.x + Entityinfo.HitBox->rect.w / 2);
		directionP2.y = App->scene->player2->Future_position.y + App->scene->player2->Entityinfo.HitBox->rect.h / 2 - (this->Future_position.y + Entityinfo.HitBox->rect.h / 2);
		absoluteDistanceP2 = sqrtf(pow(directionP2.x, 2.0f) + pow(directionP2.y, 2.0f));

		directionP3.x = App->scene->player3->Future_position.x + App->scene->player3->Entityinfo.HitBox->rect.w / 2 - (this->Future_position.x + Entityinfo.HitBox->rect.w / 2);
		directionP3.y = App->scene->player3->Future_position.y + App->scene->player3->Entityinfo.HitBox->rect.h / 2 - (this->Future_position.y + Entityinfo.HitBox->rect.h / 2);
		absoluteDistanceP3 = sqrtf(pow(directionP3.x, 2.0f) + pow(directionP3.y, 2.0f));

		directionP4.x = App->scene->player4->Future_position.x + App->scene->player4->Entityinfo.HitBox->rect.w / 2 - (this->Future_position.x + Entityinfo.HitBox->rect.w / 2);
		directionP4.y = App->scene->player4->Future_position.y + App->scene->player4->Entityinfo.HitBox->rect.h / 2 - (this->Future_position.y + Entityinfo.HitBox->rect.h / 2);
		absoluteDistanceP4 = sqrtf(pow(directionP4.x, 2.0f) + pow(directionP4.y, 2.0f));

	}
	else
	{
		directionP1.x = App->scene->player1->Future_position.x + App->scene->player1->Entityinfo.HitBox->rect.w / 2 - (pos.x);
		directionP1.y = App->scene->player1->Future_position.y + App->scene->player1->Entityinfo.HitBox->rect.h / 2 - (pos.y);
		absoluteDistanceP1 = sqrtf(pow(directionP1.x, 2.0f) + pow(directionP1.y, 2.0f));

		directionP2.x = App->scene->player2->Future_position.x + App->scene->player2->Entityinfo.HitBox->rect.w / 2 - (pos.x);
		directionP2.y = App->scene->player2->Future_position.y + App->scene->player2->Entityinfo.HitBox->rect.h / 2 - (pos.y);
		absoluteDistanceP2 = sqrtf(pow(directionP2.x, 2.0f) + pow(directionP2.y, 2.0f));

		directionP3.x = App->scene->player3->Future_position.x + App->scene->player3->Entityinfo.HitBox->rect.w / 2 - (pos.x);
		directionP3.y = App->scene->player3->Future_position.y + App->scene->player3->Entityinfo.HitBox->rect.h / 2 - (pos.y);
		absoluteDistanceP3 = sqrtf(pow(directionP3.x, 2.0f) + pow(directionP3.y, 2.0f));

		directionP4.x = App->scene->player4->Future_position.x + App->scene->player4->Entityinfo.HitBox->rect.w / 2 - (pos.x);
		directionP4.y = App->scene->player4->Future_position.y + App->scene->player4->Entityinfo.HitBox->rect.h / 2 - (pos.y);
		absoluteDistanceP4 = sqrtf(pow(directionP4.x, 2.0f) + pow(directionP4.y, 2.0f));
	}
}

bool j1Player::AreOtherPlayersDead()
{
	bool ret = false;
	uint dead = 0;

	if (this != App->scene->player1 && !App->scene->player1->active)
		dead++;
	if (this != App->scene->player2 && !App->scene->player2->active)
		dead++;
	if (this != App->scene->player3 && !App->scene->player3->active)
		dead++;
	if (this != App->scene->player4 && !App->scene->player4->active)
		dead++;

	if (dead == 3)
		ret = true;

	return ret;
}

void j1Player::BlitArrows()
{
	if (App->ui_scene->actual_menu != SELECTION_MENU)
	{
		ComputeDistance2players();

		uint width, height = 0;
		App->win->GetWindowSize(width, height);
		width = width / 2;

		uint radius = 75;

		Entityinfo.scale = 1.0f;

		switch (ID)
		{
		case PLAYER::P1:
			if (absoluteDistanceP2 > width / 2 && App->scene->player2->active && !App->scene->player2->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->blue_arrow, ((int)ID) + 1, 0, std::atan2(directionP2.y, directionP2.x)* (180.0f / M_PI) + 90.0f, (manager->blue_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP3 > width / 2 && App->scene->player3->active && !App->scene->player3->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->yellow_arrow, ((int)ID) + 1, 0, std::atan2(directionP3.y, directionP3.x)* (180.0f / M_PI) + 90.0f, (manager->yellow_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP4 > width / 2 && App->scene->player4->active && !App->scene->player4->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->green_arrow, ((int)ID) + 1, 0, std::atan2(directionP4.y, directionP4.x)* (180.0f / M_PI) + 90.0f, (manager->green_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			break;
		case PLAYER::P2:
			if (absoluteDistanceP1 > width / 2 && App->scene->player1->active && !App->scene->player1->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->red_arrow, ((int)ID) + 1, 0, std::atan2(directionP1.y, directionP1.x)* (180.0f / M_PI) + 90.0f, (manager->red_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP3 > width / 2 && App->scene->player3->active && !App->scene->player3->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->yellow_arrow, ((int)ID) + 1, 0, std::atan2(directionP3.y, directionP3.x)* (180.0f / M_PI) + 90.0f, (manager->yellow_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP4 > width / 2 && App->scene->player4->active && !App->scene->player4->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->green_arrow, ((int)ID) + 1, 0, std::atan2(directionP4.y, directionP4.x)* (180.0f / M_PI) + 90.0f, (manager->green_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			break;
		case PLAYER::P3:
			if (absoluteDistanceP1 > width / 2 && App->scene->player1->active && !App->scene->player1->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->red_arrow, ((int)ID) + 1, 0, std::atan2(directionP1.y, directionP1.x)* (180.0f / M_PI) + 90.0f, (manager->red_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP2 > width / 2 && App->scene->player2->active && !App->scene->player2->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->blue_arrow, ((int)ID) + 1, 0, std::atan2(directionP2.y, directionP2.x)* (180.0f / M_PI) + 90.0f, (manager->blue_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP4 > width / 2 && App->scene->player4->active && !App->scene->player4->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->green_arrow, ((int)ID) + 1, 0, std::atan2(directionP4.y, directionP4.x)* (180.0f / M_PI) + 90.0f, (manager->green_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			break;
		case PLAYER::P4:
			if (absoluteDistanceP1 > width / 2 && App->scene->player1->active && !App->scene->player1->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->red_arrow, ((int)ID) + 1, 0, std::atan2(directionP1.y, directionP1.x)* (180.0f / M_PI) + 90.0f, (manager->red_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP2 > width / 2 && App->scene->player2->active && !App->scene->player2->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->blue_arrow, ((int)ID) + 1, 0, std::atan2(directionP2.y, directionP2.x)* (180.0f / M_PI) + 90.0f, (manager->blue_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			if (absoluteDistanceP3 > width / 2 && App->scene->player3->active && !App->scene->player3->ghost)
				App->view->PushQueue(11, this->manager->arrows_tex, this->Entityinfo.position.x + (int)(10 * Entityinfo.scale), this->Entityinfo.position.y - (int)(radius * Entityinfo.scale), manager->yellow_arrow, ((int)ID) + 1, 0, std::atan2(directionP3.y, directionP3.x)* (180.0f / M_PI) + 90.0f, (manager->yellow_arrow.w / 2 * Entityinfo.scale), (int)(radius * Entityinfo.scale), Entityinfo.scale, alpha);

			break;
		}

		Entityinfo.scale = 0.66f;

	}

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
	if(App->scene->player1->active && !App->scene->player1->ghost)
	absoluteDistance = absoluteDistanceP1;

	if (((absoluteDistanceP2 < absoluteDistance && absoluteDistanceP2 != 0.0f) || absoluteDistance == 0.0f) && App->scene->player2->active && !App->scene->player2->ghost)
		absoluteDistance = absoluteDistanceP2;
	if (((absoluteDistanceP3 < absoluteDistance && absoluteDistanceP3 != 0.0f) || absoluteDistance == 0.0f) && App->scene->player3->active && !App->scene->player3->ghost)
		absoluteDistance = absoluteDistanceP3;
	if (((absoluteDistanceP4 < absoluteDistance && absoluteDistanceP4 != 0.0f) || absoluteDistance == 0.0f) && App->scene->player4->active && !App->scene->player4->ghost)
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


	MeliadoulAXES.clear();

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

	App->buff->RestartAttribute(&App->buff->effects[EXHAUST], this);

	std::list<Particle*>::iterator item = MeliadoulAXES.begin();

	while (item != MeliadoulAXES.end())
	{
		if (*item && (*item)->toDelete)
		{
			MeliadoulAXES.erase(item);
		}

		item++;
	}

	if (dt != 0.0f)
	{
		if (this->current_stepD == fade_step::maintain)
			this->current_stepD = fade_step::none;

		EntityMovement = MOVEMENT::STATIC;

		HandleAttacks();

		HandleShield();

		PlayerState = PSTATE::IDLE;

		if (!shieldON)
			Update(dt);

		if (damage_received)
		{
			damage_received = false;
			App->fade->FadeCustom(255, 0, 0, alphaA, 0.01f, start_time, total_time, current_step, colA);
			App->fade->FadeCustom(colC.r, colC.g, colC.b, alphaC, 0.0001f, start_timeHP, total_timeHP, current_stepHP, colC);
		}

		// --- On player death, deactivate it ---
		if (this->Entityinfo.health <= 0.0f && !AreOtherPlayersDead())
		{
			P_rank = RANK::LOSER;
			this->active = false;
			this->Entityinfo.entitycoll->rect.x = 0;
			this->Entityinfo.entitycoll->rect.y = 0;
			this->Entityinfo.HitBox->SetPos(this->Entityinfo.entitycoll->rect.x, this->Entityinfo.entitycoll->rect.y);
			App->fade->FadeCustom(colB.r, colB.g, colB.b, alphaB, 2.0f, start_timeD, total_timeD, current_stepD,colB);

			App->audio->PlayFx(this->playerinfo.basic_fx);
		}

	}

}
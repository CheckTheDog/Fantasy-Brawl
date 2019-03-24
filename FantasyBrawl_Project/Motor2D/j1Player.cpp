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


j1Player::j1Player(entity_info entityinfo) : j1Entity(entity_type::PLAYER, entityinfo)
{
}

j1Player::~j1Player()
{

}

bool j1Player::Start()
{
	LOG("--- Loading player ---");

	// --- Get every data loaded by Entity Manager ---
	playerinfo = manager->GetPlayerData();

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
	//if (spritesheet == nullptr)
	//	spritesheet = App->tex->Load(playerinfo.Texture.GetString());

	//-- active ----
	active = true;

	return true;
}

void j1Player::UpdateEntityMovement(float dt)
{

	/*switch (EntityMovement)
	{
	case MOVEMENT::RIGHTWARDS:
		Accumulative_pos_Right += Velocity.x*dt;

		if (Accumulative_pos_Right > 1.1)
		{
			Future_position.x += Accumulative_pos_Right;
			Accumulative_pos_Right -= Accumulative_pos_Right;
		}
		break;
	case MOVEMENT::LEFTWARDS:
		Accumulative_pos_Left += Velocity.x*dt;

		if (on_air)
		{
			if (Accumulative_pos_Left > 1.0)
			{
				Future_position.x -= Accumulative_pos_Left;
				Future_position.x -= Accumulative_pos_Left;

				Accumulative_pos_Left -= Accumulative_pos_Left;
			}
		}
		else
		{
			if (Accumulative_pos_Left > 1.5)
			{
				Future_position.x -= Accumulative_pos_Left;
				Accumulative_pos_Left -= Accumulative_pos_Left;
			}
		}
		break;
	case MOVEMENT::UPWARDS:

		Accumulative_pos_Up += Velocity.y*dt;

		if (Accumulative_pos_Up > 0.75)
		{
			Future_position.y -= Accumulative_pos_Up;
			Accumulative_pos_Up -= Accumulative_pos_Up;
		}
		break;
	case MOVEMENT::FREEFALL:

		Accumulative_pos_Down += playerinfo.gravity* dt;

		if (on_air)
			Accumulative_pos_Down += playerinfo.gravity * dt;

		if (Accumulative_pos_Down > 1.0)
		{
			Velocity.y -= Accumulative_pos_Down;
			Future_position.y += Accumulative_pos_Down;
			Accumulative_pos_Down -= Accumulative_pos_Down;
		}

		break;
	}

	entitycoll->SetPos(Future_position.x, Future_position.y);

	App->coll->QueryCollisions(*entitycoll);*/

	MOVEMENT EntityMovement = MOVEMENT::STATIC;
}


void j1Player::Handle_Ground_Animations()
{
	// --- Handling Ground Animations ---

	//--- TO RUN ---

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (CurrentAnimation == playerinfo.runRight)
			CurrentAnimation = playerinfo.idleRight;
		else if (CurrentAnimation == playerinfo.runLeft)
			CurrentAnimation = playerinfo.idleLeft;
	}
	else
	{

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
			CurrentAnimation != playerinfo.jumpingRight      &&
			CurrentAnimation != playerinfo.fallingRight)
		{
			CurrentAnimation = playerinfo.runRight;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
			CurrentAnimation != playerinfo.jumpingLeft       &&
			CurrentAnimation != playerinfo.fallingLeft)
		{
			CurrentAnimation = playerinfo.runLeft;
		}
	}

	//--- TO IDLE ---

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && CurrentAnimation == playerinfo.runRight || CurrentAnimation == playerinfo.jumpingRight)
		CurrentAnimation = playerinfo.idleRight;

	if (CurrentAnimation == playerinfo.fallingRight)
		CurrentAnimation = playerinfo.idleRight;

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP && CurrentAnimation == playerinfo.runLeft || CurrentAnimation == playerinfo.jumpingLeft)
		CurrentAnimation = playerinfo.idleLeft;

	if (CurrentAnimation == playerinfo.fallingLeft)
		CurrentAnimation = playerinfo.idleLeft;

	//--------------    ---------------

}


void j1Player::Handle_Aerial_Animations()
{
	// --- Handling Aerial Animations ---


	////--- TO JUMP ---

	//if (Velocity.y > playerinfo.jump_force / 2)
	//{
	//	if (CurrentAnimation == playerinfo.runRight || CurrentAnimation == playerinfo.idleRight)
	//		CurrentAnimation = playerinfo.jumpingRight;

	//	else if (CurrentAnimation == playerinfo.runLeft || CurrentAnimation == playerinfo.idleLeft)
	//		CurrentAnimation = playerinfo.jumpingLeft;
	//}

	////--- TO FALL ---

	//else if (Velocity.y < playerinfo.jump_force / 2)
	//{
	//	if (CurrentAnimation == playerinfo.jumpingRight || CurrentAnimation == playerinfo.runRight || CurrentAnimation == playerinfo.idleRight)
	//		CurrentAnimation = playerinfo.fallingRight;
	//	else if (CurrentAnimation == playerinfo.jumpingLeft || CurrentAnimation == playerinfo.runLeft || CurrentAnimation == playerinfo.idleLeft)
	//		CurrentAnimation = playerinfo.fallingLeft;
	//}


	//------------ --------------
}

bool j1Player::Update(float dt)
{
	// --- LOGIC --------------------

		//// --- RIGHT --
		//if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT &&
		//	App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		//{
		//	EntityMovement = MOVEMENT::RIGHTWARDS;
		//}

		//if (EntityMovement != MOVEMENT::STATIC)
		//	UpdateEntityMovement(dt);

		//// --- LEFT ---
		//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT &&
		//	App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		//{
		//	EntityMovement = MOVEMENT::LEFTWARDS;
		//}

		//if (EntityMovement != MOVEMENT::STATIC)
		//	UpdateEntityMovement(dt);

		//// --- IMPULSE ---
		//if (!on_air && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		//{
		//	App->audio->PlayFx(App->audio->jumpfx);
		//	Apply_Vertical_Impulse(dt);
		//	on_air = true;
		//}

		//else if (on_air && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && double_jump == false)
		//{
		//	App->audio->PlayFx(App->audio->doublejumpfx);
		//	double_jump = true;
		//	Velocity.y = 0.0f;
		//	Apply_Vertical_Impulse(dt);
		//}

		//// --- UP ---
		//if (on_air && Velocity.y > 0.0f)
		//{
		//	EntityMovement = MOVEMENT::UPWARDS;
		//}

		//if (EntityMovement != MOVEMENT::STATIC)
		//	UpdateEntityMovement(dt);


		//// --- FREE FALL --- 


		//for (unsigned short i = 0; i < 4; ++i)
		//{
		//	EntityMovement = MOVEMENT::FREEFALL;
		//	UpdateEntityMovement(dt);

		//	if (coll_up)
		//		break;

		//}

	//-------------------------------

	// --- Handling animations ---

	//if (Velocity.y == 0.0f)
	//	Handle_Ground_Animations();
	//else
	//	Handle_Aerial_Animations();


	return true;
}

bool j1Player::PostUpdate(float dt)
{
	bool ret = true;

	// ---------------------- //

	// --- Blitting player ---


	//if (CurrentAnimation == playerinfo.runRight || CurrentAnimation == playerinfo.idleRight || CurrentAnimation == playerinfo.fallingRight || CurrentAnimation == playerinfo.jumpingRight)
	//	App->render->Blit(spritesheet, Future_position.x - 3, Future_position.y, &CurrentAnimation->GetCurrentFrame(dt));

	//else if (CurrentAnimation == playerinfo.runLeft || CurrentAnimation == playerinfo.idleLeft || CurrentAnimation == playerinfo.fallingLeft || CurrentAnimation == playerinfo.jumpingLeft)
	//	App->render->Blit(spritesheet, Future_position.x - 6, Future_position.y, &CurrentAnimation->GetCurrentFrame(dt));

	//else
	//	App->render->Blit(spritesheet, Future_position.x - 3, Future_position.y, &CurrentAnimation->GetCurrentFrame(dt));


	// ---------------------- //

	return ret;
}

void j1Player::OnCollision(Collider * entitycollider, Collider * to_check)
{
	if (!god_mode)
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
		case MOVEMENT::FREEFALL:
			Down_Collision(entitycollider, to_check);
			break;
		}

		Future_position.x = entitycollider->rect.x;
		Future_position.y = entitycollider->rect.y;
	}
}

void j1Player::Right_Collision(Collider * entitycollider, const Collider * to_check)
{
	SDL_IntersectRect(&entitycollider->rect, &to_check->rect, &Intersection);

	switch (to_check->type)
	{
	case COLLIDER_TYPE::COLLIDER_FLOOR:
		entitycollider->rect.x -= Intersection.w;
		App->render->camera.x = camera_pos_backup.x;
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
		App->render->camera.x = camera_pos_backup.x;
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

	/*Velocity.y = 0.0f;*/
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
	/*Velocity.y = 0.0f;*/
	on_air = false;
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

	camera_pos_backup.x = App->render->camera.x;
	camera_pos_backup.y = App->render->camera.y;

	EntityMovement = MOVEMENT::STATIC;

	Update(dt);
}


#include "j1Player.h"
#include "p2Log.h"

j1Player::j1Player()
{
}

j1Player::~j1Player()
{
}

bool j1Player::Awake()
{
	logic_updates_per_second = 10.0;
	update_s_cycle = 1.0f / (float)logic_updates_per_second;

	return true;
}

bool j1Player::Start()
{

	logic_updates_per_second = 10.0;
	update_s_cycle = 1.0f / (float)logic_updates_per_second;

	return true;
}

bool j1Player::PreUpdate()
{

	do_logic = false;


	return true;
}

bool j1Player::UpdateTick(float dt)
{
	frame_count++;

	return true;
}

bool j1Player::Update(float dt)
{
	accumulated_time += dt;

	if (accumulated_time >= update_s_cycle)
	{
		do_logic = true;
	}

	if (do_logic == true)
	{
		UpdateTick(dt);
		//LOG("Did logic step after %f", accumulated_time);
		//LOG("frame_Count: %i", frame_count);
		accumulated_time = 0.0f;
	}

	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

bool j1Player::CleanUp()
{
	return true;
}

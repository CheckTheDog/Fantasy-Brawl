#include "j1ArenaInteractions.h"
#include "j1Render.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Viewport.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include <string>

j1ArenaInteractions::j1ArenaInteractions() : j1Module()
{
	name.assign("arena_interactions");
}

j1ArenaInteractions::~j1ArenaInteractions()
{
}

bool j1ArenaInteractions::CleanUp()
{
	for (std::list<stormPhase*>::iterator item = storm_phases.begin();
		item != storm_phases.end(); item++)
	{
		RELEASE((*item));
	}
	storm_phases.clear();

	for (int i = 0; i < 4; ++i)
	{
		if (storm_colliders[i] != nullptr)
		{
			storm_colliders[i]->to_delete = true;
			storm_colliders[i] = nullptr;
		}
	}

	return true;
}

bool j1ArenaInteractions::Awake(pugi::xml_node & config)
{
	// RGB values of the storm color, Alpha on "Blink", Speed, Time between blinks,
	r = config.child("r").attribute("value").as_uint();
	g = config.child("g").attribute("value").as_uint();
	b = config.child("b").attribute("value").as_uint();
	a = config.child("maximum_opacity").attribute("value").as_float();

	storm_speed = config.child("storm_speed").attribute("value").as_int();
	s_between_blinks = config.child("seconds_between_blinks").attribute("value").as_float();
	
	// Read and add the phases of the storm
	for (pugi::xml_node phase = config.child("storm_phase"); phase != nullptr;
		phase = phase.next_sibling("storm_phase"))
	{
		Uint16 waiting_time = phase.attribute("w_time").as_uint();
		Uint8 tiles_advanced = phase.attribute("tiles_to_advance").as_uint();
		uint damage_per_tick = phase.attribute("damage_per_tick").as_uint();

		stormPhase* ph = new stormPhase(waiting_time, tiles_advanced,damage_per_tick);
		storm_phases.push_back(ph);
	}

	active = false;

	return true;
}

bool j1ArenaInteractions::Start()
{

	return true;
}

bool j1ArenaInteractions::Update(float dt)
{
	
	std::list<stormPhase*>::iterator phase_iterator = storm_phases.begin();

	for (int i = 0; i < current_phase; i++)
	{
		phase_iterator++;
	}

	if (phase_iterator != storm_phases.end())
	{
		if (storm_moving == true)
			UpdateStorm(dt);

		// If the storm is STOPPED WAITING && it needs to move
		if (storm_moving == false && storm_timer.ReadSec() >= (*phase_iterator)->waiting_time)
		{
			storm_moving = true;
			storm_timer.Start();
			//Calculate how much time it will take us to reach the next stop
			target_time = (uint)GetMovingTargetTime((*phase_iterator)->tiles_to_advance);
			alarm_hasplayed = false;
			App->audio->PlayFx(App->audio->fxStormClose);
		}
		// If the storm is MOVING && reached destination -> stop
		else if (storm_moving == true && 
			px_moved >= (*phase_iterator)->tiles_to_advance * App->map->data.tile_width)
		{
			storm_moving = false;
			px_moved = 0;
			storm_timer.Start();
			current_phase++;
			target_time = (*phase_iterator)->waiting_time;
		}
		
		if (storm_moving == false)
		{
			float test = storm_timer.ReadSec();

			time_for_timer = std::to_string (UI_storm_countdown);
			
			if (UI_storm_countdown >= 10)
			{
				time_for_timer.erase(2,7);
			}
			else
			{
				time_for_timer.erase(3, 7);
			}
			
			timeFtimer = time_for_timer.data();
			UI_storm_countdown = (*phase_iterator)->waiting_time - test;
			
			if ( alarm_hasplayed == false && UI_storm_countdown < 6)
			{
				App->audio->PlayFx(App->audio->fxStormCloseCount);
				alarm_hasplayed = true;
			}
		}
	}

	return true;
}

bool j1ArenaInteractions::PostUpdate(float dt)
{
	DrawStorm();
	return true;
}

int j1ArenaInteractions::GetStormDamage(int ID)
{
	//If we don't need to apply any debuff (most cases)
	if (damage_entity[ID] == false)
	{
		return 0;
	}
	else // In case we need to apply a debuff
	{
		std::list<stormPhase*>::const_iterator phase = storm_phases.cbegin();
		
		for (int i = 0; i < current_phase; i++)
		{
			if(current_phase < storm_phases.size())
			phase++;
		}

		//We received damage so until next tick we should not receive any debuff/damage
		damage_entity[ID] = false;
		return  (*phase)->damage_per_tick;
	}
}

void j1ArenaInteractions::StartStorm()
{
	//Activate the module!
	active = true;
	//Set position, width and height of the safe area (AKA eye of the storm)
	safe_area.x = safe_area.y = 0;
	map_size.x = safe_area.h = App->map->data.height * App->map->data.tile_height;
	map_size.y = safe_area.w = App->map->data.width * App->map->data.tile_width;

	// Set the values of width and height that will remain constant for certain storm_areas
	storm_areas[(int)STORM_AREA::ABOVE].w = map_size.x;
	storm_areas[(int)STORM_AREA::BELOW].w = map_size.x;

	//Create the colliders for the storm areas
	for (int i = 0; i < 4; ++i)
		storm_colliders[i] = App->coll->AddCollider({ 0,0,0,0 }, COLLIDER_TYPE::COLLIDER_STORM);
	
	//In case the storm was stopped
	/*ticks_timer.Continue();
	storm_timer.Continue();*/

	//Start the timer
	storm_timer.Start();

	current_phase = 0;

	//Start the ticks
	StartStormTick(s_between_blinks);
}

void j1ArenaInteractions::DestroyStorm()
{
	//Deactivate module Update and PostUpdate
	active = false;

	//Delete colliders
	for (int i = 0; i < 4; ++i)
	{
		if (storm_colliders[i] != nullptr)
		{
			storm_colliders[i]->to_delete = true;
			storm_colliders[i] = nullptr;
		}

		storm_areas[i] = { 0,0,0,0 };
	}

	//In case the storm was stopped
	/*ticks_timer.Stop();
	storm_timer.Stop();*/

	current_phase = 0;
}

void j1ArenaInteractions::PauseStorm()
{
	storm_timer.Stop();
	ticks_timer.Stop();
}

void j1ArenaInteractions::ContinueStorm()
{
	storm_timer.Continue();
	ticks_timer.Continue();
}

void j1ArenaInteractions::UpdateStorm(float dt)
{
	if(safe_area.w > 0 && safe_area.h > 0)
	{
		//Calculate normalized movement
		int movement = 0;
		accumulated_movement += storm_speed * dt;
		if (accumulated_movement > 1)
		{
			movement = accumulated_movement;
			accumulated_movement -= 1;

			px_moved += movement;
			
			while (accumulated_movement > 1)
				accumulated_movement -= 1;
		}

		//Update the safe area position
		safe_area.x += movement;
		safe_area.y += movement;
		//Update safe area dimensions
		safe_area.w -= movement * 2;
		safe_area.h -= movement * 2;

		//Update the storm area to fit everything outside the safe area
		storm_areas[(int)STORM_AREA::ABOVE].h = safe_area.y;

		storm_areas[(int)STORM_AREA::BELOW].y = safe_area.y + safe_area.h;
		storm_areas[(int)STORM_AREA::BELOW].h = map_size.y - (safe_area.y + safe_area.h);

		storm_areas[(int)STORM_AREA::LEFT].y = safe_area.y;
		storm_areas[(int)STORM_AREA::LEFT].w = safe_area.x;
		storm_areas[(int)STORM_AREA::LEFT].h = safe_area.h;

		storm_areas[(int)STORM_AREA::RIGHT].x = safe_area.y;
		storm_areas[(int)STORM_AREA::RIGHT].y = safe_area.y;
		storm_areas[(int)STORM_AREA::RIGHT].w = safe_area.x;
		storm_areas[(int)STORM_AREA::RIGHT].h = safe_area.h;

		storm_areas[(int)STORM_AREA::RIGHT] = {safe_area.x + safe_area.w, safe_area.y,
												map_size.x - (safe_area.x + safe_area.w), safe_area.h};

		UpdateStormColliders();
	}
}

void j1ArenaInteractions::DrawStorm()
{
	///Iterate through the storm_areas and print them
	//Time calculations for the blend
	float now = ticks_timer.ReadSec() - start_time;
	float normalized = MIN(0.5f, now / (float)total_time);

	normalized = 1.0f - normalized;

	//Draw the 4 quads
	for (int i = 0; i < 4; ++i)
	{
		App->view->LayerDrawQuad(storm_areas[i], r, g, b, (uint)(normalized * a), true);
	}

	//If the blend finished restart it
	if (now >= total_time)
		StartStormTick(s_between_blinks);
}

void j1ArenaInteractions::StartStormTick(float time)
{
	//Initialize necessary variables to do the blending
	ticks_timer.Start();
	start_time = ticks_timer.ReadSec();
	total_time = time;

	//Set all booleans to hurt players to true
	for (int i = 0; i < ENTITIES_TO_HURT; ++i)
		damage_entity[i] = true;

}

float j1ArenaInteractions::GetMovingTargetTime(int tiles_to_move)
{
	// Calculate the seconds it will take to move from one storm phase to the other
	return ( (tiles_to_move * App->map->data.tile_width) / storm_speed);
}

void j1ArenaInteractions::UpdateStormColliders()
{
	for (int i = 0; i < 4; ++i)
	{
		if(storm_colliders[i] != nullptr)
		storm_colliders[i]->rect = storm_areas[i];
	}
}

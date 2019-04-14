#include "ArenaInteractions.h"
#include "j1Render.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "j1Map.h"
#include "j1App.h"

ArenaInteractions::ArenaInteractions() : j1Module()
{
	name.assign("arena_interactions");
}

ArenaInteractions::~ArenaInteractions()
{
}

bool ArenaInteractions::Awake(pugi::xml_node & config)
{
	// RGB values of the storm color, Alpha on "Blink", Speed, Time between blinks,
	r = config.child("r").attribute("value").as_uint();
	g = config.child("g").attribute("value").as_uint();
	b = config.child("b").attribute("value").as_uint();
	a = config.child("maximum_opacity").attribute("value").as_float();

	storm_speed = config.child("storm_speed").attribute("value").as_int();
	s_between_blinks = config.child("seconds_between_blinks").attribute("value").as_uint();
	
	// Read and add the phases of the storm
	for (pugi::xml_node phase = config.child("storm_phase"); phase != nullptr;
		phase = phase.next_sibling("storm_phase"))
	{
		Uint16 waiting_time = phase.attribute("w_time").as_uint();
		Uint8 tiles_advanced = phase.attribute("move_time").as_uint();

		stormPhase* ph = new stormPhase(waiting_time, tiles_advanced);
		storm_phases.push_back(ph);
	}


	return true;
}

bool ArenaInteractions::Start()
{
	StartStorm();
	storm_timer.Start();

	return true;
}

bool ArenaInteractions::Update(float dt)
{
	if(storm_moving == true)
	UpdateStorm(dt);

	std::list<stormPhase*>::iterator phase_iterator = storm_phases.begin();

	for (int i = 0; i < current_phase; i++)
	{
		phase_iterator++;
	}

	if (phase_iterator != storm_phases.end())
	{
		// If the storm is STOPPED WAITING && it needs to move
		if (storm_moving == false && storm_timer.ReadSec() >= (*phase_iterator)->waiting_time)
		{
			storm_moving = true;
			storm_timer.Start();
		}
		// If the storm is MOVING && reached destination -> stop
		else if (storm_moving == true && storm_timer.ReadSec() >= (*phase_iterator)->tiles_advanced)
		{
			storm_moving = false;
			storm_timer.Start();
			current_phase++;
			target_time = (*phase_iterator)->waiting_time;
		}

		if (storm_moving == false)
		{
			float test = storm_timer.ReadSec();
			UI_storm_countdown = (*phase_iterator)->waiting_time - test;
		}
	}

	return true;
}

bool ArenaInteractions::PostUpdate(float dt)
{
	DrawStorm();
	return true;
}

void ArenaInteractions::StartStorm()
{
	//Set position, width and height of the safe area (AKA eye of the storm)
	safe_area.x = safe_area.y = 0;
	map_size.x = safe_area.h = App->map->data.height * App->map->data.tile_height;
	map_size.y = safe_area.w = App->map->data.width * App->map->data.tile_width;

	// Set the values of width and height that will remain constant for certain storm_areas
	storm_areas[(int)STORM_AREA::ABOVE].w = map_size.x;
	storm_areas[(int)STORM_AREA::BELOW].w = map_size.x;

	//Start the timers
	storm_timer.Start();
	storm_update_ptimer.Start();

	//Start the visual blend
	BlendStormStart(s_between_blinks);
}

void ArenaInteractions::UpdateStorm(float dt)
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

		storm_update_ptimer.Start();
	}
}

void ArenaInteractions::DrawStorm()
{
	//Iterate through the storm_areas and print them
	
	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(0.5f, (float)now / (float)total_time);

	normalized = 1.0f - normalized;


	for (int i = 0; i < 4; ++i)
	{
		App->render->DrawQuad(storm_areas[i], r, g, b, (Uint8)(normalized * a));
	}

	if (now >= total_time)
		BlendStormStart(s_between_blinks);
}

void ArenaInteractions::BlendStormStart(float time)
{

	start_time = SDL_GetTicks();
	total_time = (Uint32)(time * 0.5f * 1000.0f);
}

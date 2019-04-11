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

bool ArenaInteractions::Start()
{
	StartStorm();

	storm_speed = 5;

	return true;
}

bool ArenaInteractions::Update(float dt)
{

	UpdateStorm();
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
	BlendStormStart(2.000);
}

void ArenaInteractions::UpdateStorm()
{
	if(safe_area.w > 0 && safe_area.h > 0)
	{
		if (storm_update_ptimer.ReadMs() >= 100)
		{
			//Update the safe area position
			safe_area.x += storm_speed;
			safe_area.y += storm_speed;
			//Update safe area dimensions
			safe_area.w -= storm_speed * 2;
			safe_area.h -= storm_speed * 2;

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
}

void ArenaInteractions::DrawStorm()
{
	//Iterate through the storm_areas and print them
	
	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(0.7f, (float)now / (float)total_time);

	normalized = 1.0f - normalized;


	for (int i = 0; i < 4; ++i)
	{
		App->render->DrawQuad(storm_areas[i], 75, 0, 130, (Uint8)(normalized * a));
	}

	if (now >= total_time)
		BlendStormStart(3.000);
}

void ArenaInteractions::BlendStormStart(float time)
{

	start_time = SDL_GetTicks();
	total_time = (Uint32)(time * 0.5f * 1000.0f);
}

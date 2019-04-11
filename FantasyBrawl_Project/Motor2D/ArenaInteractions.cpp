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
	App->render->DrawQuad(safe_area, 75,0,130,80);

	UpdateStorm();
	return true;
}

void ArenaInteractions::StartStorm()
{
	//Setet position, width and height of the safe area (AKA eye of the storm)
	safe_area.x = safe_area.y = 0;
	safe_area.h = App->map->data.height * App->map->data.tile_height;
	safe_area.w = App->map->data.width * App->map->data.tile_width;

	//Start the timers
	storm_timer.Start();
	storm_update_ptimer.Start();
}

void ArenaInteractions::UpdateStorm()
{
	if(safe_area.w > 0 && safe_area.h > 0)
	{
		if (storm_update_ptimer.ReadMs() >= 100)
		{
		safe_area.x += storm_speed;
		safe_area.y += storm_speed;

		safe_area.w -= storm_speed * 2;
		safe_area.h -= storm_speed * 2;

		storm_update_ptimer.Start();
		}
	}
}

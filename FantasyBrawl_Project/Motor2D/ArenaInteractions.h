#ifndef __ARENAINTERACTIONS_H__
#define __ARENAINTERACTIONS_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2Point.h"
#include <list>

class j1Timer;
class j1PerfTimer;

struct stormPhase
{
	stormPhase(Uint16 waiting_time, Uint8 tiles_advanced) :
		waiting_time(waiting_time),
		tiles_advanced(tiles_advanced)
	{}

	Uint16 waiting_time = 0;
	Uint8 tiles_advanced = 0;
};

enum class STORM_AREA
{
	ABOVE = 0,
    BELOW,
	LEFT,
	RIGHT
};

class ArenaInteractions : public j1Module
{
public: /// Functions

	//Constructor and Destructor
	ArenaInteractions();
	virtual ~ArenaInteractions();

	bool Awake(pugi::xml_node& config);
	
	bool Start();

	//Called each loop iteration
	bool Update(float dt);
	bool PostUpdate(float dt);


private: /// Functions

	//This function initializes the Storm
	void StartStorm();

	//For now this will update the size and position of the storm
	void UpdateStorm();

	//This function does the alpha blending and prints the areas of Storm with the correct alpha
	void DrawStorm();

	void BlendStormStart(float time);

public: /// Variables

	// Amount of pixels the storm will move each 100 ms
	int storm_speed;

	float a = 0.0f;
	Uint8 r, g, b = 0;

	bool storm_moving = false;

private: /// Variables
	
	SDL_Rect safe_area = {0,0,0,0};

	// The 4 visual sides of the storm, ABOVE, BELOW, LEFT RIGHT
	SDL_Rect storm_areas[4] = { {0,0,0,0} };

	//Storm timers ------
	j1Timer storm_timer;
	j1PerfTimer storm_update_ptimer;
	//------
	
	//We will use an iPoint, x for width and y for height
	iPoint map_size;

	//Start and total time for storm areas color blending purposes
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	uint s_between_blinks;

	//List for the storm phases & current phase
	std::list<stormPhase*> storm_phases;
	Uint8 current_phase = 0;
};
#endif
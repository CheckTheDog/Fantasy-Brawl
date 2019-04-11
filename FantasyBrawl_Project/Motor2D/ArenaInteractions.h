#ifndef __ARENAINTERACTIONS_H__
#define __ARENAINTERACTIONS_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2Point.h"

class j1Timer;
class j1PerfTimer;


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
	
	bool Start();

	//Called each loop iteration
	bool Update(float dt);


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

	float a = 200.0f;

private: /// Variables
	SDL_Rect safe_area = {0,0,0,0};
	j1Timer storm_timer;
	j1PerfTimer storm_update_ptimer;

	// The 4 visual sides of the storm, ABOVE, BELOW, LEFT RIGHT
	SDL_Rect storm_areas[4] = { {0,0,0,0} };

	//We will use an iPoint, x for width and y for height
	iPoint map_size;

	//Start and total time for blending purposes
	Uint32 start_time = 0;
	Uint32 total_time = 0;
};




#endif
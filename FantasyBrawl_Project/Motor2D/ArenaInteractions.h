#ifndef __ARENAINTERACTIONS_H__
#define __ARENAINTERACTIONS_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "p2Point.h"
#include <list>

#define ENTITIES_TO_HURT 4

class j1Timer;
class j1PerfTimer;

struct stormPhase
{
	stormPhase(Uint16 waiting_time, Uint8 tiles_to_advance, uint damage_per_tick) :
		waiting_time(waiting_time),
		tiles_to_advance(tiles_to_advance),
		damage_per_tick(damage_per_tick)
	{}

	Uint16 waiting_time = 0;
	Uint8 tiles_to_advance = 0;
	uint damage_per_tick = 0;
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

	bool CleanUp();

	bool Awake(pugi::xml_node& config);
	
	bool Start();

	//Called each loop iteration
	bool Update(float dt);
	bool PostUpdate(float dt);

	//Returns the damage received at the moment of the call
	int GetStormDebuff(int ID);


private: /// Functions

	//This function initializes the Storm
	void StartStorm();

	//For now this will update the size and position of the storm
	void UpdateStorm(float dt);

	//This function does the alpha blending and prints the areas of Storm with the correct alpha
	void DrawStorm();

	void BlendStormStart(float time);

	float GetMovingTargetTime(int tiles_to_move);

public: /// Variables

	// Amount of pixels the storm will move each 100 ms
	int storm_speed;

	float a = 0.0f;
	Uint8 r, g, b = 0;

	//Bool determining if the storm is currently moving or not
	bool storm_moving = false;

	//Public stuff for timers that can be used by other modules.
	uint target_time = 0;
	float UI_storm_countdown = 0;

private: /// Variables
	
	//Storm timers ------
	j1Timer storm_timer;
	j1PerfTimer storm_update_ptimer;
	//------
	
	//We will use an iPoint, x for width and y for height
	iPoint map_size;

	//Start and total time for storm areas color blending purposes
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	float s_between_blinks;

	/// DAMAGE & STORM related data -----------------------------
	SDL_Rect safe_area = { 0,0,0,0 };

	// The 4 visual sides of the storm, ABOVE, BELOW, LEFT RIGHT
	SDL_Rect storm_areas[4] = { {0,0,0,0} };

	//List for the storm phases & current phase
	std::list<stormPhase*> storm_phases;
	Uint8 current_phase = 0;

	//Accumulated Movement for normalized movement
	float accumulated_movement = 0;
	int px_moved = 0;

	//Entity damage management
	bool damage_entity[ENTITIES_TO_HURT] = {false};
};
#endif
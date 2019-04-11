#ifndef __ARENAINTERACTIONS_H__
#define __ARENAINTERACTIONS_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"

class j1Timer;
class j1PerfTimer;

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

public: /// Variables

	// Amount of pixels the storm will move each 100 ms
	int storm_speed;

private: /// Variables
	SDL_Rect safe_area = {0,0,0,0};
	j1Timer storm_timer;
	j1PerfTimer storm_update_ptimer;

};




#endif
#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player :public j1Module
{
public:

	j1Player();
	~j1Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool UpdateTick(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


public:
	bool				do_logic = false;
	int					logic_updates_per_second = 0;
	float				update_s_cycle = 0;
	float				accumulated_time = 0;
	int frame_count = 0;

private:


};

#endif // __j1Player_H__

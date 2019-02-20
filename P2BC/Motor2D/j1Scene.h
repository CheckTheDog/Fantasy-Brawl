#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include <vector>

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	// Set new Walkability map
	bool SetWalkabilityMap();

	// Change current map
	bool ChangeMap(int destination_map_id);

private:
	SDL_Texture* debug_tex;

	std::vector <std::string*> StageList;
};

#endif // __j1SCENE_H__
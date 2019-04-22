#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include <vector>
#include "j1Player.h"
#include <queue>

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

	void LoadLvl(int num = 0);

	// --- Reset everything for new game ---
	void ResetAll();
	j1Player* GetWinner();

public:

	bool load_lvl = false;
	int newLvl = 0;

	// --- Players ---
	j1Player* player1 = nullptr;
	j1Player* player2 = nullptr;
	j1Player* player3 = nullptr;
	j1Player* player4 = nullptr;

	fPoint initialposP1 = { 0.0f,0.0f };
	fPoint initialposP2 = { 0.0f,0.0f };
	fPoint initialposP3 = { 0.0f,0.0f };
	fPoint initialposP4 = { 0.0f,0.0f };


private:
	SDL_Texture* debug_tex;

	std::vector <std::string*> StageList;
};

#endif // __j1SCENE_H__
#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Collision;
class j1EntityManager;
class j1ArenaInteractions;
class j1ParticleSystem;
class j1Viewport;
class j1Fonts;
class j1Gui;
class j1Transition;
class j1UIScene;
class j1BuffManager;
class j1FadeToBlack;
class j1ItemManager;


class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(std::list<std::string>& list_to_fill) const;
	void RequestBrowser(const char * url) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

	

public:

	// Modules
	j1Window*			win = NULL;
	j1Input*			input = NULL;
	j1Render*			render = NULL;
	j1Textures*			tex = NULL;
	j1Audio*			audio = NULL;
	j1Scene*			scene = NULL;
	j1Map*				map = NULL;
	j1EntityManager*    entities = nullptr;
	j1ArenaInteractions*  arena_interactions = nullptr;
	j1BuffManager*		buff = NULL;
	j1Collision*        coll = nullptr;
	j1ParticleSystem*	particlesys = nullptr;
	j1Viewport*			view = NULL;
	j1FadeToBlack*      fade = NULL;
	j1ItemManager*		item_manager = nullptr;

	int              framerate_cap;

	bool			 debug_mode = false;
	j1Gui*				gui = NULL;
	j1Fonts*			fonts = NULL;
	j1Transition*		transition = NULL;
	j1UIScene*			ui_scene = NULL;

	bool on_GamePause = false;


private:

	std::list <j1Module*> modules;

	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	std::string			load_game;
	mutable std::string	save_game;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					capped_ms = -1;
};

extern j1App* App;

#endif

#ifndef __J1TRANSITION_H__
#define __J1TRANSITION_H__
#include "j1Module.h"
#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1Timer.h"
#include "Animation.h"

enum transition_state
{
	UNACTIVE,
	GIN,
	GOUT
};
class j1Transition : public j1Module
{
public:

	j1Transition();
	~j1Transition();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);

	void menuTransition(menu_id newMenuID, float time = 0.5f);
	void sceneTransition(int newLvl, float time = 0.5f);

	bool doingMenuTransition = false;

private:

	transition_state sceneState = UNACTIVE;
	transition_state menuState = UNACTIVE;
	bool doingSceneTransition = false;
	menu_id newMenuID;
	int newLvl;
	j1Timer timer;
	float total_time = 0.0f;
	float alpha_value = 0;

	//BOOK
	SDL_Texture* book_texture = nullptr;
	Animation book;
	SDL_Texture* intro_tex = nullptr;
	Animation intro_anim;
	j1Timer intro_timer;
	float intro_alpha = 255;
};
#endif // !__J1TRANSITION_H__

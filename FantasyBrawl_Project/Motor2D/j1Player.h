#ifndef __j1Player_H__
#define __j1Player_H__

#include "p2Point.h"
#include "j1Entity.h"

struct SDL_Texture;
struct Collider;
enum class PLAYER;

struct Playerdata {

	Animation* idleRight = nullptr;
	Animation* idleLeft = nullptr;
	Animation* runRight = nullptr;
	Animation* runLeft = nullptr;
	Animation* jumpingRight = nullptr;
	Animation* jumpingLeft = nullptr;
	Animation* fallingRight = nullptr;
	Animation* fallingLeft = nullptr;
	Animation* deathRight = nullptr;
	Animation* deathLeft = nullptr;
	Animation* airRight = nullptr;
	Animation* airLeft = nullptr;

	std::string folder;
	std::string Texture;

	SDL_Rect playerrect = { 0,0,0,0 };
};

enum class MOVEMENT
{
	RIGHTWARDS,
	LEFTWARDS,
	UPWARDS,
	DOWNWARDS,

	STATIC
};


class j1Player :public j1Entity
{
public:

	j1Player(entity_info entityinfo, Playerdata * player_info);
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	// Called each loop iteration
	void FixedUpdate(float dt);

	// Called each logic iteration
	void LogicUpdate(float dt);

	void UpdateEntityMovement(float dt);

	void HandleAnimations();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// --- Collisions Handling ---

	void OnCollision(Collider* c1, Collider* c2);

	void Right_Collision(Collider* entitycollider, const Collider* to_check);

	void Left_Collision(Collider* entitycollider, const Collider* to_check);

	void Up_Collision(Collider* entitycollider, const Collider* to_check);

	void Down_Collision(Collider* entitycollider, const Collider* to_check);

public:

	PLAYER ID;
	Playerdata playerinfo;
	SDL_Rect Intersection = { 0,0,0,0 };

	// --- MOVEMENT VARIABLES ---
	fPoint Future_position = { 0,0 };
	MOVEMENT EntityMovement = MOVEMENT::STATIC;

	// --- Gamepad ---
	float Axisx_value = 0.0f;
	float Axisy_value = 0.0f;

	float Axis_range = 0.0f;
	float multipliermin = 0.1f;

	float multiplier_x = 0.0f;
	float multiplier_y = 0.0f;
};

#endif // __j1Player_H__

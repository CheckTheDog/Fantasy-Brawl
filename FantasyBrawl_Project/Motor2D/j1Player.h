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
	Animation* idleUp = nullptr;
	Animation* idleUpright = nullptr;
	Animation* idleUpleft = nullptr;
	Animation* idleDown = nullptr;
	Animation* idleDownright = nullptr;
	Animation* idleDownleft = nullptr;

	Animation* moveRight = nullptr;
	Animation* moveLeft = nullptr;
	Animation* moveUp = nullptr;
	Animation* moveUpright = nullptr;
	Animation* moveUpleft = nullptr;
	Animation* moveDown = nullptr;
	Animation* moveDownright = nullptr;
	Animation* moveDownleft = nullptr;

	Animation* attackRight = nullptr;
	Animation* attackLeft = nullptr;
	Animation* attackUp = nullptr;
	Animation* attackUpright = nullptr;
	Animation* attackUpleft = nullptr;
	Animation* attackDown = nullptr;
	Animation* attackDownright = nullptr;
	Animation* attackDownleft = nullptr;

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

enum class AXISDIRECTION
{
	AXIS_X,
	AXIS_Y
};

enum class PSTATE
{
	MOVING,
	ATTACKING,
	IDLE
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

	// --- Entity Movement ---
	void MoveX(float dt);
	void MoveY(float dt);
	void HandleInput();

	// --- Entity Animations ---
	void HandleAnimations();
	void HandleAttackAnimations();
	void HandleMovementAnimations();
	void HandleIdleAnimations();

	// --- Save & Load ---
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// --- Collisions Handling ---

	void OnCollision(Collider* c1, Collider* c2);

	void CheckCollision();

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
	Animation* CurrentAnimation;
	MOVEMENT EntityMovement;
	PSTATE PlayerState;
	AXISDIRECTION direction;

	// --- Gamepad ---
	float multipliermin = 0.1f;

	// Left Joystick LJ 
	float LJAxisx_value = 0.0f;
	float LJAxisy_value = 0.0f;
	float LJdirection_x = 0.0f;
	float LJdirection_y = 0.0f;

	// Right Joystick RJ
	float RJAxisx_value = 0.0f;
	float RJAxisy_value = 0.0f;
	float RJdirection_x = 0.0f;
	float RJdirection_y = 0.0f;
	
};

#endif // __j1Player_H__

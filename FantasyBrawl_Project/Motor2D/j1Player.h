#ifndef __j1Player_H__
#define __j1Player_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "j1ParticleSystem.h"
#include "j1Timer.h"

struct SDL_Texture;
struct Collider;
enum class PLAYER;

#define JOYSTICK_DEAD_ZONE 8000

enum class CHARACTER
{
	WENDOLIN,
	MELIADOUL,
	SIMON,
	TRAKT,
	NONE
};

struct Playerdata {

	Animation idleRight;
	Animation idleLeft;
	Animation idleUp;
	Animation idleUpright;
	Animation idleUpleft;
	Animation idleDown;
	Animation idleDownright;
	Animation idleDownleft;

	Animation moveRight;
	Animation moveLeft;
	Animation moveUp;
	Animation moveUpright;
	Animation moveUpleft;
	Animation moveDown;
	Animation moveDownright;
	Animation moveDownleft;

	Animation attackRight;
	Animation attackLeft;
	Animation attackUp;
	Animation attackUpright;
	Animation attackUpleft;
	Animation attackDown;
	Animation attackDownright;
	Animation attackDownleft;

	std::string folder;
	std::string Texture;
	SDL_Texture* tex = nullptr;

	Particle basic_attack;
	uint basic_fx = 0;
	uint super_fx = 0;

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
	IDLE,
	NONE
};

enum class RANK
{
	LOSER,
	CONTENDER
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
	void GetAttackAnimation();
	void GetMovementAnimation();
	void GetIdleAnimation();
	bool InRange(float axisX, float axisY, float range_start, float range_end);

	// --- Save & Load ---
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// --- Character && Player ---
	void AssignCharacter();
	const fPoint GetNearestPlayerDirection();
	void ComputeDistance2players();

	// --- Collisions Handling ---

	void OnCollision(Collider* c1, Collider* c2);

	void CheckCollision();

	void Right_Collision(Collider* entitycollider, const Collider* to_check);

	void Left_Collision(Collider* entitycollider, const Collider* to_check);

	void Up_Collision(Collider* entitycollider, const Collider* to_check);

	void Down_Collision(Collider* entitycollider, const Collider* to_check);

	void CheckParticleCollision(Collider * entitycollider, const Collider* to_check);

	//bool PlayerLayerOrder();

	// --- Entity Attacks ---
	void HandleAttacks();
	void HandleShield();
	void HandleSuperAttacks();

	// --- Character Specific Super Attacks ---
	void Launch1stSuper();
	void Launch2ndSuper();
	void Launch3rdSuper();
	void Launch4thSuper();

public:

	// --- Basic Stuff ---
	PLAYER ID;
	Playerdata playerinfo;
	PSTATE PlayerState;
	CHARACTER character = CHARACTER::NONE;

	// --- MOVEMENT VARIABLES ---
	fPoint Future_position = { 0,0 };
	MOVEMENT EntityMovement;
	AXISDIRECTION direction;

	// --- Gamepad Input ---
	float multipliermin = 0.1f;
	bool RJinverted = false;

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
	
	bool PlayerPrintOnTop = false;

	// --- Collisions ---
	SDL_Rect Intersection = { 0,0,0,0 };

	// --- Timers ---
	j1Timer superTimer;
	j1Timer shieldTimer;
	j1Timer shieldDuration;
	j1Timer basicTimer;
	j1Timer attackanimTimer;
	j1Timer RJinversion;
	bool shieldON = false;

	// --- Score ---
	uint kills = 0;
	RANK P_rank = RANK::CONTENDER;

	// --- Auto aim ---
	float Aim_Radius = 300.0f;

	// --- Particles ---
	Particle * last_particle = nullptr;

	// --- Distances to players ---
	float absoluteDistanceP1 = 0.0f;
	float absoluteDistanceP2 = 0.0f;
	float absoluteDistanceP3 = 0.0f;
	float absoluteDistanceP4 = 0.0f;

	fPoint directionP1 = { 0.0f,0.0f };
	fPoint directionP2 = { 0.0f,0.0f };
	fPoint directionP3 = { 0.0f,0.0f };
	fPoint directionP4 = { 0.0f,0.0f };

	// --- IDCircle ---
	Animation * CurrentIDCircleAnimation = nullptr;

	// --- Shield ---
	Animation * CurrentShieldAnimation = nullptr;

};

#endif // __j1Player_H__

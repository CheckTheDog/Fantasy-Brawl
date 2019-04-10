#ifndef _j1PROJECTILES_H_
#define _j1PROJECTILES_H

#include "j1Module.h"
#include <list>
#include "p2Point.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
struct Collider;
struct SDL_Texture;
class ParticleEmitter;
class j1Player;


enum PROJECTILE_TYPE {
		NONE,
		BASIC_WN,
		SPECIAL_WN
};

struct Projectile {
	Projectile(Animation* animation, iPoint position, iPoint speed, Collider* collider, int life, bool flipped, int scale, PROJECTILE_TYPE type, ParticleEmitter* emitter, iPoint emitterOffset) :
		anim(animation), pos(position), speed(speed), coll(collider), life(life), isFlipped(flipped), scale(scale), type(type), emitter(emitter), emitter_offset(emitterOffset) {

		born = SDL_GetTicks();
	}
	void Update();
	void Draw(SDL_Texture* sprites);

	iPoint pos;
	iPoint speed;
	Collider* coll;
	Animation* anim;
	int born;
	int life; 
	bool isFlipped;
	int scale;
	PROJECTILE_TYPE type;
	ParticleEmitter* emitter = nullptr;
	iPoint emitter_offset;
	int gravity = 1;
	float angularVel;
	float angle = 0.0f;
};

class j1Projectiles :
	public j1Module {
public:
	j1Projectiles();
	~j1Projectiles();

	bool Awake(const pugi::xml_node& config);
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();


	int ProjectileType(PROJECTILE_TYPE type, j1Player* player);

	Projectile* AddProjectile(PROJECTILE_TYPE type, iPoint position, iPoint speed, Collider* collider, int life, bool fliped, int scale, ParticleEmitter* emitter = nullptr, iPoint emitterOffset = { 0,0 }, float angularVel = 0);


	SDL_Texture* sprites;
	Animation dagger;



	std::list<Projectile*> projectiles;

};

#endif // !_j1PROJECTILES_H_
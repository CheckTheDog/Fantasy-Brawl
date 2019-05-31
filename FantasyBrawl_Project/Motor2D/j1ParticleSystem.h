#ifndef _j1PARTICLESYSTEM_H_
#define _j1PARTICLESYSTEM_H_

#include "j1Module.h"
#include "Animation.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Collision.h"

#define MAX_PARTICLES 500

struct SDL_Texture;
class j1Player;
struct Effect;


struct Particle
{
	Collider* pCol = nullptr;
	Animation anim;
	fPoint pos = { 0.0f,0.0f };
	fPoint speed = { 0.0f,0.0f };
	fPoint direction = { 0.0f,0.0f };
	uint born = 0;
	uint life = 0;
	uint delay = 0;

	j1Player* originplayer = nullptr;
	Effect* particle_effect = nullptr;
	SDL_Texture* tex = nullptr;
	float angle = 0.0f;
	bool ghost = false;

	fPoint GetPos()const;
	Particle();
	Particle(const Particle& p);
	~Particle();

	bool Update(float dt);
	bool toDelete = false;

};

class j1ParticleSystem :public j1Module
{
public:

	j1ParticleSystem();
	~j1ParticleSystem();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	Particle* AddParticle(Particle& particle, int x, int y, COLLIDER_TYPE type, uint delay, j1Player* porigin = nullptr);

	Particle* GetCollidedParticle(Collider* hitbox, const Collider* particlecollider);

private:

	Particle* active[MAX_PARTICLES];
	float scale = 0.66;

};
#endif // ! _j1PARTICLESYSTEM_H_

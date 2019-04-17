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

struct Particle
{
	Collider* pCol = nullptr;
	Animation anim;
	fPoint pos;
	fPoint speed;
	uint born = 0;
	uint life;
	uint delay = 0;
	j1Player* originplayer;

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

	void AddParticle(Particle& particle, int x, int y, COLLIDER_TYPE type, uint delay, j1Player* porigin = nullptr);

private:

	SDL_Texture * pSprites;

	Particle* active[MAX_PARTICLES];

};
#endif // ! _j1PARTICLESYSTEM_H_

#ifndef  _j1PARTICLE_SYSTEM_H_
#define _j1PARTICLE_SYSTEM_H_

#include "j1Module.h"
#include "j1Textures.h"
#include "Particle.h"
#include "j1Timer.h"
#include "p2Point.h"
#include "p2Log.h"
#include <iostream>

#include "SDL/include/SDL_rect.h"

class ParticleEmitter;

class j1ParticleSystem : public j1Module
{

public:

	j1ParticleSystem();
	~j1ParticleSystem();

	bool Update(float dt);

	Particle* newParticle(ParticleInfo data);
	ParticleEmitter* newEmitter(fPoint pos, const char* configPath);

	void updateParticles();
	void updateEmitters();

public:

	int updateTime;
	int particleCount = 0;
	

private:
	
	j1Timer debugPTimer;

	static const int MAX_PARTICLES = 1000;

	Particle particles[MAX_PARTICLES];

	SDL_Texture* pSprites = nullptr;

	std::list<ParticleEmitter*> pEmitters;

};

#endif // ! _j1PARTICLE_SYSTEM_H_

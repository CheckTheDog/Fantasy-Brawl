#ifndef  _j1PARTICLE_SYSTEM_H_
#define _j1PARTICLE_SYSTEM_H_

#include "j1Module.h"
#include "j1Textures.h"
#include "Particle.h"
#include "p2Point.h"
#include "p2Log.h"
#include <iostream>

#include "SDL/include/SDL_rect.h"

using namespace std;

struct SDL_Texture;
struct SDL_Rect;
class ParticleEmitter;

class j1ParticleSystem : public j1Module
{

public:

	j1ParticleSystem();
	~j1ParticleSystem();

	bool Update(float dt);

	Particle* newParticle(ParticleInfo info);
	ParticleEmitter* newEmitter(fPoint pos, string configPath);

	void updateParticles();
	void updateEmitters();
	

private:
	
	static const int MAX_PARTICLES = 1000;

	Particle particles[MAX_PARTICLES];


};

#endif // ! _j1PARTICLE_SYSTEM_H_

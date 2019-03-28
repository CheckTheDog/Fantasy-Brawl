#ifndef  _j1PARTICLE_SYSTEM_H_
#define _j1PARTICLE_SYSTEM_H_

#include "j1Module.h"
#include "p2Point.h"


#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;

enum Type //TIPOS DE PARTICULAS
{
	NONE,

};

class j1ParticleSystem : public j1Module
{

public:

	j1ParticleSystem();
	~j1ParticleSystem();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();


};

#endif // ! _j1PARTICLE_SYSTEM_H_

#ifndef  _j1PARTICLE_SYSTEM_H_
#define _j1PARTICLE_SYSTEM_H_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "p2Log.h"

#include "SDL/include/SDL_rect.h"

struct SDL_Texture;
struct SDL_Rect;


enum particleType //TIPOS DE PARTICULAS
{
	NONE,

};

struct ParticleData
{

	Particle();
	Particle(Particle& p);
	~Particle();

	Animation anim;
	particleType type;

	bool isActive = false;
	fPoint pos;
	fPoint speed;
	float angle;
	int width;
	int height;

	SDL_Rect frames[MAX_FRAMES];

	void PushBack(const SDL_Rect& rect);
	float current_frame = 0.0f;
	int last_frame = 0;
	

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

	void LoadParticle(const char* filePath, const char* fileName);
	void AddParticle(particleType type, fPoint pos);
	void AddProjectile();

private:
	
	SDL_Texture * graphics = nullptr;
	Particle* isActive[MAX_ACTIVE_PARTICLES];


};

#endif // ! _j1PARTICLE_SYSTEM_H_

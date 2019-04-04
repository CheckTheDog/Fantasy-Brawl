#ifndef _PARTICLE_H_
#define _PARTICLE_H_


#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1Render.h"


struct ParticleInfo
{
	int lifeTime;

	float iScale;
	float fScale;

	fPoint pos;

	float angle;
	float velocity;

	const char* texturePath;
};

class Particle
{

public:

	Particle();
	~Particle();

	void Init(ParticleInfo data);

private:

	void config(ParticleInfo data);

	void loadParticle(const char* path);

private:

	int lifeTime;
	int framesLeft;

	fPoint pos;
	iPoint iSize;
	fPoint velocity;

	float iScale;
	float scale;
	float fScale;

	SDL_Texture* tex = nullptr;

};

#endif // !_PARTICLE_H_

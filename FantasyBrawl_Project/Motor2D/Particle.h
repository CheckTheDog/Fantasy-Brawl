#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1Render.h"
#include <iostream>

using namespace std;

struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 alpha;
};

struct ParticleInfo
{
	int lifeTime;

	float initScale;
	float finalScale;

	Color initColor;
	Color finalColor;

	fPoint pos;

	float angle;
	float velocity;

	SDL_BlendMode blendMode;

	string texPath;

	int drawingOrder;
};

class Particle
{

public:

	Particle();
	~Particle();

	void Init(ParticleInfo data);
	void Animate();
	void Draw();

private:

	void config(ParticleInfo data);

	void loadParticle(const char* path);
	void updateColors();

private:

	int lifeTime;
	int framesLeft;

	fPoint pos;
	iPoint initSize;
	fPoint velocity;

	float initScale;
	float scale;
	float finalScale;

	Color initColor;
	Color currColor;
	Color finalColor;

	SDL_BlendMode blendMode;

	int drawingOrder = 1;

	SDL_Texture* tex = nullptr;

};

#endif // !_PARTICLE_H_

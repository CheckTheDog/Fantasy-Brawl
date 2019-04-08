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
	float speed;

	float initSpin;
	float finalSpin;

	SDL_BlendMode blendMode;

	string texPath;

	int drawingOrder;
	bool isGravityOn;
};

class Particle
{

public:

	Particle();
	~Particle();

	void Init(ParticleInfo data);
	void Animate();
	bool isActive()const;
	void Draw();

private:

	float calculateRatio(float init, float fin)const;
	float degToRad(float deg);
	void config(ParticleInfo data);

	void loadParticle(const char* path);
	void updateColors();

private:

	int lifeTime;
	int framesLeft;

	float lifeTimeRatio = 1.0f;

	fPoint pos;
	iPoint initSize;
	fPoint velocity;

	float initScale = 1.0f;
	float scale = 1.0f;
	float finalScale = 0.0f;

	Color initColor;
	Color currColor;
	Color finalColor;

	SDL_BlendMode blendMode;

	float initSpin;
	float currSpin;
	float finalSpin;

	int drawingOrder = 1;

	SDL_Texture* tex = nullptr;

	bool isGravityOn = true;

};

#endif // !_PARTICLE_H_

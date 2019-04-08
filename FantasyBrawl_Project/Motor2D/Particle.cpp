#include "Particle.h"
#include "j1App.h"
#include "j1Render.h"

Particle::Particle()
{
	framesLeft = 0;
}

Particle::~Particle()
{

}

void Particle::Init(ParticleInfo data)
{
	config(data);

	loadParticle(data.texPath.c_str());

	SDL_assert(tex);
	SDL_QueryTexture(tex, NULL, NULL, &initSize.x, &initSize.y);

	currColor = initColor;
	scale = initScale;
}

void Particle::config(ParticleInfo data)
{
	pos = data.pos;
	lifeTime = data.lifeTime;

	initColor = data.initColor;
	currColor = data.initColor;
	finalColor = data.initColor;

	initScale = data.initScale;
	scale = data.initScale;
	finalScale = data.finalScale;

	float angle = degToRad(data.angle);
	velocity.x = data.speed * cos(angle);
	velocity.y = data.speed * sin(angle);

	initSpin = data.initSpin;
	currSpin = data.initSpin;
	finalSpin = data.finalSpin;

	blendMode = data.blendMode;

	drawingOrder = data.drawingOrder;
	isGravityOn = data.isGravityOn;

}

bool Particle::isActive()const
{
	return framesLeft > 0;
}

void Particle::loadParticle(const char* path)
{
	tex = App->tex->Load(path);
}

void Particle::Animate()
{
	framesLeft--;

	if (isActive() == false)
	{
		App->tex->UnLoad(tex);
	}

	lifeTimeRatio = float(framesLeft) / float(lifeTime);
	scale = calculateRatio(initScale, finalScale);

	currSpin = calculateRatio(initSpin, finalSpin);

	updateColors();

	if (isGravityOn) 
	{
		float gravity = 0.5;

		velocity.y += gravity;
	}

	pos.x += velocity.x;
	pos.y += velocity.y;
}

float Particle::calculateRatio(float init, float fin)const
{
	return (fin - init)*(1 - lifeTimeRatio) + init;
}

float Particle::degToRad(float deg)
{
	return deg * (M_PI / 180);
}

void Particle::updateColors()
{
	currColor.r = calculateRatio(initColor.r, finalColor.r);
	currColor.g = calculateRatio(initColor.g, finalColor.g);
	currColor.b = calculateRatio(initColor.b, finalColor.b);
	currColor.alpha = calculateRatio(initColor.alpha, finalColor.alpha);
}

void Particle::Draw()
{

	iPoint drawingPos;

	drawingPos.x = pos.x - ((initSize.x / 2)*scale);
	drawingPos.y = pos.y - ((initSize.y / 2)*scale);

	SDL_SetTextureColorMod(tex, currColor.r, currColor.g, currColor.b);
	SDL_GetTextureAlphaMod(tex, &currColor.alpha);
	SDL_GetTextureBlendMode(tex, &blendMode);

	App->render->Blit(tex, pos.x, pos.y);
}
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

#include "ParticleEmitter.h"
#include "j1FileSystem.h"
#include "j1Module.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include <iostream>
#include <time.h>


ParticleEmitter::ParticleEmitter(fPoint pos, std::string configPath)
{
	initPos = pos;
	emissionTimer.Start();
	srand(time(NULL));

	loadParticle(configFile, config, configPath);

	freq = config.child("frequency").attribute("value").as_float(10);
	period = 1000 * (1 / freq);

	maxEmissions = config.child("maxEmissions").attribute("value").as_int(1000);
	currEmissions = 0;

	type = (emitterType)config.child("type").attribute("value").as_int(0);

	if (type != NO_ATTACK)
	{
		for (int i = 0; i < maxEmissions; i++)
		newParticle();
	
		isActive = false;
	}

}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(float dt)
{
	if (emissionTimer.Read() >= period)
	{
		newParticle();
		currEmissions++;
	}

	if (currEmissions >= maxEmissions)
		isActive = false;
}

bool ParticleEmitter::loadParticle(pugi::xml_document& pFile, pugi::xml_node& pNode, std::string pPath)
{
	char* buffer;
	int size = App->filesys->Load(pPath.c_str(), &buffer);
	pugi::xml_parse_result res = pFile.load_buffer(buffer, size);
	if (size != 0)
		RELEASE(buffer);

	if (res == NULL) {
		LOG("Application : Could not load xml - %s", res.description());
		return false;
	}
	else {
		pNode = pFile.child("config");
		return true;
	}

}

void ParticleEmitter::particleConfig(ParticleInfo& data)
{
	float angle = config.child("angle").attribute("value").as_float();
	float angleDiff = config.child("angle").attribute("variation").as_float();
	data.angle = operationRand(angle, angleDiff);

	float speed = config.child("speed").attribute("value").as_float();
	float speedDiff = config.child("speed").attribute("variation").as_float();
	data.speed = operationRand(speed, speedDiff);

	fPoint pos;
	pos.x = config.child("position").attribute("x").as_float();
	pos.y = config.child("position").attribute("y").as_float();
	data.pos.x = operationRand(initPos.x, pos.x);
	data.pos.y = operationRand(initPos.y, pos.y);

	float lifeTime = config.child("lifetime").attribute("vlaue").as_float();
	float lifeTimeDiff = config.child("lifetime").attribute("variation").as_float();
	data.lifeTime = operationRand(lifeTime, lifeTimeDiff);
	
	float initScale = config.child("initialScale").attribute("value").as_float();
	float iScaleDiff = config.child("initialScale").attribute("variation").as_float();
	data.speed = operationRand(initScale, iScaleDiff);

	float finalScale = config.child("finalScale").attribute("value").as_float();
	float fScaleDiff = config.child("finalScale").attribute("variation").as_float();
	data.speed = operationRand(finalScale, fScaleDiff);

	data.initColor.r = config.child("initialColor").attribute("r").as_int();
	data.initColor.g = config.child("initialColor").attribute("g").as_int();
	data.initColor.b = config.child("initialColor").attribute("b").as_int();
	data.initColor.alpha = config.child("initialColor").attribute("a").as_int();

	data.finalColor.r = config.child("finalColor").attribute("r").as_int();
	data.finalColor.g = config.child("finalColor").attribute("g").as_int();
	data.finalColor.b = config.child("finalColor").attribute("b").as_int();
	data.finalColor.alpha = config.child("finalColor").attribute("a").as_int();

	data.blendMode = (SDL_BlendMode)config.child("blendMode").attribute("value").as_int();

	data.texPath = config.child("texturePath").attribute("value").as_string("Path error");

	data.initSpin = config.child("spin").attribute("initialValue").as_float();
	float finalSpin = config.child("spin").attribute("finalValue").as_float();
	float finalSpinDiff = config.child("spin").attribute("variation").as_float();
	data.finalSpin = operationRand(finalSpin, finalSpinDiff);

	data.drawingOrder = config.child("drawingOrder").attribute("value").as_int();

	data.isGravityOn = config.child("gravity").attribute("value").as_bool();
}

Particle* ParticleEmitter::newParticle()
{
	ParticleInfo data;

	particleConfig(data);
	emissionTimer.Start();

	return App->particlesys->newParticle(data);
}

float ParticleEmitter::randomZeroToOne()const
{
	return ((double)rand() / (RAND_MAX)) + 1;
}

float ParticleEmitter::operationRand(float var, float diff)const
{
	float variable = var;

	float difference = randomZeroToOne()*(2 * diff);

	variable -= diff;
	variable += difference;

	return variable;
}
#endif // !_PARTICLE_EMITTER_H_
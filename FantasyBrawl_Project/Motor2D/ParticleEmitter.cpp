#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_


#include "ParticleEmitter.h"
#include "j1App.h"


ParticleEmitter::ParticleEmitter(fPoint pos, std::string configPath)
{
	startingPos = pos;


}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::Update(float dt)
{

}

void ParticleEmitter::loadParticle(pugi::xml_document& file, pugi::xml_node& config, std::string path)
{
	
}

void ParticleEmitter::particleConfig(ParticleInfo& info)
{
	float angle
}

#endif // !_PARTICLE_EMITTER_H_
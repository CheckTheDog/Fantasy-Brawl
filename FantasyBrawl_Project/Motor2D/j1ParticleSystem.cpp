#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1ParticleSystem.h"
#include "ParticleEmitter.h"


j1ParticleSystem::j1ParticleSystem()
{

}

j1ParticleSystem::~j1ParticleSystem()
{

}

bool j1ParticleSystem::Update(float dt)
{
	debugPTimer.Start();
	updateParticles();
	updateEmitters();
	updateTime = debugPTimer.Read();

	return true;
}

Particle* j1ParticleSystem::newParticle(ParticleInfo data)
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (particles[i].isActive() == false)
		{
			particles[i].Init(data);
			
			return &particles[i];
		}
	}
}

ParticleEmitter* j1ParticleSystem::newEmitter(fPoint pos, const char* configPath)
{
	ParticleEmitter* emitter = nullptr;

	emitter = new ParticleEmitter(pos, configPath);

	pEmitters.push_back(emitter);
	return emitter;

}

void j1ParticleSystem::updateParticles()
{
	particleCount = 0;

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (particles[i].isActive() == false)
			continue;

		particles[i].Animate();

		if (particles[i].isActive() == false)
			continue;

		particles[i].Draw();
		particleCount++;
	}
}

void j1ParticleSystem::updateEmitters()
{
	std::list<ParticleEmitter*>::iterator it = pEmitters.begin();
	while (it != pEmitters.end())
	{
		if (!(*it)->isActive)
		{
			pEmitters.erase(it++);  // alternatively, i = items.erase(i);
		}
		else
		{
			(*it)->Update(0);
			++it;
		}
	}
	
}
#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1ParticleSystem.h"

ParticleInfo;

class ParticleEmitter
{
public:

	ParticleEmitter(fPoint pos, std::string configPath);
	~ParticleEmitter();

	Particle* newParticle();

	void Update(float dt);

public:

	bool isActive = true;

	fPoint startingPos;

	int life;
	int framesLeft;

private:

	void loadParticle(pugi::xml_document& file, pugi::xml_node& config, std::string path);
	void particleConfig(ParticleInfo& info);

private:

	pugi::xml_document configFile;
	pugi::xml_node config;

};
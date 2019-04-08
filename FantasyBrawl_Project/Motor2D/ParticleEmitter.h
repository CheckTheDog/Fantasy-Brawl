#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1ParticleSystem.h"
#include "j1Timer.h"

enum emitterType 
{
		NONE,
		BASIC_ATTACK,
		SPECIAL_ABILITY,
		DEATH,
		ITEM_PICK
};

ParticleInfo;

class ParticleEmitter
{
public:

	ParticleEmitter(fPoint pos, const char* configPath);
	~ParticleEmitter();

	Particle* newParticle();

	void Update(float dt);
	float randomZeroToOne()const;
	float operationRand(float var, float diff) const;

public:

	bool isActive = true;

	emitterType type;

	fPoint initPos;

	int lifeTime;
	int framesLeft;

	j1Timer emissionTimer;
	float period;
	float freq;

	float lifeTimeRation = 1.0f;

	float initScale = 1.0f;
	float scale = 1.0f;
	float finalScale = 0.0f;

	int maxEmissions;
	int currEmissions;

private:

	bool loadParticle(pugi::xml_document& file, pugi::xml_node& config, std::string path);
	void particleConfig(ParticleInfo& data);

private:

	pugi::xml_document configFile;
	pugi::xml_node config;

};
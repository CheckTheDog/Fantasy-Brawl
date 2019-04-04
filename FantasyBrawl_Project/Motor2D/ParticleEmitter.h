#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1ParticleSystem.h"

class ParticleEmitter
{
public:

	ParticleEmitter();
	~ParticleEmitter();

	void Update(float dt);

public:

	bool isActive = true;

	fPoint startingPos;

	int life;

private:

	pugi::xml_document configFile;
	pugi::xml_node config;

};
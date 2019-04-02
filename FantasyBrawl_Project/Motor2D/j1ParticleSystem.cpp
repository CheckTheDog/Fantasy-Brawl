#include "j1App.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1ParticleSystem.h"


j1ParticleSystem::j1ParticleSystem()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)

		isActive[i] = nullptr;
}

j1ParticleSystem::~j1ParticleSystem()
{

}

bool j1ParticleSystem::Awake(pugi::xml_node& conf)
{

}

bool j1ParticleSystem::Start()
{
	bool ret = true;

	graphics = App->tex->Load("textures/Wendolin Red Dagger.png");
	
	if (graphics == nullptr)
	{
		LOG("Particle sprites were not loaded correctly");
		ret = false;
	}

		return ret;
}

bool j1ParticleSystem::CleanUp()
{
	App->tex->UnLoad(graphics);

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (isActive[i] != nullptr)
		{
			delete isActive[i];
			isActive[i] = nullptr;
		}
	}
}

bool j1ParticleSystem::Update(float dt)
{

}

void Particle::PushBack(const SDL_Rect& rect)
{
	frames[last_frame++] = rect;
}

void j1ParticleSystem::LoadParticle(const char* filePath, const char* fileName) {

	Particle* p_anim = new Particle();

	bool anim = false;

	pugi::xml_document particleDoc;
	pugi::xml_parse_result result = particleDoc.load_file(filePath);


	if (result == NULL)
	{
		LOG("Issue loading animation");
	}

	pugi::xml_node objgroup;
	for (objgroup = particleDoc.child("map").child("objectgroup"); objgroup; objgroup = objgroup.next_sibling("objectgroup"))
	{
		std::string name = objgroup.attribute("name").as_string();
		if (name == fileName)
		{
			anim = true;
			int x, y, h, w;

			for (pugi::xml_node sprite = objgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				p_anim->PushBack({ x, y, w, h });
			}

		}
	}
	/*if (anim == true)
		return p_anim;
	else
		return nullptr;*/

}

void j1ParticleSystem::AddParticle(particleType yype, fPoint pos)
{

}
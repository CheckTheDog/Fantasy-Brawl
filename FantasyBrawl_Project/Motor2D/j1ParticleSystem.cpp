#include "j1ParticleSystem.h"
#include "j1App.h"
#include "j1Textures.h"
#include"j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Player.h"

#include "SDL/include//SDL_timer.h"

j1ParticleSystem::j1ParticleSystem()
{
	for (uint i = 0; i < MAX_PARTICLES; i++)
	{
		active[i] = nullptr;
	}
}

j1ParticleSystem::~j1ParticleSystem()
{

}

bool j1ParticleSystem::Start()
{
	
	LOG("Loading particle sprites");
	pSprites = App->tex->Load("particles/Wendolin Red Dagger.png");

	if (pSprites == nullptr)
		LOG("Cannot load particle sprites");

	return true;
}

bool j1ParticleSystem::CleanUp()
{
	App->tex->UnLoad(pSprites);

	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

bool j1ParticleSystem::Update(float dt)
{

	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update(dt) == false)
		{
			if (p->toDelete) {
				p->pCol->to_delete = true;
				delete p;
				active[i] = nullptr;
			}

		}
		else //if (SDL_GetTicks() >= p->born)
		{
			App->render->Blit(pSprites, p->pos.x, p->pos.y, &(p->anim.GetCurrentFrame(dt)));
			App->coll->QueryCollisions(*p->pCol);
		}
	}

	return true;
}

void j1ParticleSystem::AddParticle(Particle& particle, int x, int y, COLLIDER_TYPE collider_type, uint delay, j1Player* porigin)
{
	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks();
			p->pos.x = x;
			p->pos.y = y;
			p->delay = delay;
			p->originplayer = porigin;
			if (collider_type != COLLIDER_TYPE::COLLIDER_NONE) {
				p->pCol = App->coll->AddCollider(p->anim.GetCurrentFrame(0), collider_type,this);
				active[i] = p;
				break;
			}
		}
	}
	
}

void j1ParticleSystem::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		// Always destroy particles that collide (except bombs )
		if (active[i] != nullptr && active[i]->pCol == c1)
		{
			/*if (c1->type == COLLIDER_TYPE::COLLIDER_PARTICLE) {}*/

		
				//AddParticle(explosion, active[i]->position.x, active[i]->position.y);
				/*delete active[i];
				active[i] = nullptr;*/
			if (active[i]->originplayer != nullptr)
			{
				if (c2 != active[i]->originplayer->Entityinfo.entitycoll)
				{
					active[i]->toDelete = true;
				}
			}
			else
			{
				active[i]->toDelete = true;
			}

			if (active[i]->toDelete) {
				active[i]->pCol->to_delete = true;
				delete active[i];
				active[i] = nullptr;
			}

				break;
			
		}
	}
}

Particle::Particle()
{
	pos.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), pos(p.pos), speed(p.speed),
	born(p.born), life(p.life), delay(p.delay)
{}

Particle::~Particle()
{

}

fPoint Particle::GetPos()const
{
	return pos;
}

bool Particle::Update(float dt)
{
	bool ret = true;

	uint pLife = SDL_GetTicks() - born;
	if (life > 0)
	{
		if (pLife < delay) {
			ret = false;
		}
		else if (pLife > life + delay) {
			toDelete = true;
			ret = false;
		}
		else {
			pos.x += speed.x*dt;
			pos.y += speed.y*dt;
		}

	}
	else
		if (anim.Finished())
			ret = false;




	if (pCol != nullptr) {
		pCol->SetPos(pos.x, pos.y);
	}

	return ret;
}
#include "j1ParticleSystem.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1Viewport.h"
#include "j1BuffManager.h"

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


	return true;
}

bool j1ParticleSystem::CleanUp()
{
	//App->tex->UnLoad(pSprites);

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
			App->view->PushQueue(6,p->tex, p->pCol->rect.x, p->pCol->rect.y, p->anim.GetCurrentFrame(dt),0,0,p->angle*(180.0f / M_PI) - 180.0f,p->pCol->rect.w/2, p->pCol->rect.h / 2,scale);

			//LOG("p.angle: %f", p->angle);
			App->coll->QueryCollisions(*p->pCol);
		}
	}

	return true;
}

Particle* j1ParticleSystem::AddParticle(Particle& particle, int x, int y, COLLIDER_TYPE collider_type, uint delay, j1Player* porigin)
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
			p->particle_effect = &App->buff->effects[3];
			p->angle = particle.angle;
			p->speed.x = particle.speed.x;
			p->speed.y = particle.speed.y;
			p->direction.x = particle.direction.x;
			p->direction.y = particle.direction.y;
			p->direction.x *= p->speed.x;
			p->direction.y *= p->speed.y;
			p->tex = particle.tex;
			p->ghost = particle.ghost;

			if (collider_type != COLLIDER_TYPE::COLLIDER_NONE) {
				p->pCol = App->coll->AddCollider(p->anim.GetCurrentFrame(0), collider_type,this);

				/*if (p->ghost)
					p->pCol->ghost = true;*/

				p->pCol->rect.w *= scale;
				p->pCol->rect.h *= scale;

				active[i] = p;
				return p;
			}
		}
	}
	
}

Particle * j1ParticleSystem::GetCollidedParticle(Collider* hitbox, const Collider * particlecollider)
{
	Particle* to_return = nullptr;

	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			if (active[i]->pCol == particlecollider)
			{
				to_return = active[i];
				break;
			}
		}
	}

	if (to_return && to_return->originplayer != nullptr)
	{
		if (hitbox != to_return->originplayer->Entityinfo.HitBox)
		{
			to_return->toDelete = true;
		}
	}


	return to_return;
}

void j1ParticleSystem::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		if (active[i] != nullptr && active[i]->pCol == c1)
		{
		
			if (!c1->ghost)
			{
				if (c2->type != COLLIDER_TYPE::COLLIDER_HITBOX)
					active[i]->toDelete = true;


				if (active[i]->toDelete) {
					active[i]->pCol->to_delete = true;
					delete active[i];
					active[i] = nullptr;
				}
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
			/*direction.x *= speed.x*dt;
			direction.y *= speed.y*dt;*/
		/*	pos.x += direction.x*dt;
			pos.y += direction.y*dt;*/

			/*LOG("cos: %f", cosf(this->angle));
			LOG("sin: %f", sinf(this->angle));
			LOG("angle: %f", angle);*/

			pos.x += cosf(this->angle)*speed.x*dt;
			pos.y += sinf(this->angle)*speed.y*dt;
			
		}

	}
	else
		if (anim.Finished())
			ret = false;




	if (pCol != nullptr) {
		pCol->SetPos(pos.x, pos.y);
	}

	if (this->toDelete)
		ret = false;

	return ret;
}
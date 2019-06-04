#include "j1ParticleSystem.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1Viewport.h"
#include "j1BuffManager.h"
#include "j1EntityManager.h"

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
	meliadoulAXE.speed.x = 0.0f;
	meliadoulAXE.speed.y = 0.0f;
	meliadoulAXE.life = 3000;
	meliadoulAXE.tex = App->entities->axe_texture;
	meliadoulAXE.anim.loop = true;
	meliadoulAXE.anim.PushBack({ 0,0,40,34 });

	return true;
}

bool j1ParticleSystem::CleanUp()
{
	//App->tex->UnLoad(pSprites);

	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			active[i]->pCol->to_delete = true;
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
			uint pLife = SDL_GetTicks() - p->born;

			if (p && p->originplayer && !p->returned && p->life > 0 && p->pCol->type == COLLIDER_TYPE::COLLIDER_PARTICLE)
			{
				if (pLife > p->life + p->delay && p->originplayer->character == CHARACTER::MELIADOUL)
				{
					meliadoulAXE.angle = p->angle;
					p->originplayer->MeliadoulAXES.push_back(AddParticle(meliadoulAXE, p->pos.x, p->pos.y, COLLIDER_TYPE::COLLIDER_FLOOR, 0, p->originplayer));
				}
			}


			if (p->toDelete) {
				if(p->originplayer && p->originplayer->last_particle == p)
				p->originplayer->last_particle = nullptr;

				p->pCol->to_delete = true;
				delete p;
				active[i] = nullptr;
			}

		}
		else //if (SDL_GetTicks() >= p->born)
		{
			if(p->pCol->type == COLLIDER_TYPE::COLLIDER_PNI)
			App->view->PushQueue(9,p->tex, (int)p->pos.x, (int)p->pos.y, p->anim.GetCurrentFrame(dt),0,0);
			else if (p->pCol->type == COLLIDER_TYPE::COLLIDER_BOUNCE)
			App->view->PushQueue(6, p->tex, p->pos.x, p->pos.y, p->anim.GetCurrentFrame(dt), 0, 0, p->angle, App->entities->parrytex_rect.w / 1.5, App->entities->parrytex_rect.h / 1.5, scale);
			else
			App->view->PushQueue(6, p->tex, (int)p->pos.x, (int)p->pos.y, p->anim.GetCurrentFrame(dt), 0, 0, p->angle*(180.0f / M_PI) - 180.0f, p->pCol->rect.w / 2, p->pCol->rect.h / 2, scale);

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
			if (p->speed.x > 0)
			{
				p->direction.x *= p->speed.x;
				p->direction.y *= p->speed.y;
			}
			p->tex = particle.tex;
			p->ghost = particle.ghost;
			p->anim = particle.anim;

			if (collider_type != COLLIDER_TYPE::COLLIDER_NONE) {
				p->pCol = App->coll->AddCollider(p->anim.GetCurrentFrame(0), collider_type,this);

				/*if (p->ghost)
					p->pCol->ghost = true;*/


					p->pCol->rect.x = x;
					p->pCol->rect.y = y;
					p->pCol->rect.w *= scale;
					p->pCol->rect.h *= scale;

					if (collider_type == COLLIDER_TYPE::COLLIDER_BOUNCE)
					{
						p->pCol->rect.x = p->originplayer->Entityinfo.position.x /*- p->pCol->rect.w/2*/ + 20 * p->direction.x;
						p->pCol->rect.y = p->originplayer->Entityinfo.position.y - p->pCol->rect.h/2 + 25 * p->direction.y;
					}
				active[i] = p;
				return p;
			}
		}
	}
	
}

Particle * j1ParticleSystem::GetCollidedParticle(Collider* hitbox, const Collider * particlecollider, bool player)
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
	
	if (player && to_return && to_return->originplayer != nullptr)
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
			if (c1->type == COLLIDER_TYPE::COLLIDER_BOUNCE)
			{
				Particle* pcollided = nullptr;
				pcollided = App->particlesys->GetCollidedParticle(c1, c2,false);

				Particle* parryP = nullptr;
				parryP = App->particlesys->GetCollidedParticle(c2, c1,false);

				if (pcollided && parryP && pcollided->originplayer != parryP->originplayer)
				{

					pcollided->life = 2000;
					pcollided->born = SDL_GetTicks();
					pcollided->direction.x = parryP->direction.x;
					pcollided->direction.y = parryP->direction.y;
					pcollided->angle = std::atan2(pcollided->direction.y, pcollided->direction.x);
					pcollided->originplayer->last_particle = nullptr;
					pcollided->originplayer = parryP->originplayer;
				}
			}

			else if (!c1->ghost)
			{
				Particle* c2P = nullptr;
				c2P = App->particlesys->GetCollidedParticle(c1, c2, false);
				Particle* c1P = nullptr;
				c1P = App->particlesys->GetCollidedParticle(c2, c1, false);

				if (c2P && c1P && c2P->originplayer == c1P->originplayer)
				{
					continue;
				}

					if (c2->type != COLLIDER_TYPE::COLLIDER_HITBOX)
					{
						active[i]->toDelete = true;
						//Create Hit Particle here
						Particle hit;
						hit.anim = App->entities->particle_hitanim;
						hit.anim.loop = false;
						hit.anim.speed = 30.0f;
						hit.tex = App->entities->particle_hittex;
						hit.life = 100;
						AddParticle(hit, c1->rect.x - 50, c1->rect.y - 50, COLLIDER_TYPE::COLLIDER_PNI, 0);
					}

					else if (c2->type != COLLIDER_TYPE::COLLIDER_PLAYER && active[i]->originplayer->Entityinfo.HitBox != c2)
					{
						Particle hit;
						hit.anim = App->entities->particle_hitanim;
						hit.anim.loop = false;
						hit.anim.speed = 30.0f;
						hit.tex = App->entities->particle_hittex;
						hit.life = 100;
						AddParticle(hit, c1->rect.x - 50, c1->rect.y - 50, COLLIDER_TYPE::COLLIDER_PNI, 0);
					}

					if (active[i]->toDelete) {
						if (active[i]->originplayer && active[i]->originplayer->last_particle == active[i])
							active[i]->originplayer->last_particle = nullptr;

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




	if (pCol != nullptr && this->pCol->type != COLLIDER_TYPE::COLLIDER_BOUNCE) {
		pCol->SetPos(pos.x, pos.y);
	}

	if (this->toDelete)
		ret = false;

	return ret;
}
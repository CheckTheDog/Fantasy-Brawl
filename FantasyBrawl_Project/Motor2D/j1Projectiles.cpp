#include "j1Projectiles.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1ParticleSystem.h"
#include "ParticleEmitter.h"
#include "p2Log.h"
#include "Animation.h"


j1Projectiles::j1Projectiles() {
}


j1Projectiles::~j1Projectiles() {
}


bool j1Projectiles::Awake(const pugi::xml_node& config) {

	sprites = App->tex->Load("textures/Wendolin Red Dagger.png");

	dagger.PushBack({});

	return true;
}

bool j1Projectiles::PreUpdate() {

	std::list<Projectile*> projectilesToDelete;
	// Remove all projectiles scheduled for deletion

	//Iterate list looking for the projectiles to delete
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* p = *it;
		if (p->coll->to_delete) { // Projectiles are deleted with their colliders
			projectilesToDelete.push_back(p);
		}
	}

	// Remove the projectiles
	for (std::list<Projectile*>::iterator it = projectilesToDelete.begin(); it != projectilesToDelete.end(); ++it) {
		Projectile* p = *it;
		projectiles.remove(p);
		if (p->emitter)
			p->emitter->isActive = false;
		delete p;
	}

	projectilesToDelete.clear();
	return true;
}

bool j1Projectiles::Update(float dt) {

	// Logical update
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* p = *it;
		p->Update();
	}
	// Drawing
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* p = *it;
		p->Draw(sprites);
	}

	return true;
}

bool j1Projectiles::CleanUp() {
	LOG("Freeing all projectiles");

	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* p = *it;
		delete p;
	}

	projectiles.clear();
	return true;
}

Projectile* j1Projectiles::AddProjectile(PROJECTILE_TYPE type, iPoint position, iPoint speed, Collider* collider, int life, bool flipped, int scale, ParticleEmitter* emitter, iPoint emitterOffset, float angularVel)
{
	Projectile* newProjectile = nullptr;
	
	switch(type)
	{
		case BASIC_ATTACK:
		newProjectile = new Projectile(&dagger, position, speed, collider, life, flipped, scale, type, emitter, emitterOffset);
		break;
	}

	newProjectile->angularVel = angularVel;
	projectiles.push_back(newProjectile);
	return newProjectile;
}

int j1Projectiles::ProjectileType(PROJECTILE_TYPE type, j1Player* player) {

	int counter = 0;
	for (std::list<Projectile*>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		Projectile* p = *it;
		if (p->pType == type && p->coll->type ==	COLLIDER_TYPE::COLLIDER_PLAYER)
		{
			counter++;
		}
	}
	return counter;
}

void Projectile::Update() {

	if (coll!= nullptr) {
		coll->SetPos(pos.x, pos.y);

		if (emitter) {
			emitter->initPos.x = (float)pos.x + coll->rect.w / 2 + emitterOffset.x;
			emitter->initPos.y = (float)pos.y + coll->rect.h / 2 + emitterOffset.y;
		}
	}
	
}
void Projectile::Draw(SDL_Texture* sprites)
{
	j1Timer delta;
	float dt = delta.ReadSec()/1000;
	if (&anim)
		App->render->Blit(sprites, pos.x, pos.y, &anim->GetCurrentFrame(dt), scale, isFlipped, 1.0f, angle);
}
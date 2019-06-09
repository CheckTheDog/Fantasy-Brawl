#include "j1App.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Timer.h"
#include "j1BuffManager.h"
#include "p2Log.h"



j1BuffManager::j1BuffManager() : j1Module()
{
	name.assign("BuffManager");
}

j1BuffManager::~j1BuffManager()
{

}


bool j1BuffManager::Awake()
{

	return true;
}


bool j1BuffManager::Start()
{
	pugi::xml_parse_result res = buffmanager_xml.load_file("buffmanager.xml");;
	node = buffmanager_xml.document_element();

	LoadEffects(node);

	return true;
}

bool j1BuffManager::Update(float dt)
{
	// better optimization if this is done on the update of every entity --> (&effect, this)

	// TEMPORARY EFFECT
	//RestartAttribute(&effects[WAR_CRY], App->player);

	// PER TICK
	//ApplyEachTick(&effects[POISON], App->player);

	// LIMIT THE ATTRIBUTES OF Entities  
	//LimitAttributes(App->player);


	return true;
}

bool j1BuffManager::CleanUp()
{
	// this makes the project crash on clean-up
	//delete effects;

	return true;
}

void j1BuffManager::ApplyEffect(Effect* effect, j1Entity *entity)
{

	if (effect->duration_type == PERMANENT)
	{
		switch (effect->attribute_to_change)
		{
		case HEALTH:
			DoMath(entity->Entityinfo.health, effect->bonus, effect->method, effect->type);
			DoMath(entity->Entityinfo.og_health, effect->bonus, effect->method, effect->type);
			break;

		case STRENGTH:
			DoMath(entity->Entityinfo.strength, effect->bonus, effect->method, effect->type);
			DoMath(entity->Entityinfo.og_strength, effect->bonus, effect->method, effect->type);
			break;

			/*case ARMOR:
				DoMath(entity->armor, effect->bonus, effect->method, effect->type);
				DoMath(entity->og_armor, effect->bonus, effect->method, effect->type);
				break;*/

		case SPEED:
			DoMath(entity->Entityinfo.Speed, effect->bonus, effect->method, effect->type);
			DoMath(entity->Entityinfo.og_speed, effect->bonus, effect->method, effect->type);
			break;

		case COOLDOWN:
			if (effect->type == BUFF)
				entity->Entityinfo.player->superTimer.Add(effect->bonus);
			else
				entity->Entityinfo.player->superTimer.Subtract(effect->bonus);
			break;
		}
	}
	else if (effect->duration_type == TEMPORARY) // we have to put manually every NEW EFFECT that has a TIMER (and create the timer in entity.h or in this case in Player.h)
	{
		switch (effect->attribute_to_change)
		{
		case HEALTH:
			if (effect->name == effects[HEAL].name)
			{
				if (entity->Entityinfo.heal_active == false)
				{
					DoMath(entity->Entityinfo.health, effect->bonus, effect->method, effect->type);
					entity->Entityinfo.heal_active = true;
				}
				entity->Entityinfo.healing.Start(); // timer starts
			}
			break;

		case STRENGTH:

			if (effect->name == effects[WAR_CRY].name)
			{
				if (entity->Entityinfo.war_cry_active == false)
				{
					DoMath(entity->Entityinfo.strength, effect->bonus, effect->method, effect->type);
					entity->Entityinfo.war_cry_active = true;
				}
				entity->Entityinfo.war_cry.Start();
			}
			break;

			/*case ARMOR:
				DoMath(entity->armor, effect->bonus, effect->method, effect->type);
				break;*/

		case SPEED:
			if (effect->name == effects[EXHAUST].name)
			{
				if (entity->Entityinfo.exhaust_active == false)
				{
					DoMath(entity->Entityinfo.Speed, effect->bonus, effect->method, effect->type);
					entity->Entityinfo.exhaust_active = true;
				}
				entity->Entityinfo.exhausting.Start(); // timer starts
			}

			if (effect->name == effects[SPEED_UP].name)
			{
				if (entity->Entityinfo.speed_up_active == false)
				{
					DoMath(entity->Entityinfo.Speed, effect->bonus, effect->method, effect->type);
					entity->Entityinfo.speed_up_active = true;
				}
				entity->Entityinfo.speed_up.Start(); // timer starts
			}
			break;
		}
	}
	/*else if (effect->duration_type == PER_TICK)// we have to put manually every NEW EFFECT that has a TIMER (and create the timer in entity.h or in this case Player.h)
	{
		switch (effect->attribute_to_change)
		{
		case HEALTH:
			if (effect->name == effects[POISON].name)
			{
				if (entity->Entityinfo.poison_tick_active == false)
				{
					entity->Entityinfo.poison_tick_active = true;
				}
				entity->Entityinfo.poison_tick.Start(); // start or restart timer
				entity->Entityinfo.poison_tick_iterator = 0; //restart iterator
			}
			break;
		case STRENGTH:
			break;
		case ARMOR:
			break;
		case SPEED:
			break;
		}
	}*/

	LimitAttributes(entity);

}

void j1BuffManager::ApplyEffect(Effect * effect, j1Entity * entity, float edited_bonus)
{
	if (effect->duration_type == PERMANENT)
	{
		switch (effect->attribute_to_change)
		{
		case HEALTH:
			DoMath(entity->Entityinfo.health, edited_bonus, effect->method, effect->type);
			DoMath(entity->Entityinfo.og_health, edited_bonus, effect->method, effect->type);
			break;

		case STRENGTH:
			DoMath(entity->Entityinfo.strength, edited_bonus, effect->method, effect->type);
			DoMath(entity->Entityinfo.og_strength, edited_bonus, effect->method, effect->type);
			break;

		case COOLDOWN:
			if (effect->type == BUFF)
				entity->Entityinfo.player->superTimer.Add(edited_bonus);
			else
				entity->Entityinfo.player->superTimer.Subtract(edited_bonus);
			break;
		}
	}
	else if (effect->duration_type == TEMPORARY) // we have to put manually every NEW EFFECT that has a TIMER (and create the timer in entity.h or in this case in Player.h)
	{
		switch (effect->attribute_to_change)
		{
		case HEALTH:
			if (effect->name == effects[HEAL].name)
			{
				if (entity->Entityinfo.heal_active == false)
				{
					DoMath(entity->Entityinfo.health, edited_bonus, effect->method, effect->type);
					entity->Entityinfo.heal_active = true;
				}
				entity->Entityinfo.healing.Start(); // timer starts
			}
			break;

		case STRENGTH:

			if (effect->name == effects[WAR_CRY].name)
			{
				if (entity->Entityinfo.war_cry_active == false)
				{
					DoMath(entity->Entityinfo.strength, edited_bonus, effect->method, effect->type);
					entity->Entityinfo.war_cry_active = true;
				}
				entity->Entityinfo.war_cry.Start();
			}
			break;

		case SPEED:
			if (effect->name == effects[EXHAUST].name)
			{
				if (entity->Entityinfo.exhaust_active == false)
				{
					DoMath(entity->Entityinfo.Speed, edited_bonus, effect->method, effect->type);
					entity->Entityinfo.exhaust_active = true;
				}
				entity->Entityinfo.exhausting.Start(); // timer starts
			}

			if (effect->name == effects[SPEED_UP].name)
			{
				if (entity->Entityinfo.speed_up_active == false)
				{
					DoMath(entity->Entityinfo.Speed, edited_bonus, effect->method, effect->type);
					entity->Entityinfo.speed_up_active = true;
				}
				entity->Entityinfo.speed_up.Start(); // timer starts
			}
			break;
		}
	}
}

void j1BuffManager::DoMath(float &att_value, float bonus, EffectMethod method, EffectType eff_type)
{
	if (eff_type == BUFF)
	{
		if (method == ADD)
		{
			att_value += bonus;
		}
		else if (method == MULTIPLY)
		{
			att_value *= bonus;
		}
		else if (method == PERCENTAGE)
		{
			att_value += att_value * (bonus / 100);
		}
	}
	else if (eff_type == DEBUFF)
	{
		if (method == ADD)
		{
			att_value -= bonus;
		}
		else if (method == MULTIPLY)
		{
			att_value /= bonus;
		}
		else if (method == PERCENTAGE)
		{
			att_value -= att_value * (bonus / 100);
		}
	}

}

void j1BuffManager::RestartAttribute(Effect *effect, j1Entity *entity) //Check all the TEMPORAY effects of an entity and 
{
	if (effect->name == effects[HEAL].name)
	{
		if (entity->Entityinfo.heal_active == true && entity->Entityinfo.healing.ReadSec() > effect->duration_value)
		{
			entity->Entityinfo.health = entity->Entityinfo.og_health;
			entity->Entityinfo.heal_active = false;
		}
	}
	else if (effect->name == effects[WAR_CRY].name)
	{
		if (entity->Entityinfo.war_cry_active == true && entity->Entityinfo.war_cry.ReadSec() > effect->duration_value)
		{
			entity->Entityinfo.strength = entity->Entityinfo.og_strength;
			entity->Entityinfo.war_cry_active = false;
		}
	}
	else if (effect->name == effects[EXHAUST].name)
	{
		if (entity->Entityinfo.exhaust_active == true && entity->Entityinfo.exhausting.ReadSec() > effect->duration_value)
		{
			entity->Entityinfo.Speed = entity->Entityinfo.og_speed;
			entity->Entityinfo.exhaust_active = false;
		}
	}
	else if (effect->name == effects[SPEED_UP].name)
	{
		if (entity->Entityinfo.speed_up_active == true && entity->Entityinfo.speed_up.ReadSec() > effect->duration_value)
		{
			entity->Entityinfo.Speed = entity->Entityinfo.og_speed;
			entity->Entityinfo.speed_up_active = false;
		}
	}
}

void j1BuffManager::ApplyEachTick(Effect * effect, j1Entity * entity) //Check all the PER_TICK effects of an entity
{
	/*if (effect->name == effects[POISON].name)
	{
		if (entity->Entityinfo.poison_tick_active == true)
		{
			if (entity->Entityinfo.poison_tick.ReadSec() > effect->duration_value)
			{
				entity->Entityinfo.poison_tick_active = false;
				entity->Entityinfo.poison_tick_iterator = 0;
			}
			if (entity->Entityinfo.poison_tick.ReadSec() > entity->Entityinfo.poison_tick_iterator)
			{
				DoMath(entity->Entityinfo.health, effect->bonus, effect->method, effect->type);
				DoMath(entity->Entityinfo.og_health, effect->bonus, effect->method, effect->type);
				entity->Entityinfo.poison_tick_iterator++;
			}
		}
	}*/

}

void j1BuffManager::LimitAttributes(j1Entity * entity)
{
	// HEALTH ATT
	if (entity->Entityinfo.og_health > MAX_HEALTH)
		entity->Entityinfo.og_health = MAX_HEALTH;
	else if (entity->Entityinfo.og_health < MIN_HEALTH)
		entity->Entityinfo.og_health = MIN_HEALTH;

	if (entity->Entityinfo.health > MAX_HEALTH)
		entity->Entityinfo.health = MAX_HEALTH;
	else if (entity->Entityinfo.health < MIN_HEALTH)
		entity->Entityinfo.health = MIN_HEALTH;

	// STRENGTH ATT
	if (entity->Entityinfo.og_strength > MAX_STRENGTH)
		entity->Entityinfo.og_strength = MAX_STRENGTH;
	else if (entity->Entityinfo.og_strength < MIN_STRENGTH)
		entity->Entityinfo.og_strength = MIN_STRENGTH;

	if (entity->Entityinfo.strength > MAX_STRENGTH)
		entity->Entityinfo.strength = MAX_STRENGTH;
	else if (entity->Entityinfo.strength < MIN_STRENGTH)
		entity->Entityinfo.strength = MIN_STRENGTH;

	// ARMOR ATT
	/*if (entity->og_armor > MAX_ARMOR)
		entity->og_armor = MAX_ARMOR;
	else if (entity->og_armor < MIN_ARMOR)
		entity->og_armor = MIN_ARMOR;
	if (entity->armor > MAX_ARMOR)
		entity->armor = MAX_ARMOR;
	else if (entity->armor < MIN_ARMOR)
		entity->armor = MIN_ARMOR;*/

	// SPEED ATT
	if (entity->Entityinfo.og_speed > MAX_SPEED)
		entity->Entityinfo.og_speed = MAX_SPEED;
	else if (entity->Entityinfo.og_speed < MIN_SPEED)
		entity->Entityinfo.og_speed = MIN_SPEED;
	if (entity->Entityinfo.Speed > MAX_SPEED)
		entity->Entityinfo.Speed = MAX_SPEED;
	else if (entity->Entityinfo.Speed < MIN_SPEED)
		entity->Entityinfo.Speed = MIN_SPEED;
}

const int j1BuffManager::GetMaxHealth()
{
	return MAX_HEALTH;
}

void j1BuffManager::LoadEffects(pugi::xml_node & data)
{
	pugi::xml_node iterator;
	Effect effect;

	for (iterator = data.child("effect"); iterator; iterator = iterator.next_sibling("effect"))
	{
		effect.name = iterator.attribute("name").as_string();
		SetValue(effect, iterator.attribute("type").as_string());
		SetValue(effect, iterator.attribute("duration_type").as_string());
		SetValue(effect, iterator.attribute("method").as_string());
		SetValue(effect, iterator.attribute("att_to_change").as_string());
		effect.bonus = iterator.attribute("bonus").as_int();
		effect.duration_value = iterator.attribute("duration_value").as_int();

		effects[iterator.attribute("id").as_int()] = effect;
		CreatedEffects++;
	}

}

void j1BuffManager::SetValue(Effect &effect, std::string string)
{
	if (string == "BUFF")
	{
		effect.type = BUFF;
	}
	else if (string == "DEBUFF")
	{
		effect.type = DEBUFF;
	}
	else if (string == "PERMANENT")
	{
		effect.duration_type = PERMANENT;
	}
	else if (string == "TEMPORARY")
	{
		effect.duration_type = TEMPORARY;
	}
	else if (string == "PER_TICK")
	{
		effect.duration_type = PER_TICK;
	}
	else if (string == "ADD")
	{
		effect.method = ADD;
	}
	else if (string == "MULTIPLY")
	{
		effect.method = MULTIPLY;
	}
	else if (string == "PERCENTAGE")
	{
		effect.method = PERCENTAGE;
	}
	else if (string == "HEALTH")
	{
		effect.attribute_to_change = HEALTH;
	}
	else if (string == "STRENGTH")
	{
		effect.attribute_to_change = STRENGTH;
	}
	//else if (string == "ARMOR")
	//{
	//	effect.attribute_to_change = ARMOR;
	//}

	else if (string == "SPEED")
	{
		effect.attribute_to_change = SPEED;
	}
	else if (string == "COOLDOWN")
	{
		effect.attribute_to_change = COOLDOWN;
	}

}
#ifndef __j1BuffManager_H__
#define __j1BuffManager_H__

#include "j1Module.h"
#include "p2Point.h"

#define MAX_EFFECTS 10

#define MAX_HEALTH 300
#define MAX_STRENGTH 500
#define MAX_ARMOR 300
#define MAX_SPEED 20

#define MIN_HEALTH 0
#define MIN_STRENGTH 0
#define MIN_ARMOR 0
#define MIN_SPEED 0


struct SDL_Texture;

enum Attribute // All the attributes that entities have
{
	HEALTH,
	STRENGTH,
	ARMOR,
	SPEED
};

enum Effects  // Enum of all the current existing EFFECTS - SAME ORDER AS IN XML (ID'S MUST MATCH)
{
	HEAL,
	POISON,
	WAR_CRY,
	HURT,
	STORM
};


enum EffectType
{
	BUFF,
	DEBUFF
};

enum EffectTime
{
	PERMANENT,
	TEMPORARY,
	PER_TICK
};

enum EffectMethod
{
	ADD,
	MULTIPLY,
	PERCENTAGE
};


struct Effect
{
	std::string		name;

	EffectType		type;
	EffectTime		duration_type;
	EffectMethod	method;
	Attribute		attribute_to_change; //Attribute that the effect modifies

	int			bonus;
	int			duration_value;
};


class j1BuffManager :public j1Module
{
public:

	j1BuffManager();
	~j1BuffManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void ApplyEffect(Effect* effect, j1Entity *entity);
	void ApplyEffect(Effect* effect, j1Entity *entity, float edited_bonus);

	void DoMath(float &att_value, float bonus, EffectMethod method, EffectType eff_type);
	void RestartAttribute(Effect* effect, j1Entity *entity);
	void ApplyEachTick(Effect* effect, j1Entity *entity);

	void LimitAttributes(j1Entity *entity);

private:


	void LoadEffects(pugi::xml_node& data); // Load XML values
	void SetValue(Effect &effect, std::string string); // Used when LoadEffects() --> converts the strings from XML into the diferent enums types

public:

	Effect			effects[MAX_EFFECTS];
	int				CreatedEffects = 0;

private:

	pugi::xml_document buffmanager_xml;
	pugi::xml_node node;

};

#endif // __j1BuffManager_H__

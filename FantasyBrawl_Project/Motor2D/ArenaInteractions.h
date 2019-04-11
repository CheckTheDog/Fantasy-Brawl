#ifndef __ARENAINTERACTIONS_H__
#define __ARENAINTERACTIONS_H__

#include "j1Module.h"

class ArenaInteractions : public j1Module
{
public: /// Functions

	//Constructor and Destructor
	ArenaInteractions();
	virtual ~ArenaInteractions();
	
	bool Start();

	//Called each loop iteration
	bool Update(float dt);


private: /// Functions


public: /// Variables


private: /// Variables

};




#endif
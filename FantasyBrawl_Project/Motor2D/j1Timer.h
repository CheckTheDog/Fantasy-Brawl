#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	void Stop();
	void Continue();
	void Subtract(uint32 time);
	void Add(float seconds);
	void Limit(uint32 limit);

private:
	uint32	started_at;
	bool stopped = false;
	float stopped_read = 0;
};

#endif //__j1TIMER_H__
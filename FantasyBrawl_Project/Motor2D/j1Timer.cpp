// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
	stopped = false;
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	if (stopped == false)
		return float(SDL_GetTicks() - started_at) / 1000.0f;
	else
		return stopped_read / 1000.0f;
}

void j1Timer::Stop()
{
	stopped = true;
	stopped_read = SDL_GetTicks() - started_at;
}

void j1Timer::Continue()
{
	if (stopped == true)
	{
		stopped = false;
		started_at = SDL_GetTicks() - stopped_read;
	}
}

void j1Timer::Add(float seconds)
{
	started_at -= uint32(seconds * 1000.0f);
}

void j1Timer::Subtract(uint32 time)
{
	started_at += time*1000.0f;
}

void j1Timer::Limit(uint32 limit)
{
	if (((SDL_GetTicks() - started_at) / 1000.0f) > limit)
	{
		float diff = ((SDL_GetTicks() - started_at) / 1000.0f) - limit;
		started_at += diff * 1000.0f;
	}
}

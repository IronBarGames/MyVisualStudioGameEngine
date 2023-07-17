#ifndef FCPP_TIMER_H
#define FCPP_TIMER_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "OtherExtensions/SDL/include/SDL.h"
#include "MiscFunctions.h"

class SDL_Timer
{
private:
	int Ticks;
	int ResetTicks;

public:
	SDL_Timer(): Ticks(0), ResetTicks(0) {}

	void reset()
	{
		ResetTicks = SDL_GetTicks64();
	}

	float& Get()
	{
		Ticks = SDL_GetTicks64() - ResetTicks;
		float T = Ticks / 1000.f;
		return T;
	}
};
/*=========================================================================================================*/

class FrameTimer // optimized
{
private:
	Uint32 StartTime;

public:
	FrameTimer()
	{
		StartTime = 0;
	}

	void Reset()
	{
		StartTime = 0;
	}

	void start()
	{
		StartTime = SDL_GetTicks64();
	}

	Uint32& GetTime()
	{
		Uint32 T = SDL_GetTicks64() - StartTime;
		return T;
	}
};
//******************************************************************************************************************************************************************************************************************************************************************************
class FpsCounter // optimized
{
private:
	int framecount{};
	FrameTimer timer{};
	FrameTimer cTime{};
	int fps{};
	float FrameLimit{};

public:
	bool capped{ false }; // independant from vsync

	FpsCounter() {}

	inline void Start()
	{
		timer.start();
	}

	inline const int& GetFrames()
	{
		return framecount;
	}

	inline void SetLimit(float cap)
	{
		FrameLimit = 1000.f / cap;
	}

	inline void equate()
	{
		if (capped) cTime.start();
		fps = framecount / (timer.GetTime() / 1000.f);
		if (fps > 2000000) fps = 0;
	}

	inline const int& get()
	{
		return fps;
	}

	// replaces SDL_RenderPresent()
	void RenderFrame(SDL_Renderer* ren)
	{
		SDL_RenderPresent(ren);
		++framecount;
		if (capped)
		{
			int frameTicks = cTime.GetTime();
			if (frameTicks < FrameLimit)
			{
				SDL_Delay(FrameLimit - frameTicks);
			}
		}
	}

};

class MyClock
{
private:
	SDL_Timer t{};
	float Delay{};
public:
	MyClock() {}
	explicit MyClock(float d) : Delay(d) { t.reset(); }

	const float& ReadClock()
	{
		return t.Get();
	}

	const float& DELAY()
	{
		return Delay;
	}

	void SetDelay(float d)
	{
		Delay = d;
		t.reset();
	}

	const bool& Reached()
	{
		return (t.Get() >= Delay);
	}

	void Reset()
	{
		t.reset();
	}
};

#endif
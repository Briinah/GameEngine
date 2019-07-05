#include "Timing.h"
#include <SDL\SDL.h>

namespace Palico
{
	FpsLimiter::FpsLimiter(float maxFps) : maxFps(maxFps)
	{
	}

	FpsLimiter::~FpsLimiter()
	{
	}

	void FpsLimiter::beginFrame()
	{
		startTicks = SDL_GetTicks();
	}

	float FpsLimiter::end()
	{
		calculateFps();
		float frameTicks = SDL_GetTicks() - startTicks;
		// limit fps
		if (1000.0f / maxFps > frameTicks)
		{
			SDL_Delay(1000.0f / maxFps - frameTicks);
		}
		return fps;
	}

	void FpsLimiter::calculateFps()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();

		float currentTicks;
		currentTicks = SDL_GetTicks();

		frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = frameTime;
		prevTicks = currentTicks;

		int count;
		currentFrame++;
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float frameTimeAvg = 0;
		for (int i = 0; i < count; i++)
		{
			frameTimeAvg += frameTimes[i];
		}

		frameTimeAvg /= count;

		if (frameTimeAvg > 0)
		{
			fps = 1000.0f / frameTimeAvg;
		}
		else
		{
			fps = 60;
		}
	}

	Time::Time(FpsLimiter* fpsLimiter) : fpsLimiter(fpsLimiter)
	{
	}
	Time::~Time()
	{
	}
}
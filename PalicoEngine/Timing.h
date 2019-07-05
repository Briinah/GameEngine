#pragma once

#include <iostream>

namespace Palico
{
	class FpsLimiter
	{
	public:
		FpsLimiter(float targetFps);
		~FpsLimiter();

		void beginFrame();

		float end(); // will return fps

		float getTargetFps() { return maxFps; }
		float getFps() { return fps; }

	private:
		void calculateFps();

		float frameTime;
		float fps;
		float maxFps;
		unsigned int startTicks;
	};

	class Time
	{
	public:
		Time(FpsLimiter* fpsLimiter);
		~Time();

		float deltaTime()
		{
			return fpsLimiter->getTargetFps() / fpsLimiter->getFps();
		}

	private:
		FpsLimiter* fpsLimiter;
	};
}
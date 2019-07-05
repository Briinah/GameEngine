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

		const float MAX_DELTA_TIME = 1.0f;

		float getTotalDeltaTime();

	private:
		FpsLimiter* fpsLimiter;

		const float MS_PER_SECOND = 1000;
		const float DESIRED_FRAMERATE = MS_PER_SECOND / fpsLimiter->getTargetFps();
		float prevTicks;
		float totalDeltaTime;
	};
}
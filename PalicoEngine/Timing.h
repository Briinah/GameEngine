#pragma once
namespace Palico
{
	class FpsLimiter
	{
	public:
		FpsLimiter(float targetFps);
		~FpsLimiter();

		void beginFrame();

		float end(); // will return fps

	private:
		void calculateFps();

		float frameTime;
		float fps;
		float maxFps;
		unsigned int startTicks;
	};
}
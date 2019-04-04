#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4};
namespace Palico
{
	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int width, int height, unsigned int currentFlags);
		void swapBuffer();

		int getWidth() { return this->width; }
		int getHeight() { return this->height; }
	private:
		SDL_Window* sdlWindow;
		int width;
		int height;
	};

}
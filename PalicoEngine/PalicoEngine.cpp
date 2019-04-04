#include <SDL\SDL.h>
#include <GL\glew.h>

#include "PalicoEngine.h"

namespace Palico
{
	int init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}
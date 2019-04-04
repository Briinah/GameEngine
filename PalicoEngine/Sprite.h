#pragma once

#include <GL/glew.h>
#include "GLTexture.h"

#include <string>
namespace Palico
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, int width, int height, std::string texturePath);
		void draw();

	private:
		int x;
		int y;
		int width;
		int height;

		GLuint vboID;

		GLTexture texture;
	};

}
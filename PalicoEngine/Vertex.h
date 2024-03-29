#pragma once

#include <GL/glew.h>
#include "Color.h"

namespace Palico
{
	struct UV {
		float u;
		float v;
	};

	struct Position
	{
		float x;
		float y;
	};

	// make structs a multiple of 4 bytes, so alignment in de gpu is better
	struct Vertex
	{
		Position position;

		Color color;

		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
	};
}
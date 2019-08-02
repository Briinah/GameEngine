#pragma once
#include <GL/glew.h>

namespace Palico
{
	class Color
	{
	public:

		Color()
			: r(0), g(0), b(0), a(1)
		{}
		Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
			: r(r), g(g), b(b), a(a)
		{}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;

		static const Color white;
		static const Color black;
	};
}
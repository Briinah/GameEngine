#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <SDL_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "Vertex.h"

namespace Palico
{
	class GLTexure;
	class SpriteBatch;

	struct Char
	{
	public:
		char character;
		glm::vec4 uvRect;
		glm::vec2 size;
	};

constexpr auto FIRST_PRINTABLE_CHAR = ((char)32);
constexpr auto LAST_PRINTABLE_CHAR = ((char)126);

	// for text alignment
	enum class Alignment
	{
		LEFT, MIDDLE, RIGHT
	};

	class SpriteFont
	{
	public:
		SpriteFont(const char* font, int size, char cs, char ce);
		SpriteFont(const char* font, int size) :
			SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR)
		{}

		//Destroys the font resources
		void dispose();

		int getFontHeight() const 
		{
			return fontHeight;
		}

		// Measures the dimensions of the text
		glm::vec2 measure(const char* s);

		// Draws using a spritebatch
		void draw(SpriteBatch& spriteBatch, const char* s, glm::vec2 position, glm::vec2 scaling,
			float depth, Color color, Alignment alignment = Alignment::LEFT);

	private:
		static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int rows, int padding, int& w);

		int regStart, regLength;
		Char* chars;
		int fontHeight;

		unsigned int textureID;
	};
}

#endif // SpriteFont_h__
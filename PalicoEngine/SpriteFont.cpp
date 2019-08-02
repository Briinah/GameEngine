#include "SpriteFont.h"

#include "SpriteBatch.h"

#include <SDL/SDL.h>

int closestPow2(int i)
{
	i--;
	int pi = 1;
	while (i > 0)
	{
		i >>= 1;
		pi <<= 1;
	}
	return pi;
}

namespace Palico
{
	SpriteFont::SpriteFont(const char* font, int size, char cs, char ce) : textureID(0), chars(0)
	{
		// initialize SDL_ttf
		if (!TTF_WasInit())
		{
			TTF_Init();
		}

		TTF_Font* f = TTF_OpenFont(font, size);
		if (f == nullptr)
		{
			fprintf(stderr, "Failed to open TTF font %s\n", font);
			fflush(stderr);
			throw 281;
		}
		fontHeight = TTF_FontHeight(f);
		regStart = cs;
		regLength = ce - cs + 1;
		int padding = size / 8;

		// first measure all the regions
		glm::ivec4* glyphRects = new glm::ivec4[regLength];
		int i = 0;
		int advance;

		for (char c = cs; c <= ce; c++)
		{
			TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
			glyphRects[i].z -= glyphRects[i].x;
			glyphRects[i].x = 0;
			glyphRects[i].w -= glyphRects[i].y;
			glyphRects[i].y = 0;

			i++;
		}

		int rows = 1;
		int w;
		int h;
		int bestWidth = 0;
		int bestHeight = 0;
		int area = 4096 * 4096;
		int bestRows = 0;

		std::vector<int>* bestPartition = nullptr;

		while (rows <= regLength)
		{
			h = rows * (padding + fontHeight) + padding;
			auto gr = createRows(glyphRects, regLength, rows, padding, w);

			// Desire a power of 2 texture
			w = closestPow2(w);
			h = closestPow2(h);
			// A texture must be feasible
			if (w > 4096 || h > 4096)
			{
				rows++;
				delete[] gr;
				continue;
			}

			// Check for minimal area
			if (area >= w * h)
			{
				if (bestPartition) delete[] bestPartition;
				bestPartition = gr;
				bestWidth = w;
				bestHeight = h;
				bestRows = rows;
				area = bestWidth * bestHeight;
				rows++;
			}
			else
			{
				delete[] gr;
				break;
			}
		}

		// Can a bitmap font be made?
		if (!bestPartition) return;

		// Create the texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);//86

		// Now draw all the glyphs
		SDL_Color fg = { 255, 255, 255, 255 };
		int ly = padding;
		for (int ri = 0; ri < bestRows; ri++)
		{
			int lx = padding;
			for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) 
			{
				int gi = bestPartition[ri][ci];
				SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(cs + gi), fg);

				// Pre-multiplication occurs here
				unsigned char* sp = (unsigned char*)glyphSurface->pixels;
				int cp = glyphSurface->w * glyphSurface->h * 4;
				for (int i = 0; i < cp; i += 4)
				{
					float a = sp[i + 3] / 255.0f;
					sp[i] = static_cast<unsigned char>(sp[i] * a);
					sp[i + 1] = sp[i];
					sp[i + 2] = sp[i];
				}

				// Save glyph image and update coordinates
				glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
				glyphRects[gi].x = lx;
				glyphRects[gi].y = ly;
				glyphRects[gi].z = glyphSurface->w;
				glyphRects[gi].w = glyphSurface->h;

				SDL_FreeSurface(glyphSurface);
				glyphSurface = nullptr;

				lx += glyphRects[gi].z + padding;
			}
			ly += fontHeight + padding;
		}

		// Draw the unsupported glyph
		int rs = padding - 1;
		int* pureWhiteSquare = new int[rs * rs];
		memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int)); //126
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
		delete[] pureWhiteSquare;
		pureWhiteSquare = nullptr;

		// Set some texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Create spriteBatch glyphs
		chars = new Char[regLength + 1];
		for (int i = 0; i < regLength; i++) 
		{
			chars[i].character = (char)(cs + i);
			chars[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
			chars[i].uvRect = glm::vec4(
				(float)glyphRects[i].x / (float)bestWidth,
				(float)glyphRects[i].y / (float)bestHeight,
				(float)glyphRects[i].z / (float)bestWidth,
				(float)glyphRects[i].w / (float)bestHeight
			);
		}
		chars[regLength].character = ' ';
		chars[regLength].size = chars[0].size;
		chars[regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

		glBindTexture(GL_TEXTURE_2D, 0);
		delete[] glyphRects;
		delete[] bestPartition;
		TTF_CloseFont(f);
	}

	void SpriteFont::dispose()
	{
		if (textureID != 0) 
		{
			glDeleteTextures(1, &textureID);
			textureID = 0;
		}
		if (chars) 
		{
			delete[] chars;
			chars = nullptr;
		}
	}

	std::vector<int>* SpriteFont::createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w)
	{
		std::vector<int>* l = new std::vector<int>[r]();
		int* cw = new int[r]();
		for (int i = 0; i < r; i++) {
			cw[i] = padding;
		}

		// Loop through all glyphs
		for (int i = 0; i < rectsLength; i++) 
		{
			// Find row for placement
			int ri = 0;
			for (int rii = 1; rii < r; rii++)
				if (cw[rii] < cw[ri]) ri = rii;

			// Add width to that row
			cw[ri] += rects[i].z + padding; // 186

			// Add glyph to the row list
			l[ri].push_back(i);
		}

		// Find the max width
		w = 0;
		for (int i = 0; i < r; i++) 
		{
			if (cw[i] > w) w = cw[i];
		}

		return l;
	}


	glm::vec2 SpriteFont::measure(const char* s)
	{
		glm::vec2 size(0, fontHeight);
		float cw = 0;
		for (int si = 0; s[si] != 0; si++) 
		{
			char c = s[si];
			if (s[si] == '\n') 
			{
				size.y += fontHeight;
				if (size.x < cw) size.x = cw;
				cw = 0;
			}
			else 
			{
				// Check for correct glyph
				int gi = c - regStart;
				if (gi < 0 || gi >= regLength) gi = regLength;
				cw += chars[gi].size.x;
			}
		}
		if (size.x < cw) size.x = cw;
		return size;
	}

	void SpriteFont::draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling, float depth, Color tint, Alignment alignment /* = Justification::LEFT*/) 
	{
		glm::vec2 tp = position;
		// Apply justification
		if (alignment == Alignment::MIDDLE) 
		{
			tp.x -= measure(s).x * scaling.x / 2;
		}
		else if (alignment == Alignment::RIGHT) 
		{
			tp.x -= measure(s).x * scaling.x;
		}
		for (int si = 0; s[si] != 0; si++) 
		{
			char c = s[si];
			if (s[si] == '\n') 
			{
				tp.y += fontHeight * scaling.y;
				tp.x = position.x;
			}
			else 
			{
				// check for correct glyph
				int gi = c - regStart;
				if (gi < 0 || gi >= regLength) gi = regLength;
				glm::vec4 destRect(tp, chars[gi].size * scaling);
				batch.draw(destRect, chars[gi].uvRect, textureID, depth, tint);
				tp.x += chars[gi].size.x * scaling.x;

			}
		}
	}
}
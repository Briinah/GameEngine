#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include "Vertex.h"

namespace Palico
{
	enum class SpriteSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Sprite 
	{
	public:

		Sprite() {};
		Sprite(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color)
			: texture(texture), depth(depth)
		{
			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint offset, GLuint numVerts, GLuint texture) : offset(offset), numVerts(numVerts), texture(texture) {};
		GLuint offset;
		GLuint numVerts;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();
		void begin(SpriteSortType sortType = SpriteSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortSprites();

		static bool compareFrontToBack(Sprite* a, Sprite* b);
		static bool compareBackToFront(Sprite* a, Sprite* b);
		static bool compareTexture(Sprite* a, Sprite* b);

		GLuint vbo;
		GLuint vao;

		SpriteSortType sortType;

		std::vector<Sprite*> spritePointers;
		std::vector<Sprite> sprites;
		std::vector<RenderBatch> renderBatches;
	};

}
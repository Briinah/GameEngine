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

	struct Sprite 
	{
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

		std::vector<Sprite*> sprites;
		std::vector<RenderBatch> renderBatches;
	};

}
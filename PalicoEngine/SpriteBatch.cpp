#include "SpriteBatch.h"
#include <algorithm>

namespace Palico
{
	SpriteBatch::SpriteBatch() : vbo(0), vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
		for (int i = 0; i < sprites.size(); ++i)
		{
			delete sprites[i];
		}
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(SpriteSortType sortType)
	{
		this->sortType = sortType;
		this->renderBatches.clear();
		this->sprites.clear();
	}

	void SpriteBatch::end()
	{
		sortSprites();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color)
	{
		Sprite* sprite = new Sprite();
		sprite->texture = texture;
		sprite->depth = depth;

		sprite->topLeft.color = color;
		sprite->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		sprite->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		sprite->bottomLeft.color = color;
		sprite->bottomLeft.setPosition(destRect.x, destRect.y);
		sprite->bottomLeft.setUV(uvRect.x, uvRect.y);

		sprite->bottomRight.color = color;
		sprite->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		sprite->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		sprite->topRight.color = color;
		sprite->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		sprite->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		sprites.push_back(sprite);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(vao);
		for (int i = 0; i < this->renderBatches.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVerts);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(sprites.size() * 6);

		if (sprites.empty()) return;

		int currentVert = 0;
		int offset = 0;

		for (int currentGlyph = 0; currentGlyph < sprites.size(); currentGlyph++)
		{
			if (currentGlyph == 0 || sprites[currentGlyph]->texture != sprites[currentGlyph - 1]->texture)
			{
				// same as push_back but you do not need to create an object first
				// just place the parameters of the constructor in it
				this->renderBatches.emplace_back(offset, 6, sprites[currentGlyph]->texture);
			}
			else
			{
				this->renderBatches.back().numVerts += 6;
			}
			vertices[currentVert++] = sprites[currentGlyph]->topRight;
			vertices[currentVert++] = sprites[currentGlyph]->topLeft;
			vertices[currentVert++] = sprites[currentGlyph]->bottomLeft;
			vertices[currentVert++] = sprites[currentGlyph]->bottomLeft;
			vertices[currentVert++] = sprites[currentGlyph]->bottomRight;
			vertices[currentVert++] = sprites[currentGlyph]->topRight;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // orphan the buffer
		
		// upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		// unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void SpriteBatch::createVertexArray()
	{
		if (vao == 0)
		{
			glGenVertexArrays(1, &vao);
		}
		glBindVertexArray(vao);

		if (vbo == 0)
		{
			glGenBuffers(1, &vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// tell opengl that we want to use the first 3 attribute arrays.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

	}
	void SpriteBatch::sortSprites()
	{
		switch (sortType)
		{
		case SpriteSortType::BACK_TO_FRONT:
			std::stable_sort(sprites.begin(), sprites.end(), compareBackToFront);
			break;
		case SpriteSortType::FRONT_TO_BACK:
			std::stable_sort(sprites.begin(), sprites.end(), compareFrontToBack);
			break;
		case SpriteSortType::TEXTURE:
			std::stable_sort(sprites.begin(), sprites.end(), compareTexture);
			break;
		}
	}
	bool SpriteBatch::compareFrontToBack(Sprite * a, Sprite * b)
	{
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Sprite * a, Sprite * b)
	{
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Sprite * a, Sprite * b)
	{
		return (a->texture < b->texture);
	}
}
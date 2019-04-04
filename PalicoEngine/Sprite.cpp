#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>
namespace Palico
{
	Sprite::Sprite() : vboID(0)
	{
	}


	Sprite::~Sprite()
	{
		if (vboID != 0)
		{
			glDeleteBuffers(1, &vboID);
		}
	}

	void Sprite::init(float x, float y, int width, int height, std::string texturePath)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->texture = ResourceManager::getTexture(texturePath);

		if (vboID == 0)
		{
			glGenBuffers(1, &vboID);
		}

		Vertex vertexData[6];

		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1, 1);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0, 1);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0, 0);

		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0, 0);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1, 0);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1, 1);


		for (int i = 0; i < 6; i++)
		{
			vertexData[i].setColor(255, 0, 255, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
#pragma once
#include <glm\glm.hpp>
#include <PalicoEngine\GLTexture.h>
#include <PalicoEngine\SpriteBatch.h>
#include <PalicoEngine\ResourceManager.h>
#include <string>

class Agent
{
public:
	Agent();
	~Agent();

	virtual void update();
	virtual void draw(Palico::SpriteBatch& spriteBatch);

	virtual void onCollision();

	void setPosition(glm::vec2 position)
	{
		this->position = position;
	}

	void setDirection(glm::vec2 direction)
	{
		this->direction = direction;
	}

	void setTexture(std::string texturePath)
	{
		texture = Palico::ResourceManager::getTexture(texturePath);
	}

	void setSpeed(float speed)
	{
		this->speed = speed;
	}


protected:

	glm::vec2 position;
	glm::vec2 direction;

	float speed;
	static Palico::GLTexture texture;

};


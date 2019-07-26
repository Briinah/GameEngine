#pragma once
#include <glm\glm.hpp>
#include <PalicoEngine\GLTexture.h>
#include <PalicoEngine\SpriteBatch.h>
#include <PalicoEngine\ResourceManager.h>
#include <string>
#include <iostream>

#include <PokemonGhost\Level.h>

// forward declaration, to prevent circular includes
class Ghost;
class Friendly;

const float AGENT_WIDTH = 64.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Agent
{
public:
	Agent();
	~Agent();

	virtual void update(float deltaTime, Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts);
	virtual void draw(Palico::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* other);

	glm::vec2 getPosition() 
	{ 
		return position; 
	}
	void setPosition(glm::vec2 newPosition)
	{
		this->position = newPosition;
	}
	void setPosition(float x, float y) 
	{ 
		position.x = x; 
		position.y = y; 
	}

	void setDirection(glm::vec2 newDirection)
	{
		this->direction = newDirection;
	}

	void setTexture(std::string textureName)
	{
		texture = Palico::ResourceManager::getTexture(textureName);
	}

	void setSpeed(float newSpeed)
	{
		this->speed = newSpeed;
	}


protected:

	glm::vec2 position;
	glm::vec2 direction;

	float speed;
	Palico::GLTexture texture;

	virtual bool handleLevelCollision(Level* level);
};


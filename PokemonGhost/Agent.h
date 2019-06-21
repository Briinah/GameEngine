#pragma once
#include <glm\glm.hpp>
#include <PalicoEngine\GLTexture.h>
#include <PalicoEngine\SpriteBatch.h>
#include <PalicoEngine\ResourceManager.h>
#include <string>

#include <PokemonGhost\Level.h>

// forward declaration, to prevent circular includes
class Ghost;
class Normal;

class Agent
{
public:
	Agent();
	~Agent();

	virtual void update(Level* level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts);
	virtual void draw(Palico::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* other);

	void setPosition(glm::vec2 position)
	{
		this->position = position;
	}
	void setPosition(float x, float y) { position.x = x; position.y = y; }
	glm::vec2 getPosition() { return position; }

	void setDirection(glm::vec2 direction)
	{
		this->direction = direction;
	}

	void setTexture(std::string textureName)
	{
		texture = Palico::ResourceManager::getTexture(textureName);
	}

	void setSpeed(float speed)
	{
		this->speed = speed;
	}


protected:

	glm::vec2 position;
	glm::vec2 direction;

	float speed;
	Palico::GLTexture texture;

	virtual bool handleCollision(Level* level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts);

};


#include "Agent.h"

#include <PalicoEngine\ResourceManager.h>

#include <algorithm>
#include <math.h>


Agent::Agent() : position(0, 0), direction(0, 0), speed(0)
{
}

Agent::~Agent()
{
}

void Agent::update(float deltaTime, Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts)
{
	position += direction * speed * deltaTime;

	handleLevelCollision(level);
}

void Agent::draw(Palico::SpriteBatch& spriteBatch)
{
	glm::vec4 pos(position.x, position.y, AGENT_WIDTH, AGENT_WIDTH);
	glm::vec4 uv(0, 0, 1, 1);
	Palico::Color color(255, 255, 255, 255);

	spriteBatch.draw(pos, uv, texture.id, 1, color);
}

bool Agent::collideWithAgent(Agent* other)
{
	// Is the Agent too far away in the X direction to check?
	if (other->position.x < position.x - AGENT_WIDTH)      
		return false;
	else if (other->position.x > position.x + AGENT_WIDTH) 
		return false;

	// Is the Agent too far away in the Y direction to check?
	if (other->position.y < position.y - AGENT_WIDTH)      
		return false;
	else if (other->position.y > position.y + AGENT_WIDTH) 
		return false;

	// collision
	const float MIN_DISTANCE = AGENT_WIDTH; 

	glm::vec2 centerPosA = position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = other->position + glm::vec2(AGENT_RADIUS);

	glm::vec2 distanceVector = centerPosA - centerPosB;
	float distance = glm::length(distanceVector);

	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVector;
		if (glm::length(distanceVector) == 0)
		{
			collisionDepthVector = glm::vec2(1,0) * collisionDepth;
		}
		else
		{
			collisionDepthVector = glm::normalize(distanceVector) * collisionDepth; // glm::normalize(0) = NaN
		}
		
		position += collisionDepthVector / 2.0f;
		other->position -= collisionDepthVector / 2.0f;
		return true;
	}

	return false;
}


bool Agent::handleLevelCollision(Level* level)
{
	std::vector<glm::vec2> tiles = level->getCollidingTiles(position, AGENT_WIDTH);
	if (tiles.size() <= 0)
	{
		return false;
	}

	for (size_t i = 0; i < tiles.size(); i++)
	{
		position = level->checkTileCollision(tiles[i], position, AGENT_RADIUS);
	}
	return true;
}



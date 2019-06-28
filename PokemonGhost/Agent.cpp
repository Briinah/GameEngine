#include "Agent.h"

#include <PalicoEngine\ResourceManager.h>

#include <algorithm>
const float AGENT_WIDTH = 64.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

Agent::Agent() : position(0, 0), direction(0, 0), speed(0)
{
}

Agent::~Agent()
{
}

void Agent::update(Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts)
{
	position += direction * speed;
	handleLevelCollision(level);
}

void Agent::draw(Palico::SpriteBatch& spriteBatch)
{
	glm::vec4 pos(position.x, position.y, AGENT_WIDTH, AGENT_WIDTH);
	glm::vec4 uv(0, 0, 1, 1);
	Palico::Color color(255,255,255,255);

	spriteBatch.draw(pos, uv, texture.id, 1, color);
}

bool Agent::collideWithAgent(Agent * other)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	glm::vec2 centerPosA = position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = other->position + glm::vec2(AGENT_RADIUS);

	glm::vec2 distanceVector = centerPosA - centerPosB;
	float distance = glm::length(distanceVector);

	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVector = glm::normalize(distanceVector) * collisionDepth;
		position += collisionDepthVector / 2.0f;
		other->position -= collisionDepthVector / 2.0f;
		return true;
	}

	return false;
}



bool Agent::handleLevelCollision(Level* level)
{
	std::vector<glm::vec2> tiles = level->getCollidingTiles(position, AGENT_WIDTH);

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DIST = AGENT_RADIUS + TILE_RADIUS;

	if (tiles.size() <= 0)
	{
		return false;
	}

	for (int i = 0; i < tiles.size(); i++)
	{
		// AABB
		glm::vec2 centerPlayer = position + glm::vec2(AGENT_RADIUS);
		glm::vec2 distance = centerPlayer - tiles[i];

		float xDepth = MIN_DIST - abs(distance.x);
		float yDepth = MIN_DIST - abs(distance.y);

		if (xDepth > 0 || yDepth > 0)
		{
			// colliding
			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
			{
				if (distance.x < 0)
					position.x -= xDepth;
				else
					position.x += xDepth;
			}
			else
			{
				if (distance.y < 0)
					position.y -= yDepth;
				else
					position.y += yDepth;
			}
		}

		return true;
	}
}


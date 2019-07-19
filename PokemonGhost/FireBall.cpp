#include "FireBall.h"
#include <PalicoEngine\ResourceManager.h>

FireBall::FireBall(glm::vec2 position, glm::vec2 direction, float speed, float lifeTime) :
	position(position),
	direction(direction),
	speed(speed),
	lifeTime(lifeTime)
{
	texture = Palico::ResourceManager::getTexture("fireball");
}

FireBall::~FireBall()
{
}

void FireBall::draw(Palico::SpriteBatch & spriteBatch)
{
	glm::vec4 uv(0, 0, 1, 1);
	Palico::Color color(255, 255, 255, 255);
	glm::vec4 posSize = glm::vec4(position.x, position.y, FIRE_WIDTH, FIRE_WIDTH);

	spriteBatch.draw(posSize, uv, texture.id, 0, color);
}

bool FireBall::update(float deltaTime, std::vector<Ghost*> ghosts, Level* level)
{
	position += direction * speed * deltaTime;
	lifeTime--;
	
	return (lifeTime == 0) || collideWithGhosts(ghosts) || collideWithLevel(level);

}

bool FireBall::collideWithGhosts(std::vector<Ghost*> ghosts)
{
	for (size_t i = 0; i < ghosts.size(); i++)
	{
		const float MIN_DISTANCE = FIRE_RADIUS + AGENT_RADIUS;

		glm::vec2 centerPosA = position + glm::vec2(FIRE_RADIUS);
		glm::vec2 centerPosB = ghosts[i]->getPosition() + glm::vec2(AGENT_RADIUS);

		glm::vec2 distanceVector = centerPosA - centerPosB;
		float distance = glm::length(distanceVector);

		float collisionDepth = MIN_DISTANCE - distance;
		if (collisionDepth > 0)
		{
			// hit!
			ghosts[i]->removeHealth(5.0f);
			return true;
		}
	}

	return false;
}

bool FireBall::collideWithLevel(Level* level)
{
	std::vector<glm::vec2> tiles = level->getCollidingTiles(position, FIRE_RADIUS);

	if (tiles.size() <= 0)
	{
		return false;
	}

	for (size_t i = 0; i < tiles.size(); i++)
	{
		position = level->checkTileCollision(tiles[i], position, FIRE_RADIUS);
		return true;
	}

	return false;
}

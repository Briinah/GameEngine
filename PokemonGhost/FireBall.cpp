#include "FireBall.h"
#include <PalicoEngine\ResourceManager.h>

const float FIRE_RADIUS = 30;
const float AGENT_RADIUS = 64;

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
	glm::vec4 posSize = glm::vec4(position.x, position.y, FIRE_RADIUS, FIRE_RADIUS);

	spriteBatch.draw(posSize, uv, texture.id, 0, color);
}

bool FireBall::update(std::vector<Ghost*> ghosts)
{
	position += direction * speed;
	lifeTime--;
	
	return (lifeTime == 0) || collideWithGhosts(ghosts);

}

bool FireBall::collideWithGhosts(std::vector<Ghost*> ghosts)
{
	for (int i = 0; i < ghosts.size(); i++)
	{
		const float MIN_DISTANCE = FIRE_RADIUS * AGENT_RADIUS;

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

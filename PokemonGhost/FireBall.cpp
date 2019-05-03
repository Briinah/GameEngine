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
	glm::vec4 posSize = glm::vec4(position.x, position.y, 30, 30);

	spriteBatch.draw(posSize, uv, texture.id, 0, color);
}

bool FireBall::update()
{
	position += direction * speed;
	lifeTime--;

	return (lifeTime == 0);

}
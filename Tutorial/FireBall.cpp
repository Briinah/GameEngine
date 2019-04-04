#include "FireBall.h"
#include <PalicoEngine\ResourceManager.h>

FireBall::FireBall(glm::vec2 position, glm::vec2 direction, float speed, float lifeTime) :
	position(position),
	direction(direction),
	speed(speed),
	lifeTime(lifeTime)
{
}

FireBall::~FireBall()
{
}

void FireBall::draw(Palico::SpriteBatch & spriteBatch)
{
	glm::vec4 uv(0, 0, 1, 1);
	Palico::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	static Palico::GLTexture texture = Palico::ResourceManager::getTexture("Textures/fireball.png");
	glm::vec4 posSize = glm::vec4(position.x, position.y, 30, 30);

	spriteBatch.draw(posSize, uv, texture.id, 0, color);
}

bool FireBall::update()
{
	position += direction * speed;
	lifeTime--;

	return (lifeTime == 0);

}
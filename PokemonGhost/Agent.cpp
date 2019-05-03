#include "Agent.h"

#include <PalicoEngine\ResourceManager.h>

Palico::GLTexture Agent::texture;

Agent::Agent() : position(0, 0), direction(0, 0), speed(0)
{
}

Agent::~Agent()
{
}

void Agent::update()
{
	position += direction * speed;
}

void Agent::draw(Palico::SpriteBatch& spriteBatch)
{
	glm::vec4 pos(position.x, position.y, 100, 100);
	glm::vec4 uv(0, 0, 1, 1);
	Palico::Color color(255,255,255,255);

	spriteBatch.draw(pos, uv, texture.id, 1, color);
}

void Agent::onCollision()
{
}

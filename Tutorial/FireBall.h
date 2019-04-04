#pragma once

#include <glm\glm.hpp>
#include <PalicoEngine\SpriteBatch.h>
#include <PalicoEngine\GLTexture.h>

class FireBall
{
public:
	FireBall(glm::vec2 position, glm::vec2 direction, float speed, float lifeTime);
	~FireBall();

	void draw(Palico::SpriteBatch& spriteBatch);
	bool update();

private:
	glm::vec2 position;
	glm::vec2 direction;
	float speed;
	float lifeTime;
};

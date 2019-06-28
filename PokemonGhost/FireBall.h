#pragma once

#include <glm\glm.hpp>
#include <PalicoEngine\SpriteBatch.h>
#include <PalicoEngine\GLTexture.h>

#include "Ghost.h"

class FireBall
{
public:
	FireBall(glm::vec2 position, glm::vec2 direction, float speed, float lifeTime);
	~FireBall();

	void draw(Palico::SpriteBatch& spriteBatch);
	bool update(std::vector<Ghost*> ghosts);

private:
	glm::vec2 position;
	glm::vec2 direction;
	float speed;
	float lifeTime;

	Palico::GLTexture texture;

	bool collideWithGhosts(std::vector<Ghost*> ghosts);
};

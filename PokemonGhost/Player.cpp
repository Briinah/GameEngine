#include "Player.h"



Player::Player() : position(0)
{
}


Player::~Player()
{
}

void Player::update()
{
	Normal::update();

	for (int i = 0; i < fireBalls.size();)
	{
		if (fireBalls[i].update())
		{
			fireBalls[i] = fireBalls.back();
			fireBalls.pop_back();
		}
		else
		{
			i++;
		}
	}
}

void Player::draw(Palico::SpriteBatch& spriteBatch)
{
	Normal::draw(spriteBatch);

	for (int i = 0; i < fireBalls.size(); ++i)
	{
		fireBalls[i].draw(spriteBatch);
	}
}

void Player::addFireBall(glm::vec2 mousePosition)
{
	glm::vec2 direction = mousePosition - position;
	direction = glm::normalize(direction);

	fireBalls.emplace_back(position, direction, 8, 30);
}

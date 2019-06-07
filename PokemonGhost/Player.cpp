#include "Player.h"


#include <SDL\SDL.h>

Player::Player(int speed, glm::vec2 position, std::string texture) : Normal(speed, position, texture)
{
}


Player::~Player()
{
}

void Player::update(Level* level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts)
{
	handleCollision(level, normals, ghosts);
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

void Player::processInput(Palico::InputManager& inputManager)
{
	if (inputManager.isKeyDown(SDLK_w))
	{
		position.y += speed;
	}
	if (inputManager.isKeyDown(SDLK_s))
	{
		position.y -= speed;
	}
	if (inputManager.isKeyDown(SDLK_a))
	{
		position.x -= speed;
	}
	if (inputManager.isKeyDown(SDLK_d))
	{
		position.x += speed;
	}
}

void Player::addFireBall(glm::vec2 mousePosition)
{
	glm::vec2 direction = mousePosition - position;
	direction = glm::normalize(direction);

	fireBalls.emplace_back(position, direction, 8, 30);
}

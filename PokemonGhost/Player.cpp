#include "Player.h"


#include <SDL\SDL.h>
#include <iostream>

Player::Player(int speed, glm::vec2 position, std::string texture) : Friendly(speed, position, texture)
{
}


Player::~Player()
{
}

void Player::update(float deltaTime, Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts)
{
	handleLevelCollision(level);
	for (int i = 0; i < fireBalls.size();)
	{
		if (fireBalls[i].update(deltaTime, ghosts, level))
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
	Friendly::draw(spriteBatch);

	for (int i = 0; i < fireBalls.size(); ++i)
	{
		fireBalls[i].draw(spriteBatch);
	}
}

void Player::processInput(Palico::InputManager& inputManager, float deltaTime)
{
	if (inputManager.isKeyDown(SDLK_w))
	{
		position.y += speed * deltaTime;
	}
	if (inputManager.isKeyDown(SDLK_s))
	{
		position.y -= speed * deltaTime;
	}
	if (inputManager.isKeyDown(SDLK_a))
	{
		position.x -= speed * deltaTime;
	}
	if (inputManager.isKeyDown(SDLK_d))
	{
		position.x += speed * deltaTime;
	}
}

void Player::addFireBall(glm::vec2 mousePosition)
{
	glm::vec2 direction = mousePosition - position;
	direction = glm::normalize(direction);

	fireBalls.emplace_back(position, direction, 8, 30);
}

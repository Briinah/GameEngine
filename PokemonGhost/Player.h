#pragma once

#include <PalicoEngine/SpriteBatch.h>
#include <PalicoEngine/InputManager.h>
#include "Normal.h"
#include "FireBall.h"

class Player : public Normal
{
public:
	Player(int speed, std::string texture);
	~Player();

	void update() override;
	void draw(Palico::SpriteBatch& spriteBatch) override;
	void processInput(Palico::InputManager& inputManager);
	void addFireBall(glm::vec2 mousePosition);

	glm::vec2 getPosition() { return position; }
	void setPosition(glm::vec2 position) { this->position = position; }
	void setPosition(float x, float y) { position.x = x; position.y = y; }

private:

	std::vector<FireBall> fireBalls;
};


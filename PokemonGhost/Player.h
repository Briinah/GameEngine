#pragma once

#include <PalicoEngine/SpriteBatch.h>
#include <PalicoEngine/InputManager.h>
#include "Friendly.h"
#include "FireBall.h"

class Player : public Friendly
{
public:
	Player(int speed, glm::vec2 position, std::string texture);
	~Player();

	void update(float deltaTime, Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts) override;
	void draw(Palico::SpriteBatch& spriteBatch) override;
	void processInput(Palico::InputManager& inputManager, float deltaTime);
	void addFireBall(glm::vec2 mousePosition);

	glm::vec2 getPosition() { return position; }

private:

	std::vector<FireBall> fireBalls;
};


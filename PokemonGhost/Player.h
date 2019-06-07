#pragma once

#include <PalicoEngine/SpriteBatch.h>
#include <PalicoEngine/InputManager.h>
#include "Normal.h"
#include "FireBall.h"

class Player : public Normal
{
public:
	Player(int speed, glm::vec2 position, std::string texture);
	~Player();

	void update(Level* level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts) override;
	void draw(Palico::SpriteBatch& spriteBatch) override;
	void processInput(Palico::InputManager& inputManager);
	void addFireBall(glm::vec2 mousePosition);

	glm::vec2 getPosition() { return position; }

private:

	std::vector<FireBall> fireBalls;
};


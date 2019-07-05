#pragma once

#include "Agent.h"

class Friendly : public Agent
{
public:
	Friendly(float speed, glm::vec2 postition, std::string texture);
	~Friendly();

	void update(float deltaTime, Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts) override;
	void draw(Palico::SpriteBatch& spriteBatch) override;

private:

	float frames;
};


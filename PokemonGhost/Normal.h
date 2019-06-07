#pragma once

#include "Agent.h"

class Normal : public Agent
{
public:
	Normal(int speed, glm::vec2 postition, std::string texture);
	~Normal();

	void update(Level* level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts) override;
	void draw(Palico::SpriteBatch& spriteBatch) override;
};


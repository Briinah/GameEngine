#pragma once

#include "Agent.h"

class Ghost : public Agent
{
public:
	Ghost(float speed, glm::vec2 postition, std::string texture);
	~Ghost();

	void draw(Palico::SpriteBatch& spriteBatch) override;
	void update(Level* level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts) override;

private:

	float frames;
};


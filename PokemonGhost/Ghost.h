#pragma once

#include "Agent.h"

class Ghost : public Agent
{
public:
	Ghost(float speed, glm::vec2 postition, std::string texture);
	~Ghost();

	void draw(Palico::SpriteBatch& spriteBatch) override;
	void update(float deltaTime, Level* level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts) override;
	void removeHealth(float amount);

	bool isAlive() 
	{ 
		return health > 0;
	}

private:

	float frames;
	float health;

	Friendly* getNearestFriendly(std::vector<Friendly*> friendlies);
};


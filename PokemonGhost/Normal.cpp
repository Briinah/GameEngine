#include "Normal.h"

#include <random>
#include <ctime>

Normal::Normal(int speed, glm::vec2 position, std::string texture) 
{
	this->speed = speed;
	this->position = position;
	setTexture(texture);

	// only want to seed the engine once
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	setDirection(glm::normalize(glm::vec2(distribution(randomEngine), distribution(randomEngine))));
}

Normal::~Normal()
{
}

void Normal::update(Level * level, std::vector<Normal*> normals, std::vector<Ghost*> ghosts)
{
	Agent::update(level, normals, ghosts);

	// move random
}

void Normal::draw(Palico::SpriteBatch & spriteBatch)
{
	Agent::draw(spriteBatch);
}

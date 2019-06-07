#include "Normal.h"

#include <random>
#include <ctime>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\rotate_vector.hpp>

Normal::Normal(int speed, glm::vec2 position, std::string texture) : frames(0)
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
	position += direction * speed;

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f * M_PI / 180, 40.0f * M_PI / 180);

	if (frames == 20)
	{
		direction = glm::rotate(direction, randRotate(randomEngine));
		frames = 0;
	}
	else
	{
		frames++;
	}

	if (handleCollision(level, normals, ghosts))
	{
		direction = glm::rotate(direction, randRotate(randomEngine));
	}
}

void Normal::draw(Palico::SpriteBatch & spriteBatch)
{
	Agent::draw(spriteBatch);
}

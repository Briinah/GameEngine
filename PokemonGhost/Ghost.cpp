#include "Ghost.h"

#include <random>
#include <ctime>
#include <limits>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\rotate_vector.hpp>

#include "Friendly.h"

Ghost::Ghost(float speed, glm::vec2 position, std::string texture) : frames(0), health(10)
{
	this->speed = speed;
	this->position = position;
	setTexture(texture);

	// only want to seed the engine once
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	setDirection(glm::normalize(glm::vec2(distribution(randomEngine), distribution(randomEngine))));
}

Ghost::~Ghost()
{
}

void Ghost::draw(Palico::SpriteBatch & spriteBatch)
{
	Agent::draw(spriteBatch);
}

void Ghost::update(float deltaTime, Level * level, std::vector<Friendly*> friendlies, std::vector<Ghost*> ghosts)
{
	position += direction * speed * deltaTime;

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

	Friendly* target = getNearestFriendly(friendlies);
	if (target != nullptr)
	{
		direction = glm::normalize(target->getPosition() - position);
	}

	handleLevelCollision(level);
}

void Ghost::removeHealth(float amount)
{
	health -= amount;
}

Friendly* Ghost::getNearestFriendly(std::vector<Friendly*> friendlies)
{
	Friendly* closestFriendly = nullptr;
	float smallestDistance = std::numeric_limits<float>::max();

	for (int i = 0; i < friendlies.size(); i++)
	{
		glm::vec2 distanceVec = friendlies[i]->getPosition() - position;
		float distance = glm::length(distanceVec);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestFriendly = friendlies[i];
		}
	}

	return closestFriendly;
}

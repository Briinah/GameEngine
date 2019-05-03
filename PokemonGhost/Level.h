#pragma once

#include <string>
#include <vector>

#include <PalicoEngine/SpriteBatch.h>

class Level
{
public:
	Level(const std::string& filePath);
	~Level();

	void draw();

	glm::ivec2 getPlayerStartPosition() { return playerStartPosition; }

private:

	std::vector<std::string> levelData;

	int numMundane;

	Palico::SpriteBatch spriteBatch;
	//Palico::ResourceManager* resourceManager; // change to not static

	glm::ivec2 playerStartPosition;
	std::vector<glm::ivec2> ghostStartPositions;
};


#pragma once

#include <string>
#include <vector>

#include <PalicoEngine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string& filePath);
	~Level();

	void draw();

	glm::ivec2 getPlayerStartPosition() { return playerStartPosition; }

	int getNumNormals() { return numNormal; }
	int getWidth() const { return levelData[1].size(); }
	int getHeight() const { return levelData.size(); }

	std::vector<glm::vec2> getCollidingTiles(glm::vec2 position, int agentWidth);

private:

	std::vector<std::string> levelData;

	int numNormal;

	Palico::SpriteBatch spriteBatch;
	//Palico::ResourceManager* resourceManager; // change to not static

	glm::vec2 playerStartPosition;
	std::vector<glm::ivec2> ghostStartPositions;

	void addTile(std::vector<glm::vec2>& tiles, int x, int y);
};


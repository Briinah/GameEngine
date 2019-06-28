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

	glm::vec2 getPlayerStartPosition() { return playerStartPosition; }

	int getNumFriendlies() { return numFriendlies; }
	int getWidth() const { return levelData[1].size(); }
	int getHeight() const { return levelData.size(); }

	std::vector<glm::vec2> getCollidingTiles(glm::vec2 position, int agentWidth);
	std::vector<glm::vec2> getGhostStartPositions() { return ghostStartPositions; }

private:

	std::vector<std::string> levelData;

	int numFriendlies;

	Palico::SpriteBatch spriteBatch;

	glm::vec2 playerStartPosition;
	std::vector<glm::vec2> ghostStartPositions;

	void addTile(std::vector<glm::vec2>& tiles, int x, int y);
};


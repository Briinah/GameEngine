#include "Level.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <PalicoEngine/Errors.h>
#include <PalicoEngine/ResourceManager.h>

Level::Level(const std::string & name)
{
	std::ifstream file;
	std::string filePath = "Levels/" + name + ".txt";
	file.open(filePath);

	if (file.fail())
	{
		Palico::fatalError("Failed to open " + filePath);
	}

	std::string tmp;
	file >> tmp >> numFriendlies;

	while (std::getline(file, tmp))
	{
		levelData.push_back(tmp);
	}

	spriteBatch.init();
	spriteBatch.begin();
	glm::vec4 uvRect(0.f, 0.f, 1.f, 1.f);
	
	for (size_t y = 0; y < levelData.size(); y++)
	{
		for (size_t x = 0; x < levelData[y].size(); x++)
		{
			char tile = levelData[y][x];
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile)
			{
			case '.':
				break;
			case 'L':
				spriteBatch.draw(destRect, uvRect,
					Palico::ResourceManager::getTexture("lightbrick").id,
					0.f,
					Palico::Color(255, 255, 255, 255));
				break;
			case 'R':
				spriteBatch.draw(destRect, uvRect, 
					Palico::ResourceManager::getTexture("redbrick").id, 
					0.f, 
					Palico::Color(255, 255, 255, 255));
					break;
			case 'G':
				spriteBatch.draw(destRect, uvRect,
					Palico::ResourceManager::getTexture("glass").id,
					0.f,
					Palico::Color(255, 255, 255, 255));
				break;
			case 'S':
				spriteBatch.draw(destRect, uvRect,
					Palico::ResourceManager::getTexture("rock").id,
					0.f,
					Palico::Color(255, 255, 255, 255));
				break;
			case '@':
				playerStartPosition.x = x * TILE_WIDTH;
				playerStartPosition.y = y * TILE_WIDTH;
				levelData[y][x] = '.';
				break;
			case 'Z':
				ghostStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				levelData[y][x] = '.';
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				levelData[y][x] = '.';
				break;
			}
		}
	}

	spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw()
{
	spriteBatch.renderBatch();
}

glm::vec2 Level::checkTileCollision(glm::vec2 tile, glm::vec2 currentPosition, float radius)
{
	const float MIN_DIST = radius + TILE_RADIUS;

	glm::vec2 newPosition = currentPosition;	
	
	// AABB
	glm::vec2 centerPlayer = currentPosition + radius;
	glm::vec2 distance = centerPlayer - tile;

	float xDepth = MIN_DIST - abs(distance.x);
	float yDepth = MIN_DIST - abs(distance.y);

	if (xDepth > 0 && yDepth > 0)
	{
		// colliding
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			if (distance.x < 0)
				newPosition.x -= xDepth;
			else
				newPosition.x += xDepth;
		}
		else
		{
			if (distance.y < 0)
				newPosition.y -= yDepth;
			else
				newPosition.y += yDepth;
		}
	}

	return newPosition;
}

void Level::addTile(std::vector<glm::vec2>& tiles, float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(
		floor(x / TILE_WIDTH),
		floor(y / TILE_WIDTH));

	// collision out of bounds
	if (cornerPos.x < 0 || cornerPos.x >= levelData[1].size() ||
		cornerPos.y < 1 || cornerPos.y >= levelData.size())
	{
		std::cout << "Level::addTile -> collision out of bounds! x: " << cornerPos.x << " " << cornerPos.y << std::endl;
		return;
	}
	
	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		tiles.push_back(cornerPos * TILE_WIDTH + glm::vec2(TILE_WIDTH / 2.f));
	}
}

std::vector<glm::vec2> Level::getCollidingTiles(glm::vec2 position, float agentWidth)
{
	std::vector<glm::vec2> tiles;
	addTile(tiles, position.x, position.y);
	addTile(tiles, position.x + agentWidth, position.y);
	addTile(tiles, position.x, position.y + agentWidth);
	addTile(tiles, position.x + agentWidth, position.y + agentWidth);

	return tiles;
}



#include "Level.h"

#include <iostream>
#include <fstream>
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

	int yMax = levelData.size();

	for (int y = 0; y < levelData.size(); y++)
	{
		for (int x = 0; x < levelData[y].size(); x++)
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

void Level::addTile(std::vector<glm::vec2>& tiles, int x, int y)
{
	glm::vec2 cornerPos = glm::vec2(
		floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	if (cornerPos.x < 0 || cornerPos.y < 0)
		return;
	
	if (levelData[cornerPos.y][cornerPos.x] != '.') // todo: fix out of range problem
	{
		tiles.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.f));
	}
}

std::vector<glm::vec2> Level::getCollidingTiles(glm::vec2 position, int agentWidth)
{
	std::vector<glm::vec2> tiles;
	addTile(tiles, position.x, position.y);
	addTile(tiles, position.x + agentWidth, position.y);
	addTile(tiles, position.x, position.y + agentWidth);
	addTile(tiles, position.x + agentWidth, position.y + agentWidth);

	return tiles;
}

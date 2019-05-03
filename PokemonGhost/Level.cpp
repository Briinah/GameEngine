#include "Level.h"

#include <iostream>
#include <fstream>
#include <PalicoEngine/Errors.h>
#include <PalicoEngine/ResourceManager.h>

const int TILE_WIDTH = 32;

Level::Level(const std::string & filePath)
{
	std::ifstream file;
	file.open(filePath);

	if (file.fail())
	{
		Palico::fatalError("Failed to open " + filePath);
	}

	std::string tmp;
	file >> tmp >> numMundane;

	while (std::getline(file, tmp))
	{
		levelData.push_back(tmp);
	}

	spriteBatch.begin();
	glm::vec4 uvRect(0.f, 0.f, 1.f, 1.f);

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
					Palico::ResourceManager::getTexture("Textures/lightbrick.png").id,
					0.f,
					Palico::Color(255, 255, 255, 255));
				break;
			case 'R':
				spriteBatch.draw(destRect, uvRect, 
					Palico::ResourceManager::getTexture("Textures/redbrick.png").id, 
					0.f, 
					Palico::Color(255, 255, 255, 255));
					break;
			case 'G':
				spriteBatch.draw(destRect, uvRect,
					Palico::ResourceManager::getTexture("Textures/glass.png").id,
					0.f,
					Palico::Color(255, 255, 255, 255));
				break;
			case 'S':
				spriteBatch.draw(destRect, uvRect,
					Palico::ResourceManager::getTexture("Textures/rock.png").id,
					0.f,
					Palico::Color(255, 255, 255, 255));
				break;
			case '@':
				playerStartPosition.x = x * TILE_WIDTH;
				playerStartPosition.y = y * TILE_WIDTH;
				break;
			case 'Z':
				ghostStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
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

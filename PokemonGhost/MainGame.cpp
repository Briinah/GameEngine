#include "MainGame.h"

#include <iostream>
#include <string>
#include <random>
#include <ctime>

#include <PalicoEngine\ResourceManager.h>


MainGame::MainGame() :
	screenWidth(1024),
	screenHeight(768),
	gameState(GameState::PLAY),
	camera(screenWidth, screenHeight),
	fpsLimiter(60),
	player(nullptr)
{
}


MainGame::~MainGame()
{
	for (int i = 0; i < levels.size(); ++i)
	{
		delete levels[i];
	}

	delete player;
}


void MainGame::run()
{
	initSystems();
	loadLevels();
	player = new Player(4, glm::vec2(0), "charmander");
	friendlies.push_back(player);
	setCurrentLevel(0);

	gameLoop();
}

void MainGame::loadLevels()
{
	levels.push_back(new Level("0"));
}

void MainGame::setCurrentLevel(int level)
{
	currentLevel = level;
	player->setPosition(levels[currentLevel]->getPlayerStartPosition());

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randomPosX(2, levels[currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randomPosY(2, levels[currentLevel]->getHeight() - 2);

	// generate friendly creatures
	for (int i = 0; i < levels[currentLevel]->getNumFriendlies(); i++)
	{
		glm::vec2 position(randomPosX(randomEngine) * TILE_WIDTH, randomPosY(randomEngine) * TILE_WIDTH);

		friendlies.push_back(new Friendly(1.0f, position, "clefairy"));
	}

	// generate ghosts
	const std::vector<glm::vec2>& ghostPositions = levels[currentLevel]->getGhostStartPositions();	
	for (int i = 0; i < ghostPositions.size(); i++)
	{
		ghosts.push_back(new Ghost(1.3f, ghostPositions[i], "gengar"));
	}
}

void MainGame::initSystems()
{
	Palico::init();
	window.create("Pokemon Ghost", screenWidth, screenHeight, 0);
	glClearColor(0.5f, 0.2f, 0.2f, 1.f);

	camera.setScale(1.5f);

	initShaders();
	spriteBatch.init();
}

void MainGame::initShaders()
{
	colorProgram.compileShaders("Shaders/colorShading.vert", "shaders/colorShading.frag");
	colorProgram.addAttribute("vertexPosition");
	colorProgram.addAttribute("vertexColor");
	colorProgram.addAttribute("vertexUV");
	colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (gameState != GameState::EXIT)
	{
		fpsLimiter.beginFrame();

		update();
		processInput();
		draw();

		fps = fpsLimiter.end();
		printFps();
	}
}

void MainGame::printFps()
{
	// print once per 1000 frames
	static int frameCount = 0;
	frameCount++;
	if (frameCount == 600)
	{
		std::cout << fps << std::endl;
		frameCount = 0;
	}
}

void MainGame::processInput()
{
	const float SCALE_SPEED = 0.2f;

	player->processInput(inputManager);
	camera.setPosition(player->getPosition());

	if (inputManager.isMouseScrolling() < 0)
	{
		camera.setScale(camera.getScale() + SCALE_SPEED);
	}
	if (inputManager.isMouseScrolling() > 0)
	{
		camera.setScale(camera.getScale() - SCALE_SPEED);
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mousePos = camera.getWorldPosition(inputManager.getMousePosition());
		player->addFireBall(mousePos);
	}

	if (inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		std::cout << "exit game" << std::endl;
		gameState = GameState::EXIT;
	}
}

void MainGame::update()
{
	inputManager.update();
	camera.update();
	updateAgents();
}

void MainGame::updateAgents()
{
	for (int i = 0; i < friendlies.size(); ++i)
	{
		friendlies[i]->update(levels[currentLevel], friendlies, ghosts);
	}
	for (int i = 0; i < ghosts.size(); ++i)
	{
		ghosts[i]->update(levels[currentLevel], friendlies, ghosts);
	}

	for (int i = 0; i < friendlies.size(); ++i)
	{
		for (int j = i + 1; j < friendlies.size(); j++)
		{
			friendlies[i]->collideWithAgent(friendlies[j]);
		}
	}

	for (int i = 0; i < ghosts.size(); ++i)
	{
		for (int j = i + 1; j < ghosts.size(); j++)
		{
			ghosts[i]->collideWithAgent(ghosts[j]);
		}

		// skip the player
		for (int j = 1; j < friendlies.size(); j++)
		{
			if (ghosts[i]->collideWithAgent(friendlies[j]))
			{ 
				changeFriendlyToGhost(j);
			}
		}

		// todo: check collision with player


		if (!ghosts[i]->isAlive())
		{
			delete ghosts[i];
			ghosts[i] = ghosts.back();
			ghosts.pop_back();
		}
	}
}

void MainGame::changeFriendlyToGhost(int j)
{
	ghosts.push_back(new Ghost(1.3f, friendlies[j]->getPosition(), "gengar"));

	delete friendlies[j];
	friendlies[j] = friendlies.back();
	friendlies.pop_back();
}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader
	colorProgram.use();

	// set texture
	glActiveTexture(GL_TEXTURE0);

	// set shader variables
	GLint textureLocation = colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	// set camera projection matrix
	GLint pLocation = colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	spriteBatch.begin();

	for (int i = 0; i < friendlies.size(); ++i)
	{
		friendlies[i]->draw(spriteBatch);
	}
	for (int i = 0; i < ghosts.size(); ++i)
	{
		ghosts[i]->draw(spriteBatch);
	}

	levels[currentLevel]->draw();

	spriteBatch.end();
	spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// stop using shader
	colorProgram.unuse();

	window.swapBuffer();
}



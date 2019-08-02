#include "MainGame.h"

#include <iostream>
#include <string>
#include <random>
#include <ctime>

#include <PalicoEngine\ResourceManager.h>

#pragma float_control( except, on )


MainGame::MainGame() :
	camera(screenWidth, screenHeight),
	hudCamera(screenWidth, screenHeight),
	fpsLimiter(60),
	gameTime(&fpsLimiter)
{
}


MainGame::~MainGame()
{
	for (size_t i = 0; i < levels.size(); ++i)
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
	hud = new HUD(&friendlies, &ghosts, screenWidth, screenHeight);

	gameLoop();
}

void MainGame::loadLevels()
{
	levels.push_back(new Level("2"));
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
	for (size_t i = 0; i < ghostPositions.size(); i++)
	{
		ghosts.push_back(new Ghost(1.3f, ghostPositions[i], "gengar"));
	}
}

void MainGame::initSystems()
{
	Palico::init();
	window.create("Pokemon Ghost", screenWidth, screenHeight, 0);
	glClearColor(0.5f, 0.2f, 0.2f, 1.f);

	camera.setScaleDimensions(0.1f, 3);
	camera.setScale(1.5f);
	hudCamera.setPosition(glm::vec2(screenWidth / 2, screenHeight / 2));

	initShaders();
	spriteBatch.init();
	uiSpriteBatch.init();

	spriteFont = new Palico::SpriteFont("Fonts/Roboto-Regular.ttf", 32);
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

void MainGame::processInput(float deltaTime)
{
	const float SCALE_SPEED = 0.2f;

	//player->processInput(inputManager, deltaTime);
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

	if (inputManager.isKeyPressed(SDLK_ESCAPE) || inputManager.quit)
	{
		std::cout << "exit game" << std::endl;
		gameState = GameState::EXIT;
	}
}

void MainGame::update()
{
	const float MAX_PHYSICS_STEPS = 5;
	float totalDeltaTime = gameTime.getTotalDeltaTime();
	int i = 0;
	while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
	{
		float deltaTime = std::min(totalDeltaTime, gameTime.MAX_DELTA_TIME);
		player->processInput(inputManager, deltaTime); 

		updateAgents(deltaTime); 

		i++;
		totalDeltaTime -= deltaTime;
	}
	processInput(1);
	inputManager.update();
	camera.update();
	hudCamera.update();
}

void MainGame::updateAgents(float deltaTime)
{
	for (size_t i = 0; i < friendlies.size(); ++i)
	{
		friendlies[i]->update(deltaTime, levels[currentLevel], friendlies, ghosts);
	}
	for (size_t i = 0; i < ghosts.size(); ++i)
	{
		ghosts[i]->update(deltaTime, levels[currentLevel], friendlies, ghosts);
	}

	for (size_t i = 0; i < friendlies.size(); ++i)
	{
		for (size_t j = i + 1; j < friendlies.size(); j++)
		{
			friendlies[i]->collideWithAgent(friendlies[j]); 
		}
	}
	
	for (size_t i = 0; i < ghosts.size(); ++i)
	{
		for (size_t j = i + 1; j < ghosts.size(); j++)
		{
			ghosts[i]->collideWithAgent(ghosts[j]);
		}

		// skip the player
		for (size_t j = 1; j < friendlies.size(); j++)
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

	for (size_t i = 0; i < friendlies.size(); ++i)
	{
		if (camera.isInViewPort(friendlies[i]->getPosition(), glm::vec2(AGENT_WIDTH)))
		{
			friendlies[i]->draw(spriteBatch);
		}
	}
	for (size_t i = 0; i < ghosts.size(); ++i)
	{
		if (camera.isInViewPort(ghosts[i]->getPosition(), glm::vec2(AGENT_WIDTH)))
		{
			ghosts[i]->draw(spriteBatch);
		}
	}

	levels[currentLevel]->draw();


	spriteBatch.end();
	spriteBatch.renderBatch();

	// set hudcamera projection matrix
	glm::mat4 hudProjectionMatrix = hudCamera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(hudProjectionMatrix[0][0]));

	hud->draw(uiSpriteBatch, spriteFont);

	glBindTexture(GL_TEXTURE_2D, 0);

	// stop using shader
	colorProgram.unuse();

	window.swapBuffer();
}



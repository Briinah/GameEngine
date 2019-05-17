#include <iostream>
#include <string>
#include <PalicoEngine\ResourceManager.h>

#include "MainGame.h"

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
	player = new Player(4, "charmander");
	normals.push_back(player);
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

	if (inputManager.isKeyDown(SDLK_q))
	{
		camera.setScale(camera.getScale() + SCALE_SPEED);
	}
	if (inputManager.isKeyDown(SDLK_e))
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

	for (int i = 0; i < normals.size(); ++i)
	{
		normals[i]->update(levels[currentLevel], normals, ghosts);
	}
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

	for (int i = 0; i < normals.size(); ++i)
	{
		normals[i]->draw(spriteBatch);
	}

	levels[currentLevel]->draw();

	spriteBatch.end();
	spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// stop using shader
	colorProgram.unuse();

	window.swapBuffer();
}



#include <iostream>
#include <string>
#include <PalicoEngine\ResourceManager.h>

#include "MainGame.h"

MainGame::MainGame() :
	screenWidth(1024),
	screenHeight(768),
	time(0),
	gameState(GameState::PLAY),
	camera(screenWidth, screenHeight),
	fpsLimiter(60)
{
}


MainGame::~MainGame()
{
}


void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	Palico::init();
	window.create("Game Engine", screenWidth, screenHeight, 0);

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

		inputManager.update();
		processInput();
		time += 0.01;

		camera.update();
		for (int i = 0; i < fireBalls.size();)
		{
			if (fireBalls[i].update())
			{
				fireBalls[i] = fireBalls.back();
				fireBalls.pop_back();
			}
			else
			{
				i++;
			}
		}
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
	if (frameCount == 1000)
	{
		std::cout << fps << std::endl;
		frameCount = 0;
	}
}

void MainGame::processInput()
{
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.2f;

	if (inputManager.isKeyDown(SDLK_w))
	{
		camera.setPosition(camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (inputManager.isKeyDown(SDLK_s))
	{
		camera.setPosition(camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (inputManager.isKeyDown(SDLK_a))
	{
		camera.setPosition(camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0));
	}
	if (inputManager.isKeyDown(SDLK_d))
	{
		camera.setPosition(camera.getPosition() + glm::vec2(CAMERA_SPEED, 0));
	}
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
		std::cout << mousePos.x << " " << mousePos.y << std::endl;

		glm::vec2 playerPos(0);
		glm::vec2 direction = mousePos - playerPos;
		direction = glm::normalize(direction);

		fireBalls.emplace_back(playerPos, direction, 8, 30);
	}

	if (inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		std::cout << "exit game" << std::endl;
		gameState = GameState::EXIT;
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

	//GLint timeLocation = colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, time);

	// set camera projection matrix
	GLint pLocation = colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	spriteBatch.begin();

	glm::vec4 pos(0, 0, 100, 100);
	glm::vec4 uv(0, 0, 1, 1);
	static Palico::GLTexture tex = Palico::ResourceManager::getTexture("Textures/charmander.png");
	Palico::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(pos, uv, tex.id, 0, color);
	for (int i = 0; i < fireBalls.size(); ++i)
	{
		fireBalls[i].draw(spriteBatch);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	// stop using shader
	colorProgram.unuse();

	window.swapBuffer();
}



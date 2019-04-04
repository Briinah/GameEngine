#pragma once

#include <SDL\SDL.h>
#include <GL\glew.h>
#include <vector>

#include <PalicoEngine\PalicoEngine.h>
#include <PalicoEngine\GLSLProgram.h>
#include <PalicoEngine\GLTexture.h>
#include <PalicoEngine\Window.h>
#include <PalicoEngine\Camera2D.h>
#include <PalicoEngine\SpriteBatch.h>
#include <PalicoEngine\InputManager.h>
#include <PalicoEngine\Timing.h>

#include "FireBall.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();


private:

	void initSystems();
	void initShaders();
	void gameLoop();
	void printFps();
	void processInput();
	void draw();

	Palico::Window window;
	int screenWidth;
	int screenHeight;
	GameState gameState;

	Palico::GLSLProgram colorProgram;
	Palico::Camera2D camera;
	Palico::SpriteBatch spriteBatch;
	Palico::InputManager inputManager;
	Palico::FpsLimiter fpsLimiter;

	std::vector<FireBall> fireBalls;

	float fps;
	float time;
};


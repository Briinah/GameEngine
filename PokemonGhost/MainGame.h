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
#include <PalicoEngine\SpriteFont.h>

#include "Player.h"
#include "Friendly.h"
#include "Ghost.h"
#include "Level.h"
#include "HUD.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();


private:

	void loadLevels();
	void setCurrentLevel(int level);
	void initSystems();
	void initShaders();
	void gameLoop();
	void printFps();
	void processInput(float deltaTime);
	void update();
	void updateAgents(float deltaTime);
	void changeFriendlyToGhost(int j);
	void draw();

	Palico::Window window;
	int screenWidth;
	int screenHeight;
	GameState gameState;

	Palico::GLSLProgram colorProgram;
	Palico::Camera2D camera;
	Palico::SpriteBatch spriteBatch;
	Palico::SpriteBatch uiSpriteBatch;
	Palico::InputManager inputManager;
	Palico::FpsLimiter fpsLimiter;
	Palico::Time gameTime;
	Palico::SpriteFont* spriteFont;

	Player* player;
	HUD* hud;
	std::vector<Ghost*> ghosts;
	std::vector<Friendly*> friendlies;


	float fps;

	std::vector<Level*> levels;

	int currentLevel;
};


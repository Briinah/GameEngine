#pragma once

#include <PalicoEngine/SpriteBatch.h>
#include <PalicoEngine/SpriteFont.h>

class Friendly;
class Ghost;

class HUD
{
public:
	HUD(std::vector<Friendly*>* friendlies, std::vector<Ghost*>* ghosts, int screenWidth, int screenHeight);
	~HUD();

	void draw(Palico::SpriteBatch spriteBatch, Palico::SpriteFont* spriteFont);
	
private:

	std::vector<Friendly*>* friendlies;
	std::vector<Ghost*>* ghosts;

	int screenWidth;
	int screenHeight;
};


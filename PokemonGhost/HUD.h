#pragma once

#include <PalicoEngine/SpriteBatch.h>
#include <PalicoEngine/SpriteFont.h>

class Friendly;

class HUD
{
public:
	HUD(std::vector<Friendly*>* friendlies);
	~HUD();

	void draw(Palico::SpriteBatch spriteBatch, Palico::SpriteFont* spriteFont);
	
private:

	std::vector<Friendly*>* friendlies;
};


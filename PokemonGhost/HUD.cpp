#include "HUD.h"

HUD::HUD(std::vector<Friendly*>* friendlies, std::vector<Ghost*>* ghosts, int screenWidth, int screenHeight) : 
	friendlies(friendlies),
	ghosts(ghosts),
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
}

HUD::~HUD()
{
}

void HUD::draw(Palico::SpriteBatch spriteBatch, Palico::SpriteFont* spriteFont)
{
	char buffer[256];
	spriteBatch.begin();

	sprintf_s(buffer, "Num Friendlies: %d", friendlies->size());
	glm::vec2 position(5, screenHeight - spriteFont->getFontHeight());
	glm::vec2 scale(1.0f);

	spriteFont->draw(spriteBatch, buffer, position, scale, 0.0f, Palico::Color::white);

	sprintf_s(buffer, "Num Ghosts: %d", ghosts->size());
	glm::vec2 position2(screenWidth - 5, screenHeight - spriteFont->getFontHeight());
	glm::vec2 scale2(1.0f);

	spriteFont->draw(spriteBatch, buffer, position2, scale2, 0.0f, Palico::Color::white, Palico::Alignment::RIGHT);

	spriteBatch.end();
	spriteBatch.renderBatch();
}

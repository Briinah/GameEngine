#include "HUD.h"

HUD::HUD(std::vector<Friendly*>* friendlies) : friendlies(friendlies)
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

	glm::vec2 position(300, 300);
	glm::vec2 scale(4.0f);

	spriteFont->draw(spriteBatch, buffer, position, scale, 0.0f, Palico::Color::white);

	spriteBatch.end();
	spriteBatch.renderBatch();
}

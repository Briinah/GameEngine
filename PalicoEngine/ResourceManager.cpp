#include "ResourceManager.h"
namespace Palico
{
	TextureCache ResourceManager::textureCache; // needed for static variables

	GLTexture ResourceManager::getTexture(std::string textureName)
	{
		std::string texturePath = "Textures/" + textureName + ".png";
		return textureCache.getTexture(texturePath);
	}
}
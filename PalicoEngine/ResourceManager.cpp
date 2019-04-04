#include "ResourceManager.h"
namespace Palico
{
	TextureCache ResourceManager::textureCache; // needed for static variables

	GLTexture ResourceManager::getTexture(std::string texturePath)
	{
		return textureCache.getTexture(texturePath);
	}
}
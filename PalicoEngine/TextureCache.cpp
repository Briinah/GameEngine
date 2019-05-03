#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace Palico
{
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string filePath)
	{
		auto it = textureMap.find(filePath);
		if (it == textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(filePath);

			textureMap.insert(make_pair(filePath, newTexture));

			return newTexture;
		}

		return it->second;
	}
}
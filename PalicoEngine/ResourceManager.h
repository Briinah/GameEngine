#pragma once
#include "TextureCache.h"
#include <string>
namespace Palico
{
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache textureCache;
	};

}
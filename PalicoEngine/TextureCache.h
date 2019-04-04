#pragma once
#include <map>
#include <string>
#include "GLTexture.h"
namespace Palico
{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string filePath);

	private:
		std::map<std::string, GLTexture> textureMap;
	};

}
#pragma once

#include <string>

#include "GLTexture.h"
#include "picoPNG.h"
namespace Palico
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}
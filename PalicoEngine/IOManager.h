#pragma once

#include <vector>
#include <string>
namespace Palico
{
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};

}
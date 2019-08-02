#pragma once

#include <string>
#include <GL/glew.h>
namespace Palico
{
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();
	private:

		int numAttributes = 0;

		void compileShader(const std::string& filePath, GLuint shaderID);

		GLuint programID = 0;
		GLuint vertexShaderID = 0;
		GLuint fragmentShaderID = 0;
	};

}
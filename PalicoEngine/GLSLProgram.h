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

		int numAttributes;

		void compileShader(const std::string& filePath, GLuint shaderID);

		GLuint programID;
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
	};

}
#include "GLSLProgram.h"

#include "Errors.h"
#include <fstream>
#include <vector>

namespace Palico
{
	GLSLProgram::GLSLProgram() 
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string & vertexShaderPath, const std::string & fragmentShaderPath)
	{
		programID = glCreateProgram();

		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (vertexShaderID == 0)
		{
			fatalError("Vertex shader failed to be creaded");
		}

		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0)
		{
			fatalError("Fragment shader failed to be created");
		}

		compileShader(vertexShaderPath, vertexShaderID);
		compileShader(fragmentShaderPath, fragmentShaderID);
	}


	void GLSLProgram::compileShader(const std::string & filePath, GLuint shaderID)
	{
		std::ifstream file(filePath);
		if (file.fail())
		{
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(file, line))
		{
			fileContents += line + "\n";
		}

		file.close();

		const char* contentsPtr = fileContents.c_str();
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);
		glCompileShader(shaderID);

		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(shaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader failed to compile " + filePath);
		}
	}


	void GLSLProgram::linkShaders()
	{
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);

		glLinkProgram(programID);


		// Error checking
		GLint isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

			glDeleteProgram(programID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Could not link shaders");
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string & attributeName)
	{
		glBindAttribLocation(programID, numAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string & uniformName)
	{
		GLint location = glGetUniformLocation(programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX)
		{
			fatalError("uniform " + uniformName + " not found in shader \n");
		}

		return location;
	}

	void GLSLProgram::use()
	{
		glUseProgram(programID);
		for (int i = 0; i < numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}
}
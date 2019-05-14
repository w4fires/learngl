#include "shader.h"
#include <fstream>
#include <iostream>
Shader::Shader(const ShaderParam *shaders)
{
	ID = glCreateProgram();
	while (shaders->type != GL_NONE) {
		int shader = loadShader(shaders);
		glAttachShader(ID, shader);
		++shaders;
	}

	int success;
	char infoLog[1024];
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}

int Shader::loadShader(const ShaderParam* const shaderParam) {
	std::ifstream shaderFile;
	char *shaderCode = nullptr;
	try {
		//一定要用二进制文件, 否则计算缓冲区长度会出问题
		shaderFile.open(shaderParam->filename, std::ios_base::binary);
		shaderFile.seekg(0, std::ios::end);
		int charLen = shaderFile.tellg();
		shaderCode = new char[charLen / sizeof(char) + 1];
		shaderFile.seekg(0, std::ios::beg);
		shaderFile.read(shaderCode, charLen);
		shaderCode[charLen] = 0;

		//std::stringstream shaderStream;
		//shaderStream << shaderFile.rdbuf();
		//fileContent = shaderStream.str();
		shaderFile.close();
	}
	catch (std::ifstream::failure e) {
		shaderFile.close();
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return 0;
	}

	if (shaderCode != nullptr) {
		int success;
		char infoLog[1024];
		int shader = glCreateShader(shaderParam->type);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderParam->filename << "\n" << infoLog << "\n -- ----------------------------------------- -- " << std::endl;
		}
		delete(shaderCode);
		return shader;
	}
	return 0;
}


Shader::~Shader()
{
}

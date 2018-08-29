#pragma once
#include <string>
#include <GL/glew.h>
struct ShaderParam
{
	GLenum type;
	const char *filename;
};

class Shader
{
public:
	unsigned int ID;
	Shader(const ShaderParam *shaders);
	~Shader();
	void active() { glUseProgram(ID); };
private:
	int loadShader(const ShaderParam* const shader);
};


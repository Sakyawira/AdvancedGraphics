#pragma once
// Dependency Includes
#include <glew.h>
#include <freeglut.h>

// Library Includes
#include <iostream>
#include <string>
#include <map>

class ShaderLoader
{
public:
	static ShaderLoader& getInstance();
	ShaderLoader(ShaderLoader const&) = delete;
	void operator=(ShaderLoader const&) = delete;

	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename, char* geometryShaderFilename);

private:
	ShaderLoader() {}
	~ShaderLoader() {}
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char *filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	std::map<GLuint, std::string> shaderMap;

};

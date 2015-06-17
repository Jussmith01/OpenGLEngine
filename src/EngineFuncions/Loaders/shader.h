#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"

class Shader
{
public:
  	// Our program ID
	GLuint Program;
	std::string shdrname;

	// Constructor reads and builds our shader
	Shader() {};
	Shader(std::string file) {ShaderSet(file);};

    // Setup the shader by loading it, compiling it and linking it into Program
	void ShaderSet(std::string filename);

	// Use the current shader
	void Use();

    // Cleanup for deletion
	void Cleanup();
};

#endif // SHADERLOADER_H

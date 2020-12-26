#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h> // include glew to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	unsigned int id_;

	// constructor reads and builds the shader
	Shader(const char* kVertexPath, const char* kFragmentPath);
	// use/activate the shader
	void Use();
	// utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
};

#endif // !SHADER_H
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Shader {
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	GLuint Program;

	GLvoid USE() { glUseProgram(Program); }

};
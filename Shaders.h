/*
Shaders.h

Flight Simulator - A OpenGL flight simulator written in C++.
Copyright (C) 2026  Noah Forkenbrock <asr-forkenbrock> 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;
extern const char* uiVertexShaderSource;
extern const char* uiFragmentShaderSource;
extern const char* TEXTUREvertexShaderSource;
extern const char* TEXTUREfragmentShaderSource;
extern const char* OBJTEXTUREvertexShaderSourcePOSITION;
extern const char* OBJTEXTUREfragmentShaderSourcePOSITION;
extern const char* OBJTEXTUREvertexShaderSource;
extern const char* OBJTEXTUREfragmentShaderSource;
extern const char* uiTexVertexShaderSource;
extern const char* uiTexFragmentShaderSource; 

unsigned int createShaderProgram();
unsigned int OBJTEXTUREcreateShaderProgram();
unsigned int TEXTUREcreateShaderProgram();

#endif
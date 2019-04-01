#pragma once

#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileUtils.h"

class Shader {
public:
	Shader();
	Shader(const char *vertPath, const char *fragPath, int t);
	~Shader();

	void enable();
	void disable();

	// Sets a uniform integer variable in your shader program to a value
	void setUniform1i(const GLchar *name, int value);

	// Sets a uniform float variable in your shader program to a value
	void setUniform1f(const GLchar *name, float value);

	// Sets a uniform vector2 variable in your shader program to a vector
	void setUniform2f(const GLchar *name, const glm::vec2 &vector);

	// Sets a uniform vector3 variable in your shader program to a vector
	void setUniform3f(const GLchar *name, const glm::vec3 &vector);

	// Sets a uniform vector4 variable in your shader program to a vector
	void setUniform4f(const GLchar *name, const glm::vec4 &vector);

	// Sets a uniform matrix4x4 variable in your shader program to a matrix4x4
	void setUniformMat4(const GLchar *name, const glm::mat4 &matrix);

	int CreateParticleArray(int type);//creates the array from the assignment

	void setAttribute(int type);
	void setRadius(int type);//This switches between what array to use for the shader. 
	// Getters
	inline GLuint getShaderID() { return shaderProgram; }
private:
	GLuint shaderProgram;

	//I moved the variables from the createArray function here because I needed to handle two types of particle effects,
	//and I thought this was the easist way for me to do so. 
	GLuint vbo, ebo;
	GLuint manyface[1000 * 6];

	int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), direction (2), 2D texture coordinates (2), time (1)
								//	const int face_att = 3; // Vertex indices (3)
	//int type;
	GLfloat particleatt[1000 * 7];
};
#pragma once
#include "GameObject.h"
#include <iostream>
#include <stdexcept>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file


class Particle : public GameObject
{
public:
public:
	Particle(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, float angle, float s,float n,int shade);

	// Update function for moving the player object around
	virtual void render(std::vector<Shader*> shaders);
	virtual void update(double deltaTime) override;

	inline void setRange(float r) { range = r; }
	
private:

	float distance;
	glm::vec3 orgCoord;//stores the original coord of where it launched from. 
	
	glm::vec3 scale;
	int particlesize;
	double time;
	int shaderIndex;//Used to identify what shader to use. 
	float range;


};


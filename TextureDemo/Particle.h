#pragma once
#include "GameObject.h"
#include "EnemyObject.h"
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
	Particle(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float s,float n);

	// Update function for moving the player object around
	virtual void render(std::vector<Shader*> shaders);
	virtual void update(double deltaTime) override;
	inline void setCurrEnemy(EnemyObject* enemy) { target = enemy; }

	
private:

	float distance;
	glm::vec3 orgCoord;//stores the original coord of where it launched from. 
	EnemyObject* target;

	glm::vec3 scale;
	int particlesize;
	double time;
	// Source code of vertex shader


};


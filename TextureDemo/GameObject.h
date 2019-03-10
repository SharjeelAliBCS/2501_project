#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

class GameObject {
public:
	GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, std::string t);

	// Updates the GameObject's state. Can be overriden for children
	virtual void update(double deltaTime);

	// Renders the GameObject using a shader
	virtual void  render(Shader &shader);


	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getDirection() { return direction; }
	inline float getSpeed() { return speed; }
	inline glm::vec3& getTargetPos() { return targetPos; }
	inline std::string getType() { return type; }

	inline GLuint getTex() { return texture; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setDirection(glm::vec3& newDir) { direction = newDir; }
	inline void setSpeed(float s) { speed = s; }
	inline void setRotation(float r) { rotation = r; }
	inline void setTargetPos(glm::vec3& newTarPos) { targetPos = newTarPos; }
	inline void setType(std::string t) { type = t; }
	
	inline void setTex(GLuint t) { texture = t; }
	inline void setExists(bool e) {exists = e; }

	inline bool getExists() { return exists; }
	

	//inline void setTex(GLuint)
protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	glm::vec3 position;
	glm::vec3 direction;
	float speed,rotation=0.0f;
	glm::vec3 targetPos;
	bool exists;
	// Object's details
	GLint numElements;
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)
	std::string type;

	// Object's texture
	GLuint texture;
};
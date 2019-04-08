#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"
#include <vector>
#include "Audio.h"
class GameObject {
public:
	GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, std::string t, float speed=1, int c=0);
	// Updates the GameObject's state. Can be overriden for children
	virtual void update(double deltaTime);

	// Renders the GameObject using a shader
	virtual void  render(std::vector<Shader*> shaders);


	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getDirection() { return direction; }
	inline float getDefaultSpeed() { return defaultSpeed; }
	inline float getCurSpeed() { return curSpeed; }
	inline glm::vec3& getTargetPos() { return targetPos; }
	inline std::string getType() { return type; }

	inline GLuint getTex() { return texture; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setDirection(glm::vec3& newDir) { direction = newDir; }
	inline void modCurSpeed(float s) { curSpeed *= s; }
	inline void setRotation(float r) { rotation = r; }
	inline void setTargetPos(glm::vec3& newTarPos) { targetPos = newTarPos; }
	inline void setType(std::string t) { type = t; }
	
	inline void setTex(GLuint t) { texture = t; }
	inline void setExists(bool e) {exists = e; }
	inline void setImgScale(glm::vec3 s) { imgScale = s; }
	inline void setAudio(Audio* a) { audio = a;  }
	inline bool getExists() { return exists; }
	inline float getRotation() { return rotation; }

	inline int getCost() { return cost; }
	inline void setCost(int c) { cost = c; }
	inline void setEffectDuration(float t) { effectTimeLeft = t; }
	inline void setBurnDuration(float t) { burnTimeLeft = t; }

	static float cameraZoom;
	static glm::vec3 cameraTranslatePos;
	//inline void setTex(GLuint)
protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	int cost;
	glm::vec3 position;
	glm::vec3 direction;
	float defaultSpeed, curSpeed,rotation=0.0f;
	float effectTimeLeft,laserCoolDownTime,burnTimeLeft;
	glm::vec3 imgScale;
	glm::vec3 targetPos;
	bool exists;
	// Object's details
	GLint numElements;
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)
	std::string type;
	std::string uniqueID;

	float acceleration, vx, vy;
	glm::vec3 velocity, a;
	glm::vec3 orgCoord;//stores the original coord of where it launched from. glm::vec3 orgCoord;//stores the original coord of where it launched from. 
	// Object's texture
	GLuint texture;
	Audio* audio;
	//std::string uniqueID;
};
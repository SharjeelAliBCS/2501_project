#include "BossObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

BossObject::BossObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements,float h, std::string type)
	: EnemyObject(entityPos, entityTexture, entityNumElements, h,type) {}

// Update function for moving the player object around
void BossObject::update(double deltaTime) {


	// Call the parent's update method to move the object
	EnemyObject::update(deltaTime);
}

#include "EnemyObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

EnemyObject::EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type)
	: GameObject(entityPos, entityTexture, entityNumElements,type) {}

// Update function for moving the player object around
void EnemyObject::update(double deltaTime) {


	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

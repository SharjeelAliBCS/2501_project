#include "TowerObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

TowerObject::TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements) {}

// Update function for moving the player object around
void TowerObject::update(double deltaTime) {

	if (currentEnemy != NULL) {

		rotateTurret();
	}
	



	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

void rotateTurret() {


}

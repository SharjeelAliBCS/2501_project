#include "EnemyObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

EnemyObject::EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type)
	: GameObject(entityPos, entityTexture, entityNumElements,type),hit(false),health(h) {}

void EnemyObject::enemyHit(float damage) {
	health -= damage;
	hit = true;

}
// Update function checks if enemy health is 0
void EnemyObject::update(double deltaTime) {

	if (health <= 0.0f) {
		exists = false;
	}
	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

void EnemyObject::render(std::vector<Shader*> shaders) {

	//Sets the shader to red to signify damage. 
	if (hit) {
		GLint color_loc = glGetUniformLocation(shaders[0]->getShaderID(), "colorMod");
		glUniform3f(color_loc, 1.0f, -1.0f, -0.6f);	//red = damaged
		hit = false;
	}
	GameObject::render(shaders);
}
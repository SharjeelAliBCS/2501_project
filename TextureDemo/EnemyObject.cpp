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
	std::cout << "hp: " << health << std::endl;
}
// Update function for moving the player object around
void EnemyObject::update(double deltaTime) {

	if (health <= 0.0f) {
		exists = false;
	}
	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

void EnemyObject::render(Shader &shader) {
	if (hit) {
		GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
		glUniform3f(color_loc, 1.0f, -1.0f, -1.0f);	//red = start
		hit = false;
	}
	GameObject::render(shader);
}
#include "ProjectileObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

ProjectileObject::ProjectileObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float s)
	: GameObject(entityPos, entityTexture, entityNumElements, type) {
	target = enemy;
	orgCoord = entityPos;
	damage = 10.0f;
	speed = s;
	exists = true;
	rotation = angle;
}

// Update function for moving the player object around
void ProjectileObject::update(double deltaTime) {
	float vX = 0;
	float vY = 0;
	vX = speed * cos(rotation*3.14159 / 180);
	vY = speed * sin(rotation*3.14159 / 180);
	position.x += vX;
	position.y += vY;
	float distanceTar = glm::length(position - target->getPosition());
	float distanceTower = glm::length(position - orgCoord);
	if (distanceTar <= 0.2 || distanceTower>=20) {
		exists = false;
	}


	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}
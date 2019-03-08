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

ProjectileObject::ProjectileObject(glm::vec3 &entityPos, GLuint entityTexture, std::vector<GLuint> explosion,GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float s)
	: GameObject(entityPos, entityTexture, entityNumElements, type) {
	target = enemy;
	orgCoord = entityPos;
	damage = 10.0f;
	speed = s;
	exists = true;
	rotation = angle;
	explosion_tex = explosion;
	explosion_num = -1;
}

// Update function for moving the player object around
void ProjectileObject::update(double deltaTime) {

	if (explosion_num >= 0) {
		setTex(explosion_tex[explosion_num]);
		explosion_num++;

		if (explosion_num == 5)exists = false;

	}
	else {
		float vX = 0;
		float vY = 0;
		vX = speed * cos(rotation*3.14159 / 180);
		vY = speed * sin(rotation*3.14159 / 180);
		position.x += vX;
		position.y += vY;
		float distanceTar = glm::length(position - target->getPosition());
		float distanceTower = glm::length(position - orgCoord);
		std::cout << "distance = " << distanceTower << std::endl;
		if (distanceTar <= 0.2 || distanceTower >= 10) {
			explosion_num++;
			target->setHit(true);
		}
	}
	
	


	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}
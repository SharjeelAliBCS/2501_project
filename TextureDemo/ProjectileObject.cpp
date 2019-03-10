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

ProjectileObject::ProjectileObject(glm::vec3 &entityPos, GLuint entityTexture, std::vector<GLuint> explosion,GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float d, float s)
	: GameObject(entityPos, entityTexture, entityNumElements, type) {
	target = enemy;
	orgCoord = entityPos;
	damage = 10.0f;
	speed = s;
	rotation = angle;
	explosion_tex = explosion;
	explosion_num = -1;
	damage = d;
}

// Update function for moving the bullet object around
void ProjectileObject::update(double deltaTime) {

	//checks if it should despawn, so adds the explosion sprites
	if (explosion_num >= 0) {
		texture = explosion_tex[explosion_num];
		explosion_num++;

		if (explosion_num == 5)exists = false;

	}
	else {

		//explodes if there are no targets to shoot at
		if (target == NULL)explosion_num++;
		else {
			
			//follows the enemy around
			float vX = 0;
			float vY = 0;
			vX = speed * cos(rotation*3.14159 / 180);
			vY = speed * sin(rotation*3.14159 / 180);
			position.x += vX;
			position.y += vY;


			float distanceTar = glm::length(position - target->getPosition());
			float distanceTower = glm::length(position - orgCoord);
			
			//if it should despawn, increments the explosion counter. 
			if (distanceTar <= 0.2 || distanceTower >= 10) {
				explosion_num++;
				target->enemyHit(damage);
			}
		}
	}



	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}
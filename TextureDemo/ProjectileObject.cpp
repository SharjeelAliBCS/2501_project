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

ProjectileObject::ProjectileObject(glm::vec3 &entityPos, GLuint entityTexture, std::vector<GLuint> explosion, GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float d, float s)
	: GameObject(entityPos, entityTexture, entityNumElements, type, s) {
	target = enemy;
	orgCoord = entityPos;
	damage = 10.0f;
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
			vX = curSpeed * cos(rotation*3.14159 / 180);
			vY = curSpeed * sin(rotation*3.14159 / 180);
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

void ProjectileObject::render(std::vector<Shader*> shaders) {

	if (type.compare("Laserbeam")) {
		GameObject::render(shaders);
	}
	else {
		if (position.x < -1.1 / cameraZoom - cameraTranslatePos.x ||
			position.x > 1.1 / cameraZoom - cameraTranslatePos.x ||
			position.y < -0.5 / cameraZoom - cameraTranslatePos.y ||
			position.y > 1.1 / cameraZoom - cameraTranslatePos.y) {
			return; //uncomment for fps boost based on zoom
		}
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, texture);
		float length = imgScale.x;
		// Setup the transformation matrix for the shader
		float tX = (length / 9)*cos(rotation*3.14159 / 180);
		float tY = (length / 9)*sin(rotation*3.14159 / 180);

		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tX, tY, 0.0f));
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f,0.3f,0.3f));
		//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 1.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); //unknown why not all same, 3:4:4 seems a good ratio though
		glm::mat4 imgScaleMatrix = glm::scale(glm::mat4(1.0f), imgScale); //unknown why not all same, 3:4:4 seems a good ratio though


																		  // Set the transformation matrix in the shader
		glm::mat4 transformationMatrix = offsetMatrix * translationMatrix * rotationMatrix * scaleMatrix*imgScaleMatrix;
		//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);


	}

}
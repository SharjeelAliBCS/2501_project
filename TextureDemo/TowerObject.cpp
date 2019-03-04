#include "TowerObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

TowerObject::TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLuint turrTex, GLuint bulTex,GLint entityNumElements, std::string type)
	: GameObject(entityPos, entityTexture, entityNumElements, type) {
	projectileTex = bulTex;
	rotation = 0.0f;
	turretTexture = turrTex;
	size = numElements;
	_state = Init;
	fireRate = 10;
	frames = 0;

	
}

// Update function for moving the player object around
void TowerObject::update(double deltaTime) {
	
	std::vector<int> deleteBullets;

	for (int i = 0; i < bullObjects.size(); i++) {
		bullObjects[i]->setCurrEnemy(currentEnemy);
		bullObjects[i]->setRotation(rotation);
		bullObjects[i]->update(deltaTime);

		if (!bullObjects[i]->getExists()) {
			deleteBullets.push_back(i);
		}
		
	}

	for (int i = 0; i < deleteBullets.size(); i++) {
		std::cout << "ddd" << std::endl;
		bullObjects.erase(bullObjects.begin() + deleteBullets[i]);
		//delete bullObjects[deleteBullets[i]];
	}

	switch (_state) {
	case Init:
		_state = Locate;
		break;
	case Idle:
		break;
	case Locate:
		locateEnemy();
		break;
	case Fire:
		break;
	case Death:
		break;
	default:
		break;
	}
	
	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}
void TowerObject::locateEnemy() {
	float tx = position.x;
	float ty = position.y;
	float ex = currentEnemy->getPosition().x;
	float ey = currentEnemy->getPosition().y;

	float deltaX = tx-ex;
	float deltaY = ty-ey;
	float targetAngle = atan2(deltaY, deltaX);
	//std::cout << "enemy x: " <<  ex <<", enemy y: " << ey << std::endl;
	
	rotation = targetAngle * (180 / 3.14f)+180;

	fireEnemy();
	
	//std::cout << turretAngle << std::endl;

}

void TowerObject::deathAnimation() {
}

void TowerObject::fireEnemy() {
	if (frames%fireRate==0) {
		
		bullObjects.push_back(new ProjectileObject(position, projectileTex, size, "t_projectile", currentEnemy, rotation, 0.3));
	}
	frames += 1;

	

}


void TowerObject::render(Shader &shader) {

	for (int i = 0; i < bullObjects.size(); i++) {
		bullObjects[i]->render(shader);
	}
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, turretTexture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); //unknown why not all same, 3:4:4 seems a good ratio though


																		// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix*scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	
	transformationMatrix = translationMatrix  * scaleMatrix;


	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}



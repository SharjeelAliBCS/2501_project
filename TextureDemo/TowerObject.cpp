#include "TowerObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

TowerObject::TowerObject(glm::vec3 &entityPos, std::vector<GLuint> tex, std::vector<GLuint> explosion,GLint entityNumElements, float d,std::string type)
	: GameObject(entityPos, tex[0], entityNumElements, type) {

	turretTexture = tex[1];
	projectileTex = tex[2];
	icon = tex[3];
	rotation = 0.0f;
	size = numElements;
	_state = Init;
	fireRate = 10;
	frames = 0;
	explosion_tex = explosion;
	dps = d; 
	texvec = tex;


	
}

// Update function
void TowerObject::update(double deltaTime) {
	

	std::deque<int> deleteBullets;

	for (int i = 0; i < bullObjects.size(); i++) {
		bullObjects[i]->setCurrEnemy(currentEnemy);
		bullObjects[i]->setRotation(rotation);
		bullObjects[i]->update(deltaTime);

		if (!bullObjects[i]->getExists()) {
			deleteBullets.push_front(i);
		}
		
	}
	//delete the bullets that should be deleted
	for (int i = 0; i < deleteBullets.size(); i++) bullObjects.erase(bullObjects.begin() + deleteBullets[i]);


	//state machine used to move around (right now only uses locate)
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

//locates the enemy based off of it's coordinates
void TowerObject::locateEnemy() {

	if (!currentEnemy == NULL) {
		float tx = position.x;
		float ty = position.y;
		float ex = currentEnemy->getPosition().x;
		float ey = currentEnemy->getPosition().y;

		float deltaX = tx - ex;
		float deltaY = ty - ey;
		float targetAngle = atan2(deltaY, deltaX);
		
		rotation = targetAngle * (180 / 3.14f) + 180;

		fireEnemy();

	}

}

void TowerObject::deathAnimation() {
}

//creates a single bullet object
void TowerObject::fireEnemy() {
	if (frames%fireRate==0) {
		
		bullObjects.push_back(new ProjectileObject(position, projectileTex, explosion_tex,size, "t_projectile", currentEnemy, rotation, dps,0.3));
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
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); //unknown why not all same, 3:4:4 seems a good ratio though														// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix*scaleMatrix;
	
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);

	
	
	
	transformationMatrix = translationMatrix  * scaleMatrix;


	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}



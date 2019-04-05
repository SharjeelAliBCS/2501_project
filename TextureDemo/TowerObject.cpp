#include "TowerObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

TowerObject::TowerObject(glm::vec3 &entityPos, std::vector<GLuint> tex, std::vector<GLuint> explosion,GLint entityNumElements, float d,std::string type,float r, float ROF, int c)
	: GameObject(entityPos, tex[0], entityNumElements, type,0,c) {

	turretTexture = tex[1];
	projectileTex = tex[2];
	icon = tex[3];
	rotation = 0.0f;
	size = numElements;
	_state = Init;
	curROF = ROF;
	defaultROF = ROF;
	lastShotTime = 0;
	frames = 0;
	explosion_tex = explosion;
	damage = d; 
	texvec = tex;
	range = r;

	std::cout << "Cost: " << cost << " ROF: " << ROF << " range: " << range<<std::endl;
	std::cout << type << std::endl;



	
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

	if (particle != NULL) {
		particle->setRotation(rotation-90);

		if (currentEnemy != NULL) {
			float dis = glm::length(position - currentEnemy->getPosition());
			//std::cout << "dis " << dis << std::endl;
			if (dis < range) {
				currentEnemy->enemyHit(damage);
			}
		}
	}
	//delete the bullets that should be deleted
	for (int i = 0; i < deleteBullets.size(); i++) bullObjects.erase(bullObjects.begin() + deleteBullets[i]);
	if (effectTimeLeft < 0) {
		curROF = defaultROF;
	}
	if ((currentEnemy == NULL || glm::length(position - currentEnemy->getPosition())>range ) && type.compare("denderBlueprint---2")==0) {
		particle = NULL;
		std::cout << "fdire"<<std::endl;
		audio->close(uniqueID);
	}
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

		float dis = glm::length(position - currentEnemy->getPosition());
		//std::cout << "dis " << dis << std::endl;
		if (dis <= range) {
			fireEnemy();
		}

	}

}

void TowerObject::deathAnimation() {
}


void TowerObject::fireEnemy() {

	//creates a single particle object (flames)
	if (type.compare("denderBlueprint---2") == 0) {
		if (particle == NULL){
			audio->addAudio("Audio/fire.mp3", uniqueID);
			audio->volume(uniqueID, 30);
			audio->playRepeat(uniqueID);
			
			particle = new Particle(position, projectileTex, size, "particle", 0, 0.075f, 2000, 1);
		}
	}
	//creates a single bullet object
	else {
		if(lastShotTime+curROF<glfwGetTime()){
			audio->playAgain("bullet");
			lastShotTime = glfwGetTime();
			
			bullObjects.push_back(new ProjectileObject(position, projectileTex, explosion_tex, size, "t_projectile", currentEnemy, rotation, damage, 0.1));
		}
	}
	

}


void TowerObject::render(std::vector<Shader*> shaders) {

	glBindTexture(GL_TEXTURE_2D, turretTexture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); //unknown why not all same, 3:4:4 seems a good ratio though														// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix*scaleMatrix;
	
	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);


	for (int i = 0; i < bullObjects.size(); i++) {
		bullObjects[i]->render(shaders);
	}
	
	if (particle != NULL) {
		particle->render(shaders);
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	transformationMatrix = translationMatrix  * scaleMatrix;


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}



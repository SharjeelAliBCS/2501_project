#include "TowerObject.h"

#include "Window.h"

/*
PlayerGameObject inherits from GameObject
It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

TowerObject::TowerObject(glm::vec3 &entityPos, std::vector<GLuint> tex, std::vector<GLuint> explosion, GLint entityNumElements, float d, std::string type, float r, float ROF, int c, float speed)
	: GameObject(entityPos, tex[0], entityNumElements, type, 0, c) {

	turretTexture = tex[1];
	projectileTex = tex[2];
	icon = tex[3];
	rotation = 0.0f;
	size = numElements;
	_state = Idle;
	curROF = ROF;
	defaultROF = ROF;
	baseROF = ROF;
	lastShotTime = 0;
	frames = 0;
	explosion_tex = explosion;
	damage = d;
	baseDamage = d;
	texvec = tex;
	range = r;
	baseRange = r;
	projectileSpeed = speed;
	orgCoord = position;
	explosion_num = -1;
	orgSpeed = speed;
	duration = -1;

	//if (type.compare("A-Class Auto Assault Bomber") == 0)curSpeed = speed;
	std::cout << type << " Cost: " << cost << " ROF: " << ROF << " range: " << range << " speed = " << projectileSpeed << std::endl;




}

TowerObject::~TowerObject() {
	if (particle != NULL) {
		delete(particle);
		particle = NULL;

	}

	for (int i = 0; i< bullObjects.size();i++)
	{
		delete (bullObjects[i]);
	}
	bullObjects.clear();

	audio->close(uniqueID);
}

// Update function
void TowerObject::update(double deltaTime) {


	std::deque<int> deleteBullets;

	if (type.compare("A-Class High Beam Laser")) {
		for (int i = 0; i < bullObjects.size(); i++) {
			bullObjects[i]->setCurrEnemy(currentEnemy);
			bullObjects[i]->setRotation(rotation);
			bullObjects[i]->update(deltaTime);

			if (!bullObjects[i]->getExists()) {
				deleteBullets.push_front(i);
			}

		}
	}

	if (particle != NULL) {
		particle->setRotation(rotation - 90);

		if (currentEnemy != NULL) {
			float dis = glm::length(position - currentEnemy->getPosition());
			//std::cout << "dis " << dis << std::endl;
			if (dis < range && prevEnemy != currentEnemy) {
				//currentEnemy->enemyHit(damage);
				currentEnemy->setBurnDuration(curROF);
				currentEnemy->enemyBurn(damage);
				prevEnemy = currentEnemy;
			}
		}
	}
	//delete the bullets that should be deleted
	for (int i = 0; i < deleteBullets.size(); i++) {
		ProjectileObject* p = bullObjects[deleteBullets[i]];
		bullObjects.erase(bullObjects.begin() + deleteBullets[i]);
		delete(p);
		p = NULL;
	}
	if (effectTimeLeft < 0) {
		curROF = defaultROF;
	}


	//state machine used to move around (right now only uses locate)
	switch (_state) {

	case Idle: {
		if (allEnemies.size() > 0) {

			if (type.compare("A-Class Auto Assault Bomber") == 0) _state = SpeedUp;
			else _state = Locate;
		}
		break;
	}
	case Locate: {
		locateEnemy();
		break;
	}
	case Fire: {
		fireEnemy(deltaTime);
		break;
	}

	case SlowDown: {
		//std::cout << "speed  = " << projectileSpeed << std::endl;
		if (projectileSpeed <= orgSpeed * 0.5) {
			if (allEnemies.size() == 0) {
				_state = Idle;
			}

			else _state = SpeedUp;
		}
		else {
			projectileSpeed -= accelerationSlow;
			move();
		}
		break;
	}
	case SpeedUp: {
		if (projectileSpeed >= orgSpeed) {
			projectileSpeed = orgSpeed;
			_state = Locate;
			//bullObjects.push_back(new ProjectileObject(position, projectileTex, explosion_tex, size, "t_projectile", currentEnemy, rotation, damage, projectileSpeed*2));
		}
		else {
			projectileSpeed += accelerationSlow;
			move();
		}

		break;
	}
	case CoolDown: {
		if (laserCoolDownTime <= 0) {
			_state = Locate;
		}
	}
	default:
		break;
	}

	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

void TowerObject::move() {

	if (currentEnemy != NULL && _state == Locate) {
		std::cout << "target" << _state << std::endl;
		targetPos = currentEnemy->getPosition();

	}

	float dis = glm::length(position - (targetPos));

	if (dis > 0.1) {
		rotation = (int)rotation % 360;
		glm::vec3 dir = targetPos - position;


		glm::vec3 moveVec = dir / glm::length(dir);
		a = acceleration * moveVec;

		velocity += a;
		if (glm::length(velocity) > projectileSpeed) {
			velocity = projectileSpeed * velocity / glm::length(velocity);
		}

		position = glm::vec3(position.x + velocity.x, position.y + velocity.y, 0.0f);

		float targetAngle = atan2(velocity.y, velocity.x);
		rotation = targetAngle * (180 / 3.14f);
	}
	else {

		std::cout << "distance too short" << std::endl;
	}





}
//locates the enemy based off of it's coordinates
void TowerObject::locateEnemy() {

	if (type.compare("AOE") == 0) {

		enemiesInRange(range);
		if (currentEnemies.size() > 0) {
			_state = Fire;
			bullObjects.push_back(new ProjectileObject(position, projectileTex, explosion_tex, size, "circle", currentEnemy, rotation, damage, 0));
			duration = 2.0f;
			timeSince = 0.0f;

		}
	}
	else if ((currentEnemy == NULL || glm::length(position - currentEnemy->getPosition())>range) && type.compare("B-Class IGNITION Cannon") == 0) {
		std::cout << "Right";
		if (particle != NULL) {
			delete(particle);
			particle = NULL;
		}
		_state = Idle;
		std::cout << "Now\n";
		audio->close(uniqueID);
	}

	else if (type.compare("A-Class Auto Assault Bomber") == 0) {

		if (allEnemies.size() == 0) {
			std::cout << "idle" << std::endl;
			_state = SlowDown;
			int angle = (std::rand() % (360));

			targetPos.x += 10 * cos(angle*3.14159 / 180);
			targetPos.y += 10 * sin(angle*3.14159 / 180);
		}

		move();

		setCurrEnemies(enemiesInRange(range));
		if(currentEnemy!=NULL &&  glm::length(position - currentEnemy->getPosition()) < 1) {
			_state = Fire;
			}



	}
	else {

		if (allEnemies.size() == 0) {
			std::cout << "idle" << std::endl;
			_state = Idle;
		}
		else if (!currentEnemy == NULL) {
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
				_state = Fire;
			}

		}


	}

}


void TowerObject::fireEnemy(double deltaTime) {

	if (type.compare("AOE") == 0) {

		bullObjects[0]->setImgScale(glm::vec3(6 * range*projectileSpeed*(timeSince / duration), 6 * range*projectileSpeed*(timeSince / duration), 1.0f));
		std::cout << "range = " << range * projectileSpeed*(timeSince / duration) << std::endl;
		enemiesInRange(range*projectileSpeed*(timeSince / duration));
		if (timeSince*projectileSpeed >= duration) {
			for (EnemyObject* e : currentEnemies) e->enemyHit(damage);

			std::cout << "duraton " << timeSince * projectileSpeed << std::endl;
			laserCoolDownTime = curROF;
			_state = CoolDown;
			delete(bullObjects[0]);
			bullObjects.pop_back();
			audio->playAgain("cooldown");
		}
	}

	else if (type.compare("A-Class Auto Assault Bomber") == 0) {

		if (explosion_num == -1) {
			explosion_num++;
			std::cout << "BOMBER RUN!" << std::endl;
			explodePos = position;
			audio->playAgain("ship");
			for (EnemyObject* e : currentEnemies) {
				e->enemyHit(damage);
			}
			_state = SlowDown;

			int angle = (std::rand() % (360));

			targetPos.x+= 10 * cos(angle*3.14159 / 180);
			targetPos.y += 10 * sin(angle*3.14159 / 180);

		}

	}
	else if (type.compare("A-Class High Beam Laser") == 0) {
		if (bullObjects.size() == 0) {
			audio->addAudio("Audio/Towers/laser.mp3", uniqueID);
			audio->volume(uniqueID, 10);
			audio->playRepeat(uniqueID);
			bullObjects.push_back(new ProjectileObject(position, projectileTex, explosion_tex, size, "Laserbeam", currentEnemy, rotation, damage, 0));
			bullObjects[0]->setImgScale(glm::vec3(range*3.5, 1, 1));
			laserCoolDownTime = curROF;
		}

		if (currentEnemy != NULL) {
			bullObjects[0]->setCurrEnemy(currentEnemy);
			bullObjects[0]->setRotation(rotation);
			locateEnemy();

			for (EnemyObject* e : allEnemies) {
				if (lineCollision(e)) {
					std::cout << "dmg = " << (damage * deltaTime / curROF) << std::endl;
					e->enemyHit(damage*deltaTime/curROF);
				}
			}

			if (laserCoolDownTime <= 0) {
				_state = CoolDown;
				laserCoolDownTime = curROF;
				audio->close(uniqueID);
				delete(bullObjects[0]);
				bullObjects.pop_back();
				audio->playAgain("cooldown");
			}

		}
		else {
			audio->close(uniqueID);
			delete(bullObjects[0]);
			bullObjects.pop_back();
			_state = Locate;
		}



	}
	else if (type.compare("C-Class BARRIER") == 0) {

	}
	//creates a single particle object (flames)
	else if (type.compare("B-Class IGNITION Cannon") == 0) {
		if (particle == NULL) {
			audio->addAudio("Audio/Towers/fire.mp3", uniqueID);
			audio->volume(uniqueID, 30);
			audio->playRepeat(uniqueID);

			particle = new Particle(position, projectileTex, size, "particle", 0, 0.075f, 2000, 1);
		}
		_state = Locate;
	}
	//creates a single bullet object
	else {
		if (lastShotTime + curROF<glfwGetTime()) {
			audio->playAgain("bullet");
			lastShotTime = glfwGetTime();

			bullObjects.push_back(new ProjectileObject(position, projectileTex, explosion_tex, size, "t_projectile", currentEnemy, rotation, damage, projectileSpeed));
		}
		_state = Locate;
	}


}
bool TowerObject::lineCollision(EnemyObject* enemy) {
	float x1 = position.x;
	float y1 = position.y;
	float x2 = 2 * (range*0.4)*cos(rotation*3.14159 / 180) + x1;
	float y2 = 2 * (range*0.4) *sin(rotation*3.14159 / 180) + y1;
	float ex = enemy->getPosition().x;
	float ey = enemy->getPosition().y;

	float radius = 0.3;

	float lineDotProduct = ((ex - x1) * (x2 - x1) + (ey - y1) * (y2 - y1)) / (pow((x2 - x1), 2) + pow((y2 - y1), 2));

	lineDotProduct = std::fmax(std::fmin(lineDotProduct, 1), 0);

	float pointOnLineX = x1 + lineDotProduct * (x2 - x1);
	float pointOnLineY = y1 + lineDotProduct * (y2 - y1);

	float distance = pow((pointOnLineX - ex), 2) + pow((pointOnLineY - ey), 2);

	if (distance <= pow(radius, 2)) {

		return true;
	}
	else {

		return false;
	}

}

void TowerObject::render(std::vector<Shader*> shaders) {

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); //unknown why not all same, 3:4:4 seems a good ratio though														// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix;

	if (type.compare("A-Class Auto Assault Bomber") == 0) {

		glBindTexture(GL_TEXTURE_2D, turretTexture);

		// Setup the transformation matrix for the shader
		translationMatrix = glm::translate(glm::mat4(1.0f), orgCoord);

		transformationMatrix = translationMatrix * scaleMatrix;

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
		rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		translationMatrix = glm::translate(glm::mat4(1.0f), position);
		rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation - 90, glm::vec3(0.0f, 0.0f, 1.0f));
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.6f, 0.6f));

	}
	else if (type.compare("C-Class BARRIER")) {

		rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, turretTexture);

		// Setup the transformation matrix for the shader

		transformationMatrix = translationMatrix * rotationMatrix*scaleMatrix;
		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
		rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);


		for (int i = 0; i < bullObjects.size(); i++) {
			bullObjects[i]->render(shaders);
		}

		if (particle != NULL) {
			particle->render(shaders);
		}

	}

	glBindTexture(GL_TEXTURE_2D, texture);
	transformationMatrix = translationMatrix * rotationMatrix*scaleMatrix;


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	if (explosion_num >= 0) {
		translationMatrix = glm::translate(glm::mat4(1.0f), explodePos);
		glBindTexture(GL_TEXTURE_2D, explosion_tex[(int)explosion_num / 2]);
		transformationMatrix = translationMatrix * rotationMatrix*scaleMatrix;


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		explosion_num++;

		if (explosion_num == 5) {

			explosion_num = -1;
		}

	}

}


std::vector<EnemyObject*> TowerObject::enemiesInRange(float range) {
	if (range == -1) {
		return allEnemies;
	}
	std::vector<EnemyObject*> targets;

	for (EnemyObject* e : allEnemies) {

		float enemyDistance = glm::length(position - e->getPosition());
		if (enemyDistance < range) {
			targets.push_back(e);
		}
	}
	currentEnemies = targets;
	return targets;
}

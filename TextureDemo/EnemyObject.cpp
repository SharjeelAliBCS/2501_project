#include "EnemyObject.h"

#include "Window.h"

/*
PlayerGameObject inherits from GameObject
It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

EnemyObject::EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt, float speed, int c, float regen)
	: GameObject(entityPos, entityTexture, entityNumElements, type, std::fmin(2.5, speed) , c),
	enemyDeathTex(edt), framesDeath(-1), deathParticles(NULL) {
	health = h;
	defaultHealthCap = h;
	baseHealthCap = h;
	baseCost = c;
	curHealthCap = h;
	this->regen = regen;
	baseRegen = regen;
	baseSpeed = std::fmin(2.5, speed);
	hit = false;
	killed = false;
	spawned = false;

	_state = Init;

}

EnemyObject::~EnemyObject() {
	delete(deathParticles);
}

void EnemyObject::enemyHit(float damage) {
	if (spawned) {
		health -= damage;
		hit = true;
	}
}
void EnemyObject::enemyBurn(float b) {
	burn += b;
	hit = true;

}
// Update function checks if enemy health is 0
void EnemyObject::update(double deltaTime) {

	switch (_state) {
	case Init: {
		_state = Move;
		break;
	}
	
	//move state, move to the checkpoint
	case Move: {

		//if there is a burn damage, deal the damage to the health. 
		if (burn > 0) {
			if (burnTimeLeft > 0) {
				health -= burn*deltaTime;
			}
			else {
				burn = 0;
			}
		}
		//used for any effect on the enemy. 
		if (effectTimeLeft <= 0) {
			curHealthCap = defaultHealthCap;
			health = std::fmin(curHealthCap, health);
		}
		else if (!spawned) {
			effectTimeLeft += deltaTime;
		}
		timeSinceLastHeal -= deltaTime;
		if (timeSinceLastHeal <= 0) {
			health = std::fmin(health + regen, curHealthCap);
			timeSinceLastHeal = 1;
		}
		//Here if the health is zero,spawn the particle system for the death animation. 
		if (killed || (health <= 0.0f && framesDeath == -1)) {
			killed = true;
			deathParticles = new Particle(position, enemyDeathTex, numElements, "particle", 0, 0.04f, 300, 2);
			framesDeath++;
			audio->playAgain("enemyDeath");
			_state = Dying;
		}
	
		break;
	}

	case Dying: {
		//Once the frames reaches 10, stop and remove the enemy/particles. 
		if (framesDeath >= 10) {
			_state = Dead;
		}
		framesDeath++;
		break;
	}
	//set it to not exist so the main can remove it. 
	case Dead: {
		exists = false;
		killed = true;
		break;
	}
	default:
		break;

	}


	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

void EnemyObject::render(std::vector<Shader*> shaders) {
	//does an fps boost based on zoom. 
	if (position.x < -1.1 / cameraZoom - cameraTranslatePos.x ||
		position.x > 1.1 / cameraZoom - cameraTranslatePos.x ||
		position.y < -0.5 / cameraZoom - cameraTranslatePos.y ||
		position.y > 1.1 / cameraZoom - cameraTranslatePos.y) {
		return; 
	}
	switch (_state) {

	case Move: {
		if (hit || burn>0) {
			//Sets the shader to red to signify damage. 
			GLint color_loc = glGetUniformLocation(shaders[0]->getShaderID(), "colorMod");
			glUniform3f(color_loc, 1.0f, -1.0f, -0.6f);	//red = damaged
			hit = false;
		}
		GameObject::render(shaders);
		break;
	}

	case Dying: {
		deathParticles->render(shaders);
		break;
	}
	default:
		break;

	}

}
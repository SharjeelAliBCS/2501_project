#include "EnemyObject.h"

#include "Window.h"

/*
PlayerGameObject inherits from GameObject
It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

EnemyObject::EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt, float speed, int c, float regen)
	: GameObject(entityPos, entityTexture, entityNumElements, type, speed, c),
	enemyDeathTex(edt), framesDeath(-1), deathParticles(NULL) {
	health = h;
	defaultHealthCap = h;
	curHealthCap = h;
	this->regen = regen;
	hit = false;
	killed = false;
	spawned = false;

	_state = Init;

}

void EnemyObject::enemyHit(float damage) {
	health -= damage;
	hit = true;
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
	case Move: {
		if (burn > 0) {
			if (burnTimeLeft > 0) {
				health -= burn;

			}
			else {
				burn = 0;
			}
		}
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
		if (health <= 0.0f && framesDeath == -1) {
			killed = true;
			deathParticles = new Particle(position, enemyDeathTex, numElements, "particle", 0, 0.04f, 300, 2);
			framesDeath++;
			audio->playAgain("enemyDeath");
			//std::cout << " death " << std::endl;
			_state = Dying;
		}
		if (framesDeath >= 0) {

		}
		break;
	}

	case Dying: {
		//std::cout << "time : " << framesDeath << std::endl;

		//Once the frames reaches 10, stop and remove the enemy/particles. 
		if (framesDeath >= 10) {
			_state = Dead;
		}
		framesDeath++;
		break;
	}
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
	if (position.x < -1.1 / cameraZoom - cameraTranslatePos.x ||
		position.x > 1.1 / cameraZoom - cameraTranslatePos.x ||
		position.y < -0.5 / cameraZoom - cameraTranslatePos.y ||
		position.y > 1.1 / cameraZoom - cameraTranslatePos.y) {
		return; //uncomment for fps boost based on zoom
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
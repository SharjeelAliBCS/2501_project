#include "EnemyObject.h"

#include "Window.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

EnemyObject::EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt, int c)
	: GameObject(entityPos, entityTexture, entityNumElements,type,c),
	hit(false),killed(false),health(h),enemyDeathTex(edt), framesDeath(-1),deathParticles(NULL){}

void EnemyObject::enemyHit(float damage) {
	health -= damage;
	hit = true;
}
// Update function checks if enemy health is 0
void EnemyObject::update(double deltaTime) {

	//Here if the health is zero,spawn the particle system for the death animation. 
	if (health <= 0.0f && framesDeath ==-1) {
		deathParticles = new Particle(position, enemyDeathTex, numElements, "particle", 0, 0.04f, 300, 2);
		framesDeath++;
		//std::cout << " death " << std::endl;
	}
	if (framesDeath >=0) {
		//std::cout << "time : " << framesDeath << std::endl;
		
		//Once the frames reaches 10, stop and remove the enemy/particles. 
		if (framesDeath >= 10) {
			exists = false;
			killed = true;
		}
		framesDeath++;
	}
	// Call the parent's update method to move the object
	GameObject::update(deltaTime);
}

void EnemyObject::render(std::vector<Shader*> shaders) {

	if (framesDeath == -1) {
		if (hit) {
			//Sets the shader to red to signify damage. 
			GLint color_loc = glGetUniformLocation(shaders[0]->getShaderID(), "colorMod");
			glUniform3f(color_loc, 1.0f, -1.0f, -0.6f);	//red = damaged
			hit = false;
		}
		GameObject::render(shaders);
		

	}
	else if(exists){
		
		deathParticles->render(shaders);
	}
}
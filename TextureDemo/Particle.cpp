#include "Particle.h"

#include "Window.h"

/*
PlayerGameObject inherits from GameObject
It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

/*
PlayerGameObject inherits from GameObject
It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

Particle::Particle(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float s,float n)
	: GameObject(entityPos, entityTexture, entityNumElements, type) {
	target = enemy;
	orgCoord = entityPos;
	
	scale = glm::vec3(s,s,1);
	rotation = angle;

	particlesize = n;
	time = 0.0;

}


// Update function for moving the bullet object around
void Particle::update(double deltaTime) {

	time = fmod(deltaTime, 2);

	GameObject::update(deltaTime);

}

void Particle::render(Shader& shader) {

	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	GLuint particleprogram = shader.getShaderID();
	// Select proper shader program to use
	glUseProgram(particleprogram);

	//set displacement
	int matrixLocation = glGetUniformLocation(particleprogram, "x");
	int timeLocation = glGetUniformLocation(particleprogram, "time");

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();
	//position.x = 0.5;
	float k = glfwGetTime();
	rot = glm::rotate(rot, rotation, glm::vec3(0, 0, 1));
	//rot = glm::rotate(rot, glm::vec3(rotation));
	rot = glm::translate(rot, position);
	rot = glm::scale(rot,scale);
	// get ready to draw, load matrix
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &rot[0][0]);
	glUniform1f(timeLocation, k);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Draw 
	//glDepthMask(GL_FALSE); // draw particles without writing to depth buffer
	glDrawElements(GL_TRIANGLES, 6 * particlesize, GL_UNSIGNED_INT, 0);


	glDepthMask(GL_TRUE); // allow writes to depth buffer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

}
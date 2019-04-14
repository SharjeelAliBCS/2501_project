#include "Particle.h"

#include "Window.h"


Particle::Particle(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, float angle, float s,float n,int shade)
	: GameObject(entityPos, entityTexture, entityNumElements, type) {
	
	orgCoord = entityPos;
	
	scale = glm::vec3(s,s,1);
	rotation = angle;

	particlesize = n;
	time = 0.0;
	shaderIndex = shade;
}

void Particle::update(double deltaTime) {

	time = fmod(deltaTime, 2);

	GameObject::update(deltaTime);

}

void Particle::render(std::vector<Shader*> shaders) {
	//gives a small fps zoom
	if (position.x < -1.1 / cameraZoom - cameraTranslatePos.x ||
		position.x > 1.1 / cameraZoom - cameraTranslatePos.x ||
		position.y < -0.5 / cameraZoom - cameraTranslatePos.y ||
		position.y > 1.1 / cameraZoom - cameraTranslatePos.y) {
		return; 
	}
	//Here it's disabling and configuring the shader to match the particle system's requirements
	shaders[0]->disable();
	shaders[shaderIndex]->enable();
	shaders[shaderIndex]->setAttribute(shaderIndex);

	glEnable(GL_BLEND);
	GLuint particleprogram = shaders[shaderIndex]->getShaderID();
	// Select proper shader program to use
	glUseProgram(particleprogram);

	//set displacement
	int matrixLocation = glGetUniformLocation(particleprogram, "x");
	int timeLocation = glGetUniformLocation(particleprogram, "time");
	float k = glfwGetTime();

	glm::mat4 rot = glm::mat4();
	
	rot = glm::translate(rot, position);
	rot = glm::rotate(rot, rotation, glm::vec3(0, 0, 1));
	
	rot = glm::scale(rot,scale);
	// get ready to draw, load matrix
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &rot[0][0]);

	glUniform1f(timeLocation, k);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Draw 
	
	glDrawElements(GL_TRIANGLES, 6 * particlesize, GL_UNSIGNED_INT, 0);

	//reset everything back to normal for the default shader. 

	glDepthMask(GL_TRUE); // allow writes to depth buffer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shaders[shaderIndex]->disable();
	shaders[0]->enable();
	shaders[0]->setAttribute(0);

}
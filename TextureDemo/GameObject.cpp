#include "GameObject.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, std::string type) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	speed = 0.25;
	targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	
}

// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime) {
	// Update object position
	position += speed*direction * (float)deltaTime;
	
}

// Renders the GameObject using a shader
void GameObject::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f,0.3f,0.3f));
	//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 1.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); //unknown why not all same, 3:4:4 seems a good ratio though


	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

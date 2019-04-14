#include "GameObject.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, std::string t, float speed, int c){
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	
	exists = true;
	type = t;
	cost = c;
	defaultSpeed = speed;
	curSpeed = speed;
	imgScale = glm::vec3(1.0f, 1.0f, 1.0f);
	effectTimeLeft = -1;
	timeSince - 1;

	uniqueID = std::to_string(++nextUniqueID);
	
	acceleration = 0.09f;
	accelerationSlow = 0.04f;
	a = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	
}
// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime) {
	// Update object position
	if (effectTimeLeft <= 0) {
		curSpeed = defaultSpeed;
		effectTimeLeft = -1;
	}
	//update all the effects with deltatime. 
	effectTimeLeft -= deltaTime;
	burnTimeLeft -= deltaTime;
	laserCoolDownTime -= deltaTime;
	timeSince+=deltaTime;

	position += curSpeed*direction * (float)deltaTime;


}

// Renders the GameObject using a shader
void GameObject::render(std::vector<Shader*> shaders) {
	//does an fps boost based on zoom. 
	if (position.x < -1.1 / cameraZoom - cameraTranslatePos.x ||
		position.x > 1.1 / cameraZoom - cameraTranslatePos.x ||
		position.y < -0.5 / cameraZoom - cameraTranslatePos.y ||
		position.y > 1.1 / cameraZoom - cameraTranslatePos.y) {
		return; 
	}
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.3f, 0.3f)); 
	glm::mat4 imgScaleMatrix = glm::scale(glm::mat4(1.0f), imgScale); 

	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix*imgScaleMatrix;
	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
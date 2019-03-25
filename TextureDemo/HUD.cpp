#include "HUD.h"

HUD::HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, std::string type)
	: GameObject(entityPos, entityTexture, entityNumElements, type), camPos(glm::vec3(0.0f))
{
	position = entityPos;
	zoom = zoomHUD;//fix for inital zoom pas as parameter on constructur and update inmain key values
	scale = objectScale;
	flag = false;
	outSelection = NULL;

}

void HUD::addText(Text* t) { 
	t->setCamPos(camPos);
	t->setCamZoom(zoom);
	textObjects.push_back(t); 
}

void HUD::selection(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((635 <= x && x <= 667) && (472 <= y && y <= 497)) {
		outSelection = blueprints[0];
		flag = true;
		currentCursor = blueprints[0]->getIcon();

	}
	else if ((676 <= x && x <= 708) && (472 <= y && y <= 497)) {
		outSelection = blueprints[1];
		flag = true;
		currentCursor = blueprints[1]->getIcon();
	}
	else if ((716 <= x && x <= 748) && (472 <= y && y <= 497)) {
		outSelection = blueprints[2];
		flag = true;
		currentCursor = blueprints[2]->getIcon();
	}
}
void HUD::update(double deltaTime)// use it to compute the x and y coord of the mouse to detemine if its buildable
//as well as as if you are on top of a button in the HUD menu. 
{
	
	for (Text* t : textObjects) {
		t->setCamPos(camPos);
		t->setCamZoom(zoom);
		t->update(deltaTime);
	}

}
void HUD::render(Shader &shader) {

	//**********Text**********
	for (Text* t : textObjects) {
		t->render(shader);
	}
	for (int i = 0; i < blueprints.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, blueprints[i]->getIcon());
		// Setup the transformation matrix for the shader
		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
		//std::cout<< testCam.x<<std::endl;
		//glm::mat4 translationMatrix = glm::translate(glm::translate(glm::mat4(1.0f), testCam), position);
		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), blueprints[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-blueprints[i]->getPosition().x, -blueprints[i]->getPosition().y, -blueprints[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;
		//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
		shader.setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, blueprints[i]->getTex());


		shader.setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	}
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
	//std::cout<< testCam.x<<std::endl;
	//glm::mat4 translationMatrix = glm::translate(glm::translate(glm::mat4(1.0f), testCam), position);
	glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
	glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));
	glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));
	glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);


	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
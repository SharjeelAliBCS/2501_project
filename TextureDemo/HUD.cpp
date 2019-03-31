#include "HUD.h"

HUD::HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, std::string type)
	: GameObject(entityPos, entityTexture, entityNumElements, type), camPos(glm::vec3(0.0f))
{
	position = entityPos;
	zoom = zoomHUD;//fix for inital zoom pas as parameter on constructur and update inmain key values
	scale = objectScale;
	flag = false;
	enemyFlag = false;
	buttonFlag = false;
	powerUpFlag = false;
	outSelection = NULL;
	outButton = NULL;
	outEnemy = NULL;
	outPowerUP = NULL;

}

void HUD::addText(Text* t) {
	t->setCamPos(camPos);
	t->setCamZoom(zoom);
	textObjects.push_back(t);
}

void HUD::selection(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((635 <= x && x <= 667) && (472 <= y && y <= 497)) {//start first row
		outSelection = blueprints[0];
		flag = true;
		currentCursor = blueprints[0]->getIcon();
		buttonFlag = false;
	}
	else if ((676 <= x && x <= 708) && (472 <= y && y <= 497)) {
		outSelection = blueprints[1];
		flag = true;
		currentCursor = blueprints[1]->getIcon();
		buttonFlag = false;
	}
	else if ((716 <= x && x <= 748) && (472 <= y && y <= 497)) {
		outSelection = blueprints[2];
		flag = true;
		currentCursor = blueprints[2]->getIcon();
		buttonFlag = false;
	}
	else if ((757 <= x && x <= 798) && (472 <= y && y <= 497)) {
		outSelection = blueprints[3];
		flag = true;
		currentCursor = blueprints[3]->getIcon();
		buttonFlag = false;
	}
	else if ((635 <= x && x <= 667) && (502 <= y && y <= 527)) {//start of second row
		outSelection = blueprints[4];
		flag = true;
		currentCursor = blueprints[4]->getIcon();
		buttonFlag = false;
	}
	else if ((676 <= x && x <= 708) && (502 <= y && y <= 527)) {
		outSelection = blueprints[5];
		flag = true;
		currentCursor = blueprints[5]->getIcon();
		buttonFlag = false;
	}
	else if ((716 <= x && x <= 748) && (502 <= y && y <= 527)) {
		outSelection = blueprints[6];
		flag = true;
		currentCursor = blueprints[6]->getIcon();
		buttonFlag = false;
	}
	else if ((757 <= x && x <= 798) && (502 <= y && y <= 527)) {
		outSelection = blueprints[7];
		flag = true;
		currentCursor = blueprints[7]->getIcon();
		buttonFlag = false;
	}
	if ((635 <= x && x <= 667) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[0];
		buttonFlag = true;
	}
	else if ((676 <= x && x <= 708) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[1];
		buttonFlag = true;
	}
	else if ((716 <= x && x <= 748) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[2];
		buttonFlag = true;
	}
	else if ((757 <= x && x <= 798) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[3];
		buttonFlag = true;
	}
}

void HUD::selectionEnemy(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((15 <= x && x <= 50) && (472 <= y && y <= 497)) {
		outEnemy = enemyBlueprints2[3];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((55 <= x && x <= 90) && (472 <= y && y <= 497)) {
		outEnemy = enemyBlueprints2[2];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((95 <= x && x <= 130) && (472 <= y && y <= 497)) {
		outEnemy = enemyBlueprints2[1];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((135 <= x && x <= 170) && (472 <= y && y <= 497)) {
		outEnemy = enemyBlueprints2[0];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((15 <= x && x <= 50) && (500 <= y && y <= 525)) {
		outEnemy = enemyBlueprints2[7];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((55 <= x && x <= 90) && (500 <= y && y <= 525)) {
		outEnemy = enemyBlueprints2[6];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((95 <= x && x <= 130) && (500 <= y && y <= 525)) {
		outEnemy = enemyBlueprints2[5];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((135 <= x && x <= 170) && (500 <= y && y <= 525)) {
		outEnemy = enemyBlueprints2[4];
		enemyFlag = true;
		buttonFlag = false;
	}
	else if ((15 <= x && x <= 47) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[3];
		buttonFlag = true;
	}
	else if ((56 <= x && x <= 88) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[2];
		buttonFlag = true;
	}
	else if ((97 <= x && x <= 129) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[1];
		buttonFlag = true;
	}
	else if ((138 <= x && x <= 170) && (532 <= y && y <= 557)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[0];
		buttonFlag = true;
	}

}

void HUD::selectionPowerUp(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((244 <= x && x <= 256) && (408 <= y && y <= 432)) {//start first row
		outPowerUP = powerUps[0];
		powerUpFlag = true;
	}
	else if ((263 <= x && x <= 295) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[1];
		powerUpFlag = true;

	}
	else if ((302 <= x && x <= 334) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[2];
		powerUpFlag = true;

	}
	else if ((341 <= x && x <= 373) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[3];
		powerUpFlag = true;

	}
	else if ((380 <= x && x <= 412) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[4];
		powerUpFlag = true;

	}
	else if ((419 <= x && x <= 451) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[5];
		powerUpFlag = true;

	}
	else if ((458 <= x && x <= 490) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[6];
		powerUpFlag = true;

	}
	else if ((497 <= x && x <= 529) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[7];
		powerUpFlag = true;

	}
	else if ((536 <= x && x <= 568) && (408 <= y && y <= 432)) {
		outPowerUP = powerUps[8];
		powerUpFlag = true;

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
	for (int i = 0; i < powerUps.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, powerUps[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), powerUps[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-powerUps[i]->getPosition().x, -powerUps[i]->getPosition().y, -powerUps[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

		shader.setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, powerUps[i]->getTex());


		shader.setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	}
	for (int i = 0; i < buttons.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, buttons[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), buttons[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-buttons[i]->getPosition().x, -buttons[i]->getPosition().y, -buttons[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

		shader.setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, buttons[i]->getTex());


		shader.setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	}
	for (int i = 0; i < enemyBlueprints2.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, enemyBlueprints2[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), enemyBlueprints2[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-enemyBlueprints2[i]->getPosition().x, -enemyBlueprints2[i]->getPosition().y, -enemyBlueprints2[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

		shader.setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, enemyBlueprints2[i]->getTex());


		shader.setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	}
	for (int i = 0; i < blueprints.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, blueprints[i]->getIcon());
		// Setup the transformation matrix for the shader
		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
		//std::cout<< testCam.x<<std::endl;
		//glm::mat4 translationMatrix = glm::translate(glm::translate(glm::mat4(1.0f), testCam), position);
		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), blueprints[i]->getPosition());
		//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));
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
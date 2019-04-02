#include "Window.h"
#include "HUD.h"

HUD::HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, float fact, std::string type)
	: GameObject(entityPos, entityTexture, entityNumElements, type), camPos(glm::vec3(0.0f))
{
	position = entityPos;
	zoom = zoomHUD;//fix for inital zoom pas as parameter on constructur and update inmain key values
	scale = objectScale;
	factor = fact;
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

void HUD::NewCoordinates(float &x, float &y, float objecWidth, float objectHeight) {

	float movementX = 1.0f;
	float movementY = -1.0f;
	float start_x = movementX * -1.0;
	float start_y = movementY * -1.0;


	float cursor_x_pos = (x / (float)(objecWidth / 2)) - 1.0f;	//transforms cursor position to screen coordinates
	float cursor_y_pos = (y / (float)(objectHeight / 2)) - 1.0f;


	//cursor_x_pos /= zoom;
	//cursor_y_pos /= zoom;	//transforms cursor position based on screen scale. used to be const 0.2
	//cursor_x_pos -= camPos.x;
	//cursor_y_pos += camPos.y; // transform cursor position based on screen pos

	x = (int)((cursor_x_pos) / movementX);
	y = (int)((-cursor_y_pos) / movementY);

}


void HUD::selection(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((635 * factor <= x && x <= 667 * factor) && (472 * factor <= y && y <= 497 * factor)) {//start first row
		outSelection = blueprints[0];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[0]->getIcon();
	}
	else if ((676 * factor <= x && x <= 708 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outSelection = blueprints[1];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[1]->getIcon();

	}
	else if ((716 * factor <= x && x <= 748 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outSelection = blueprints[2];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[2]->getIcon();

	}
	else if ((757 * factor <= x && x <= 798 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outSelection = blueprints[3];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[3]->getIcon();

	}
	else if ((635 * factor <= x && x <= 667 * factor) && (502 * factor <= y && y <= 527 * factor)) {//start of second row
		outSelection = blueprints[4];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[4]->getIcon();

	}
	else if ((676 * factor <= x && x <= 708 * factor) && (502 * factor <= y && y <= 527 * factor)) {
		outSelection = blueprints[5];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[5]->getIcon();

	}
	else if ((716 * factor <= x && x <= 748 * factor) && (502 * factor <= y && y <= 527 * factor)) {
		outSelection = blueprints[6];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[6]->getIcon();

	}
	else if ((757 * factor <= x && x <= 798 * factor) && (502 * factor <= y && y <= 527 * factor)) {
		outSelection = blueprints[7];
		flag = true;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[7]->getIcon();

	}
	if ((635 * factor <= x && x <= 667 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[0];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
	else if ((676 * factor <= x && x <= 708 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[1];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
	else if ((716 * factor <= x && x <= 748 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[2];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
	else if ((757 * factor <= x && x <= 798 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[3];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
}

void HUD::selectionEnemy(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((15 * factor <= x && x <= 50 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outEnemy = enemyBlueprints2[3];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((55 * factor <= x && x <= 90 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outEnemy = enemyBlueprints2[2];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((95 * factor <= x && x <= 130 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outEnemy = enemyBlueprints2[1];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((135 * factor <= x && x <= 170 * factor) && (472 * factor <= y && y <= 497 * factor)) {
		outEnemy = enemyBlueprints2[0];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((15 * factor <= x && x <= 50 * factor) && (500 * factor <= y && y <= 525 * factor)) {
		outEnemy = enemyBlueprints2[7];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((55 * factor <= x && x <= 90 * factor) && (500 * factor <= y && y <= 525 * factor)) {
		outEnemy = enemyBlueprints2[6];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((95 * factor <= x && x <= 130 * factor) && (500 * factor <= y && y <= 525 * factor)) {
		outEnemy = enemyBlueprints2[5];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((135 * factor <= x && x <= 170 * factor) && (500 * factor <= y && y <= 525 * factor)) {
		outEnemy = enemyBlueprints2[4];
		flag = false;
		enemyFlag = true;
		buttonFlag = false;
		powerUpFlag = false;
	}
	else if ((15 * factor <= x && x <= 47 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[3];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
	else if ((56 * factor <= x && x <= 88 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[2];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
	else if ((97 * factor <= x && x <= 129 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[1];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}
	else if ((138 * factor <= x && x <= 170 * factor) && (532 * factor <= y && y <= 557 * factor)) {//=========================================================forom here down its just to show its selecting the button/demontration
		outButton = buttons[0];
		flag = false;
		enemyFlag = false;
		buttonFlag = true;
		powerUpFlag = false;
	}

}

void HUD::selectionPowerUp(double x, double y) {
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((244 * factor <= x && x <= 256 * factor) && (408 * factor <= y && y <= 432 * factor)) {//start first row
		outPowerUP = powerUps[0];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;
	}
	else if ((263 * factor <= x && x <= 295 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[1];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((302 * factor <= x && x <= 334 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[2];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((341 * factor <= x && x <= 373 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[3];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((380 * factor <= x && x <= 412 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[4];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((419 * factor <= x && x <= 451 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[5];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((458 * factor <= x && x <= 490 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[6];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((497 * factor <= x && x <= 529 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[7];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
		powerUpFlag = true;

	}
	else if ((536 * factor <= x && x <= 568 * factor) && (408 * factor <= y && y <= 432 * factor)) {
		outPowerUP = powerUps[8];
		flag = false;
		enemyFlag = false;
		buttonFlag = false;
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
void HUD::render(std::vector<Shader*> shaders) {

	//**********Text**********
	for (Text* t : textObjects) {
		t->render(shaders);
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

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, powerUps[i]->getTex());


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
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

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, buttons[i]->getTex());


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
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

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, enemyBlueprints2[i]->getTex());


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
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
		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, blueprints[i]->getTex());


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
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
	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
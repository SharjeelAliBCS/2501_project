#include "Window.h"
#include "HUD.h"

HUD::HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, float fact, std::string type, GLFWwindow* window)
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
	win = window;

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


	x = (int)((cursor_x_pos) / movementX);
	y = (int)((-cursor_y_pos) / movementY);

}


void HUD::selection(double x, double y) {

	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((677.6 * factor <= x && x <= 710.4 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {//start first row
		outSelection = blueprints[0];
		flag = true;
		currentCursor = blueprints[0]->getIcon();
	}
	else if ((667.6 * factor <= x && x <= 710.4 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outSelection = blueprints[1];
		flag = true;
		currentCursor = blueprints[1]->getIcon();
	}
	else if ((667.6 * factor <= x && x <= 710.4 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outSelection = blueprints[2];
		flag = true;
		currentCursor = blueprints[2]->getIcon();
	}
	//==============================next column
	else if ((716.8 * factor <= x && x <= 749.44 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outSelection = blueprints[3];
		flag = true;
		currentCursor = blueprints[3]->getIcon();
	}
	else if ((716.8 * factor <= x && x <= 749.44 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outSelection = blueprints[4];
		flag = true;
		currentCursor = blueprints[4]->getIcon();
	}
	else if ((716.8 * factor <= x && x <= 749.44 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outSelection = blueprints[5];
		flag = true;
		currentCursor = blueprints[5]->getIcon();
	}
	//==============================next column
	else if ((756 * factor <= x && x <= 788.8 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outSelection = blueprints[6];
		flag = true;
		currentCursor = blueprints[6]->getIcon();
	}
	else if ((756 * factor <= x && x <= 788.8 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outSelection = blueprints[7];
		flag = true;
		currentCursor = blueprints[7]->getIcon();
	}
	else if ((756 * factor <= x && x <= 788.8 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outSelection = blueprints[8];
		flag = true;
		currentCursor = blueprints[8]->getIcon();
	}

}

void HUD::selectionEnemy(double x, double y) {
	//std::cout << x / factor << "," << y / factor << std::endl;
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((9.6 * factor <= x && x <= 42.4 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outEnemy = enemyBlueprints2[0];
	}
	else if ((9.6 * factor <= x && x <= 42.4 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outEnemy = enemyBlueprints2[1];
	}
	else if ((9.6 * factor <= x && x <= 42.4 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outEnemy = enemyBlueprints2[2];
	}
	//====================================new column
	else if ((50.4 * factor <= x && x <= 80 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outEnemy = enemyBlueprints2[3];
	}
	else if ((50.4 * factor <= x && x <= 80 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outEnemy = enemyBlueprints2[4];
	}
	else if ((50.4 * factor <= x && x <= 80 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outEnemy = enemyBlueprints2[5];
	}
	//====================================new column
	else if ((89 * factor <= x && x <= 121.6 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outEnemy = enemyBlueprints2[6];
	}
	else if ((89 * factor <= x && x <= 121.6 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outEnemy = enemyBlueprints2[7];
	}
	else if ((89 * factor <= x && x <= 121.6 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outEnemy = enemyBlueprints2[8];
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

std::string HUD::turns(double x, double y) {
	if ((427.2 * factor <= x && x <= 455.2 * factor) && (406.4 * factor <= y && y <= 429.6 * factor))
	{
		return "turn";
	}
	if ((347.2 * factor <= x && x <= 373 * factor) && (406.4 * factor <= y && y <= 429.6 * factor))
	{
		return "wave";
	}
	return "";
}

void HUD::update(double deltaTime)// use it to compute the x and y coord of the mouse to detemine if its buildable
								  //as well as as if you are on top of a button in the HUD menu. 
{
	for (Text* t : textObjects) {
		t->setCamPos(camPos);
		t->setCamZoom(zoom);
	}
}
bool HUD::updateHotkeysTower() {
	float timeOfLastMove = 0.0f;
	if (glfwGetKey(win, GLFW_KEY_U) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[0];
		flag = true;
		currentCursor = blueprints[0]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[1];
		flag = true;
		currentCursor = blueprints[1]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[2];
		flag = true;
		currentCursor = blueprints[2]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_I) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[3];
		flag = true;
		currentCursor = blueprints[3]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_J) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[4];
		flag = true;
		currentCursor = blueprints[4]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[5];
		flag = true;
		currentCursor = blueprints[5]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_O) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[6];
		flag = true;
		currentCursor = blueprints[6]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_K) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[7];
		flag = true;
		currentCursor = blueprints[7]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[8];
		flag = true;
		currentCursor = blueprints[8]->getIcon();
		return true;
	}
	return false;
}

bool HUD::updateHotkeysEnemy() {

	float timeOfLastMove = 0.0f;
	if (timeOfLastMove + 0.05 < glfwGetTime())
	{
		if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[0];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[1];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[2];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[3];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[4];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[5];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[6];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[7];
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[8];
			return true;
		}
		//==============================================
	}
	return false;
}

void HUD::counter(std::vector<EnemyObject*> l1, std::vector<EnemyObject*> l2)// use it to compute the x and y coord of the mouse to detemine if its buildabl					  //as well as as if you are on top of a button in the HUD menu. 
{
	int normal = 0, fast = 0, hearty = 0, flying = 0, splitter = 0, regenerating = 0, fasthearty = 0, fastflying = 0, fastregenerating = 0;
	int normal2 = 0, fast2 = 0, hearty2 = 0, flying2 = 0, splitter2 = 0, regenerating2 = 0, fasthearty2 = 0, fastflying2 = 0, fastregenerating2 = 0;

	for (EnemyObject* g : l1) {
		if (g->getType().compare("normal") == 0) {
			normal += 1;
		}
		if (g->getType().compare("fast") == 0) {
			fast += 1;
		}
		if (g->getType().compare("hearty") == 0) {
			hearty += 1;
		}
		if (g->getType().compare("flying") == 0) {
			flying += 1;
		}
		if (g->getType().compare("splitter") == 0) {
			splitter += 1;
		}
		if (g->getType().compare("regenerating") == 0) {
			regenerating += 1;
		}
		if (g->getType().compare("fasthearty") == 0) {
			fasthearty += 1;
		}
		if (g->getType().compare("fastflying") == 0) {
			fastflying += 1;
		}
		if (g->getType().compare("fastregenerating") == 0) {
			fastregenerating += 1;
		}
	}
	for (EnemyObject* g : l2) {
		if (g->getType().compare("normal") == 0) {
			normal2 += 1;
		}
		if (g->getType().compare("fast") == 0) {
			fast2 += 1;
		}
		if (g->getType().compare("hearty") == 0) {
			hearty2 += 1;
		}
		if (g->getType().compare("flying") == 0) {
			flying2 += 1;
		}
		if (g->getType().compare("splitter") == 0) {
			splitter2 += 1;
		}
		if (g->getType().compare("regenerating") == 0) {
			regenerating2 += 1;
		}
		if (g->getType().compare("fasthearty") == 0) {
			fasthearty2 += 1;
		}
		if (g->getType().compare("fastflying") == 0) {
			fastflying2 += 1;
		}
		if (g->getType().compare("fastregenerating") == 0) {
			fastregenerating2 += 1;
		}
	}
	std::vector<int> out;
	out.clear();
	out.push_back(normal);//0
	out.push_back(fast);//1
	out.push_back(hearty);//2
	out.push_back(flying);//3
	out.push_back(splitter);//4
	out.push_back(regenerating);//5
	out.push_back(fasthearty);//6
	out.push_back(fastflying);//7
	out.push_back(fastregenerating);//8
	out.push_back(normal2);//9
	out.push_back(fast2);//10
	out.push_back(hearty2);//11
	out.push_back(flying2);//12
	out.push_back(splitter2);//13
	out.push_back(regenerating2);//14
	out.push_back(fasthearty2);//15
	out.push_back(fastflying2);//16
	out.push_back(fastregenerating2);//17

	for (Text* t : textObjects) {
		if (t->getID().compare("P1normal") == 0) {
			std::string temp = std::to_string(out[0]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1fast") == 0) {
			std::string temp = std::to_string(out[1]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1hearty") == 0) {
			std::string temp = std::to_string(out[2]);
			t->setRenderedText(temp);
		}

		if (t->getID().compare("P1flying") == 0) {
			std::string temp = std::to_string(out[3]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1splitter") == 0) {
			std::string temp = std::to_string(out[4]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1regenerating") == 0) {
			std::string temp = std::to_string(out[5]);
			t->setRenderedText(temp);
		}

		if (t->getID().compare("P1fasthearty") == 0) {
			std::string temp = std::to_string(out[6]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1fastflying") == 0) {
			std::string temp = std::to_string(out[7]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1fastregenerating") == 0) {
			std::string temp = std::to_string(out[8]);
			t->setRenderedText(temp);
		}
		//=====================
		if (t->getID().compare("P2normal") == 0) {
			std::string temp = std::to_string(out[9]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2fast") == 0) {
			std::string temp = std::to_string(out[10]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2hearty") == 0) {
			std::string temp = std::to_string(out[11]);
			t->setRenderedText(temp);
		}

		if (t->getID().compare("P2flying") == 0) {
			std::string temp = std::to_string(out[12]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2splitter") == 0) {
			std::string temp = std::to_string(out[13]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2regenerating") == 0) {
			std::string temp = std::to_string(out[14]);
			t->setRenderedText(temp);
		}

		if (t->getID().compare("P2fasthearty") == 0) {
			std::string temp = std::to_string(out[15]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2fastflying") == 0) {
			std::string temp = std::to_string(out[16]);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2fastregenerating") == 0) {
			std::string temp = std::to_string(out[17]);
			t->setRenderedText(temp);
		}
	}
}

void HUD::updatePlayers(long crd1, long in1, int hp1, long crd2, long in2, int hp2)
{
	for (Text* t : textObjects) {
		if (t->getID().compare("P1credits") == 0) {
			std::string temp = std::to_string(crd1);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1income") == 0) {
			std::string temp = std::to_string(in1);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P1hp") == 0) {
			std::string temp = std::to_string(hp1) + "/20";
			t->setRenderedText(temp);
		}

		if (t->getID().compare("P2credits") == 0) {
			std::string temp = std::to_string(crd2);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2income") == 0) {
			std::string temp = std::to_string(in2);
			t->setRenderedText(temp);
		}
		if (t->getID().compare("P2hp") == 0) {
			std::string temp = std::to_string(hp2) + "/20";
			t->setRenderedText(temp);
		}
	}
}

void HUD::render(std::vector<Shader*> shaders) {

	//**********Text**********
	for (HUD* o : generalList) {
		o->render(shaders);
	}
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
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
	upgradeFlag = false;
	powerUpFlag = false;
	outSelection = NULL;
	outEnemy = NULL;
	outPowerUP = NULL;
	win = window;
	toggleItemInfo = true;
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
	//choose a tower based on mouse x, and y coordinates. 
	//top left
	if ((677.6 * factor <= x && x <= 710.4 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {//start first row
		outSelection = blueprints[0];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[0]->getIcon();
	}
	
	else if ((667.6 * factor <= x && x <= 710.4 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outSelection = blueprints[3];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[3]->getIcon();
		
	}
	else if ((667.6 * factor <= x && x <= 710.4 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outSelection = blueprints[6];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[6]->getIcon();
	
	}
	//==============================next column
	else if ((716.8 * factor <= x && x <= 749.44 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outSelection = blueprints[1];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[1]->getIcon();
		
	}
	else if ((716.8 * factor <= x && x <= 749.44 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outSelection = blueprints[4];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[4]->getIcon();
	
	}
	else if ((716.8 * factor <= x && x <= 749.44 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outSelection = blueprints[7];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[7]->getIcon();
		
	}
	//==============================next column
	else if ((756 * factor <= x && x <= 788.8 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outSelection = blueprints[2];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[2]->getIcon();
		
	}
	else if ((756 * factor <= x && x <= 788.8 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outSelection = blueprints[5];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[5]->getIcon();
		
	}
	
	else if ((756 * factor <= x && x <= 788.8 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		
		outSelection = blueprints[8];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[8]->getIcon();

	
	}

}

void HUD::selectionEnemy(double x, double y) {
	
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	if ((9.6 * factor <= x && x <= 42.4 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outEnemy = enemyBlueprints2[0];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;
	}
	else if ((9.6 * factor <= x && x <= 42.4 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outEnemy = enemyBlueprints2[3];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;

	}
	else if ((9.6 * factor <= x && x <= 42.4 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outEnemy = enemyBlueprints2[6];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;
	}
	//====================================new column
	else if ((50.4 * factor <= x && x <= 80 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outEnemy = enemyBlueprints2[1];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;

	}
	else if ((50.4 * factor <= x && x <= 80 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outEnemy = enemyBlueprints2[4];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;

	}
	else if ((50.4 * factor <= x && x <= 80 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outEnemy = enemyBlueprints2[7];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;
	}
	//====================================new column
	else if ((89 * factor <= x && x <= 121.6 * factor) && (506.4 * factor <= y && y <= 531.2 * factor)) {
		outEnemy = enemyBlueprints2[2];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;
	}
	else if ((89 * factor <= x && x <= 121.6 * factor) && (536 * factor <= y && y <= 560.8 * factor)) {
		outEnemy = enemyBlueprints2[5];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;
	}
	else if ((89 * factor <= x && x <= 121.6 * factor) && (565.6 * factor <= y && y <= 590.4 * factor)) {
		outEnemy = enemyBlueprints2[8];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = true;
		upgradeFlag = false;
		powerUpFlag = false;
	}

}

void HUD::selectionPowerUp(double x, double y) {
	
	// -- posibility of going in to transformations a nd just get back the coordinates in hud coordinate and not in mouse coordiantes
	
	if ((266.4 * factor <= x && x <= 294.4 * factor) && (466.4 * factor <= y && y <= 490.4 * factor)) {//start first row
		outPowerUP = powerUps[0];
		outEnemy = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = true;
	}else if ((326.4* factor <= x && x <= 353.6 * factor) && (466.4 * factor <= y && y <= 490.4 * factor)) {//start first row
		outPowerUP = powerUps[1];
		outEnemy = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = true;
	}
	else if ((388* factor <= x && x <= 415.2 * factor) && (466.4 * factor <= y && y <= 490.4 * factor)) {//start first row
		outPowerUP = powerUps[2];
		outEnemy = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = true;
	}
	else if ((446.4* factor <= x && x <= 474 * factor) && (466.4 * factor <= y && y <= 490.4 * factor)) {//start first row
		outPowerUP = powerUps[3];
		outEnemy = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = true;
	}
	else if ((504.8* factor <= x && x <= 535.2 * factor) && (466.4 * factor <= y && y <= 490.4 * factor)) {//start first row
		outPowerUP = powerUps[4];
		outEnemy = NULL;
		outUpgrade = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = true;
	}
	
}

void HUD::selectionUpgrades(double x, double y) {
	
	if ((628 * factor <= x && x <= 656 * factor) && (505.6 * factor <= y && y <= 526.4 * factor)) {//start first row
		outUpgrade = upgrades[0];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
	}
	else if ((628 * factor <= x && x <= 656 * factor) && (536 * factor <= y && y <= 556 * factor)) {//start first row
		outUpgrade = upgrades[1];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
	}
	else if ((628 * factor <= x && x <= 656 * factor) && (567.2 * factor <= y && y <= 587.2 * factor)) {//start first row
		outUpgrade = upgrades[2];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
	}
	else if ((147.2 * factor <= x && x <= 174.4 * factor) && (505.6 * factor <= y && y <= 526.4 * factor)) {//start first row
		outUpgrade = upgrades[3];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
	}
	else if ((147.2 * factor <= x && x <= 174.4 * factor) && (536 * factor <= y && y <= 556 * factor)) {//start first row
		outUpgrade = upgrades[4];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
	}
	else if ((147.2 * factor <= x && x <= 174.4 * factor) && (567.2 * factor <= y && y <= 587.2 * factor)) {//start first row
		outUpgrade = upgrades[5];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
	}

}

std::string HUD::turns(double x, double y) {
	if ((404.8 * factor <= x && x <= 456.8 * factor) && (383.2 * factor <= y && y <= 444.8 * factor))
	{
		return "turn";
	}
	if ((344 * factor <= x && x <= 396 * factor) && (383.2 * factor <= y && y <= 444.8 * factor))
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
	//updates the hot keys based on key pressed. 
	if (glfwGetKey(win, GLFW_KEY_U) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[0];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[0]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_I) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[1];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[1]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_O) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[2];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[2]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_J) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[3];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[3]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_K) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[4];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[4]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[5];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[5]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[6];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[6]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[7];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[7]->getIcon();
		return true;
	}
	if (glfwGetKey(win, GLFW_KEY_COMMA) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outSelection = blueprints[8];
		outPowerUP = NULL;
		outUpgrade = NULL;
		outEnemy = NULL;
		flag = true;
		enemyFlag = false;
		upgradeFlag = false;
		powerUpFlag = false;
		currentCursor = blueprints[8]->getIcon();
		return true;
	}
	return false;
}

bool HUD::updateHotkeysEnemy() {
	//updates the hot keys based on key pressed. 
	float timeOfLastMove = 0.0f;
	if (timeOfLastMove + 0.05 < glfwGetTime())
	{
		if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[0];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[1];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[2];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[3];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[4];

			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[5];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[6];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[7];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}
		if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
		{
			outEnemy = enemyBlueprints2[8];
			outPowerUP = NULL;
			outUpgrade = NULL;
			outSelection = NULL;
			flag = false;
			enemyFlag = true;
			upgradeFlag = false;
			powerUpFlag = false;
			return true;
		}

		//==============================================
	}
	return false;
}

bool HUD::updateHotkeysUpgrade() {
	float timeOfLastMove = 0.0f;
	//updates the hot keys based on key pressed. 
	if (glfwGetKey(win, GLFW_KEY_Y) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outUpgrade = upgrades[0];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
		return true;
	}
	else if (glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outUpgrade = upgrades[1];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
		return true;
	}
	else if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outUpgrade = upgrades[2];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
		return true;
	}
	else if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outUpgrade = upgrades[3];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
		return true;
	}
	else if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outUpgrade = upgrades[4];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
		return true;
	}
	else if (glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
	{
		outUpgrade = upgrades[5];
		outEnemy = NULL;
		outPowerUP = NULL;
		outSelection = NULL;
		flag = false;
		enemyFlag = false;
		upgradeFlag = true;
		powerUpFlag = false;
		return true;
	}


	return false;
}

void HUD::counter(std::vector<EnemyObject*> l1, std::vector<EnemyObject*> l2)// use it to compute the x and y coord of the mouse to detemine if its buildabl					  //as well as as if you are on top of a button in the HUD menu. 
{
	int normal = 0, fast = 0, hearty = 0, flying = 0, splitter = 0, regenerating = 0, fasthearty = 0, fastflying = 0, fastregenerating = 0;
	int normal2 = 0, fast2 = 0, hearty2 = 0, flying2 = 0, splitter2 = 0, regenerating2 = 0, fasthearty2 = 0, fastflying2 = 0, fastregenerating2 = 0;

	for (EnemyObject* g : l1) {
		if (g->getType().compare("Servent Minor") == 0) {
			normal += 1;
			
		}
		if (g->getType().compare("Glutton Minor") == 0) {
			fast += 1;
		}
		if (g->getType().compare("Fleeting Minor") == 0) {
			hearty += 1;
		}
		if (g->getType().compare("Lifegiver Minor") == 0) {
			flying += 1;
		
		}
		if (g->getType().compare("Fleeting Colossal") == 0) {
			splitter += 1;
		}
		if (g->getType().compare("Life Colossal") == 0) {
			regenerating += 1;
		}
		if (g->getType().compare("Fleeting Lifeblood") == 0) {
			fasthearty += 1;
		}
		if (g->getType().compare("Absolute Zenith") == 0) {
			fastflying += 1;
		}
		if (g->getType().compare("Undying Zenith") == 0) {
			fastregenerating += 1;
		}
	}
	for (EnemyObject* g : l2) {
		if (g->getType().compare("Servent Minor") == 0) {
			normal2 += 1;
		}
		if (g->getType().compare("Glutton Minor") == 0) {
			fast2 += 1;
		}
		if (g->getType().compare("Fleeting Minor") == 0) {
			hearty2 += 1;
		}
		if (g->getType().compare("Lifegiver Minor") == 0) {
			flying2 += 1;
		}
		if (g->getType().compare("Fleeting Colossal") == 0) {
			splitter2 += 1;
		}
		if (g->getType().compare("Life Colossal") == 0) {
			regenerating2 += 1;
		}
		if (g->getType().compare("Fleeting Lifeblood") == 0) {
			fasthearty2 += 1;
		}
		if (g->getType().compare("Absolute Zenith") == 0) {
			fastflying2 += 1;
		}
		if (g->getType().compare("Undying Zenith") == 0) {
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

void HUD::detailRender(std::vector<Shader*> shaders) {
	//toggleiteminfo is deafult true, so it will display stats. 
	if (toggleItemInfo) {
		if (upgradeFlag) {

			textObjects[0]->setRenderedText(textObjects[0]->getText() + outUpgrade->getType());//name
			textObjects[1]->setRenderedText(textObjects[1]->getText() + std::to_string((int)outUpgrade->getCost()));//cost
			textObjects[2]->setRenderedText(textObjects[2]->getText() + outUpgrade->getHotKey());//hotkey
			textObjects[11]->setRenderedText(textObjects[11]->getText() + std::to_string((int)(outUpgrade->getMod()*100-100)) + "%");//mod



			textObjects[0]->render(shaders);
			textObjects[1]->render(shaders);
			textObjects[2]->render(shaders);
			textObjects[11]->render(shaders);


			glBindTexture(GL_TEXTURE_2D, outUpgrade->getTex());

			glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

			glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
			glm::mat4 posMat = glm::translate(glm::mat4(1.0f), outUpgrade->getPosition());
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.225f, 0.0f));//scale the tower
			glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-outUpgrade->getPosition().x, -outUpgrade->getPosition().y, -outUpgrade->getPosition().z));
			glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

			glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

			// Draw the entity
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, outUpgrade->getTex());


			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		}
		if (powerUpFlag) {
			textObjects[0]->setRenderedText(textObjects[0]->getText() + outPowerUP->getType());//name
			textObjects[1]->setRenderedText(textObjects[1]->getText() + std::to_string(outPowerUP->getCost()));//cost
			textObjects[2]->setRenderedText(textObjects[2]->getText() + outPowerUP->getHotKey());//hotkey
			if (outPowerUP->getRange() == -1) {
				textObjects[9]->setRenderedText(textObjects[9]->getText() + "Global");//range
			}
			textObjects[10]->setRenderedText(textObjects[10]->getText() + round(outPowerUP->getDur(), 2));//duration


			textObjects[0]->render(shaders);
			textObjects[1]->render(shaders);
			textObjects[2]->render(shaders);
			textObjects[9]->render(shaders);
			textObjects[10]->render(shaders);

			glBindTexture(GL_TEXTURE_2D, outPowerUP->getTex());

			glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

			glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
			glm::mat4 posMat = glm::translate(glm::mat4(1.0f), outPowerUP->getPosition());
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.225f, 0.0f));//scale the tower
			glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-outPowerUP->getPosition().x, -outPowerUP->getPosition().y, -outPowerUP->getPosition().z));
			glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

			glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

			// Draw the entity
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, outPowerUP->getTex());


			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		}
		if (flag) {
			
			textObjects[0]->setRenderedText(textObjects[0]->getText() + outSelection->getType());//name
			textObjects[1]->setRenderedText(textObjects[1]->getText() + std::to_string((int)outSelection->getCost()));//cost
			textObjects[2]->setRenderedText(textObjects[2]->getText() + outSelection->getHotKey());//hotkey
			textObjects[3]->setRenderedText(textObjects[3]->getText() + round(outSelection->getDamage()*upmod["Upgrade Damage"], 1));//damage
			textObjects[4]->setRenderedText(textObjects[4]->getText() + round(std::fmax(outSelection->getROF()*(2-upmod["Upgrade Rate of Fire"]),0.05),3));//ROF
			textObjects[5]->setRenderedText(textObjects[5]->getText() + round(std::fmin(outSelection->getRange()*upmod["Upgrade Range"], outSelection->getRange()*2),2));//rangestd::fmax(baseROF *(2-mod),0.05)

			textObjects[0]->render(shaders);
			textObjects[1]->render(shaders);
			textObjects[2]->render(shaders);
			textObjects[3]->render(shaders);
			textObjects[4]->render(shaders);
			textObjects[5]->render(shaders);
			glBindTexture(GL_TEXTURE_2D, outSelection->getIcon());

			glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

			glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
			glm::mat4 posMat = glm::translate(glm::mat4(1.0f), outSelection->getPosition());
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.225f, 0.0f));//scale the tower
			glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-outSelection->getPosition().x, -outSelection->getPosition().y, -outSelection->getPosition().z));
			glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

			glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

			// Draw the entity
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, outSelection->getIcon());


			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		}

		if (enemyFlag) {

			textObjects[0]->setRenderedText(textObjects[0]->getText() + outEnemy->getType());//name
			textObjects[1]->setRenderedText(textObjects[1]->getText() + std::to_string((int)(outEnemy->getCost()*upmod["Increase Cost"])));//cost
			textObjects[2]->setRenderedText(textObjects[2]->getText() + outEnemy->getHotKey());//hotkey
			textObjects[6]->setRenderedText(textObjects[6]->getText() + std::to_string((int)(outEnemy->getHealth()*upmod["Upgrade Hardiness"])));//hp
			textObjects[7]->setRenderedText(textObjects[7]->getText() + round(std::fmin(2.5, outEnemy->getCurSpeed()*upmod["Upgrade Speed"]), 2));//speed
			textObjects[8]->setRenderedText(textObjects[8]->getText() + round(outEnemy->getRegen()*upmod["Upgrade Hardiness"], 2));//range

			textObjects[0]->render(shaders);
			textObjects[1]->render(shaders);
			textObjects[2]->render(shaders);
			textObjects[6]->render(shaders);
			textObjects[7]->render(shaders);
			textObjects[8]->render(shaders);
			glBindTexture(GL_TEXTURE_2D, outEnemy->getTex());

			glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

			glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
			glm::mat4 posMat = glm::translate(glm::mat4(1.0f), outEnemy->getPosition());
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.225f, 0.225f, 0.0f));//scale the tower
			glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-outEnemy->getPosition().x, -outEnemy->getPosition().y, -outEnemy->getPosition().z));
			glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

			glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

			// Draw the entity
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

			glBindTexture(GL_TEXTURE_2D, outEnemy->getTex());


			shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
			glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
		}
	}
	//this outputs the descripitions. 
	else if (flag || enemyFlag || upgradeFlag || powerUpFlag) {
	
		if (upgradeFlag) {

			textObjects[0]->setRenderedText(textObjects[0]->getText() + outUpgrade->getType());//name
			textObjects[12]->setRenderedText(outUpgrade->getDescription());//discription
		}
		else if (powerUpFlag) {
			textObjects[0]->setRenderedText(textObjects[0]->getText() + outPowerUP->getType());//name
			textObjects[12]->setRenderedText(outPowerUP->getDescription());//discription
			
		}

		else if (flag) {
			textObjects[0]->setRenderedText(textObjects[0]->getText() + outSelection->getType());//name
			textObjects[12]->setRenderedText(outSelection->getDescription());//discription
		}
		else if (enemyFlag) {
			textObjects[0]->setRenderedText(textObjects[0]->getText() + outEnemy->getType());//name
			textObjects[12]->setRenderedText(outEnemy->getDescription());//discription
		}

		textObjects[0]->render(shaders);
		textObjects[12]->render(shaders);


	}
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

	glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
	glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));
	glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));
	glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;

	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	
}

void HUD::render(std::vector<Shader*> shaders) {

	//**********Text**********
	for (HUD* o : generalList) {
		o->render(shaders);
	}
	for (Text* t : textObjects) {
		t->render(shaders);
	}
	for (int i = 0; i < upgrades.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, upgrades[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), upgrades[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-upgrades[i]->getPosition().x, -upgrades[i]->getPosition().y, -upgrades[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, upgrades[i]->getTex());


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
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

	for (int i = 0; i < gameObjectList.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, gameObjectList[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), gameObjectList[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-gameObjectList[i]->getPosition().x, -gameObjectList[i]->getPosition().y, -gameObjectList[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, gameObjectList[i]->getTex());


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

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), blueprints[i]->getPosition());
		
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.0f));
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-blueprints[i]->getPosition().x, -blueprints[i]->getPosition().y, -blueprints[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;
	
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
	glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
	glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));
	glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));
	glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;
	
	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void HUD::newRender(std::vector<Shader*> shaders) {
	for (Text* t : textObjects) {
		t->render(shaders);
	}

	for (int i = 0; i < enemyBlueprints2.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, enemyBlueprints2[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), enemyBlueprints2[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.08f, 0.08f, 0.0f));//scale the tower
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
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
	glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
	glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));
	glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));
	glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;
	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void HUD::buttonRender(std::vector<Shader*> shaders) {


	for (int i = 0; i < gameObjectList.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, gameObjectList[i]->getTex());

		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);

		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), gameObjectList[i]->getPosition());
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.22f, 0.0f));//scale the tower
		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-gameObjectList[i]->getPosition().x, -gameObjectList[i]->getPosition().y, -gameObjectList[i]->getPosition().z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));//scale of the hud with zoom

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix *oMatrix;

		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, gameObjectList[i]->getTex());


		shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	}

	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
	glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
	glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, -position.z));
	glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));
	glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* oMatrix;
	
	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture);


	shaders[0]->setUniformMat4("transformationMatrix", transformationMatrix);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}


std::string HUD::round(float num, int places) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(places) << num;
	return ss.str(); 
}
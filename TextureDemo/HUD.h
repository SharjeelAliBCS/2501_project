#pragma once
#include "Shader.h"
#include "Window.h"
#include "GameObject.h"
#include "Text.h"
#include "TowerObject.h"
#include "PowerUpObject.h"
#include "UpgradeObject.h"
#include <iomanip>


class HUD : public GameObject {
public:
	//constructor
	HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, float fact, std::string type, GLFWwindow* window);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void selection(double x, double y);//Used to select towers
	virtual void selectionEnemy(double x, double y);//Used to select enemies
	virtual void selectionPowerUp(double x, double y);//used to select power ups
	virtual void NewCoordinates(float&x, float&y, float objecWidth, float objectHeight);
	virtual void counter(std::vector<EnemyObject*> l, std::vector<EnemyObject*> l2);
	virtual void updatePlayers(long crd1, long in1, int hp1, long crd2, long in2, int hp2);
	virtual bool updateHotkeysEnemy();
	virtual bool updateHotkeysTower();
	virtual bool updateHotkeysUpgrade();
	virtual void selectionUpgrades(double x, double y);
	//virtual bool updateHotkeysPowerUp();

	virtual std::string turns(double x, double y);
	virtual void detailRender(std::vector<Shader*> shaders);

	// Renders the GameObject using a shader
	virtual void  render(std::vector<Shader*> shaders) override;

	void addText(Text* t);
	// Getters
	inline glm::vec3 getCamPos() { return camPos; }
	inline float getZoom() { return zoom; }
	inline std::vector<Text*> getTextObjects() { return textObjects; }
	inline bool getFlag() { return flag; }
	inline bool getPowerUpFlag() { return powerUpFlag; }
	inline bool getEnemyFlag() { return enemyFlag; }
	inline bool getUpgradeFlag() { return upgradeFlag; }
	inline TowerObject* getSelection() { return outSelection; }
	inline EnemyObject* getSelectionEnemy() { return outEnemy; }
	inline UpgradeObject* getSelectionUpgrades() { return outUpgrade; }
	inline PowerUpObject* getSelectionPowerUps() { return outPowerUP; }
	inline GLuint getCursor() { return currentCursor; }

	// Setters
	inline void setCamPos(glm::vec3 newPosition) { camPos = newPosition; }
	inline void setScale(glm::vec3 newScale) { camPos = newScale; }
	inline void setZoom(float newZoom) { zoom = newZoom; }
	inline void setFactor(float newFact) { factor = newFact; }
	//flags
	inline void setFlag(bool newFlag) { flag = newFlag; }
	inline void setUpgradeFlag(bool newFlag) {upgradeFlag = newFlag; }
	inline void setPowerUpFlag(bool power) { powerUpFlag = power; }
	inline void setEnemyFlag(bool newEnemyFlag) { enemyFlag = newEnemyFlag; }
	//
	inline void setUpgrades(std::vector<UpgradeObject*> up) { upgrades = up; }
	inline void setPowerUPs(std::vector<PowerUpObject*> power) { powerUps = power; }
	inline void setBlueprints(std::vector<TowerObject*> b) { blueprints = b; }
	inline void setEnemyBlueprints(std::vector<EnemyObject*> e) { enemyBlueprints2 = e; }
	inline void setGeneralList(std::vector<HUD*> list) { generalList = list; }
	inline void setGameObjects(std::vector<GameObject*> g) { gameObjectList = g; }
	//outs
	inline void setSelectionEnemy(EnemyObject* newEnemy) { outEnemy = newEnemy; }
	inline void setSelectionPowerUps(PowerUpObject* power) { outPowerUP = power; }
	inline void setSelectionUpgrades(UpgradeObject* up) { outUpgrade = up; }

	inline void setUpmod(std::map<std::string, float> u) { upmod = u; }
	inline void setToggleInfo() { toggleItemInfo = !toggleItemInfo; }
private:
	float zoom;
	glm::vec3 camPos;
	glm::vec3 scale;
	float factor;
	GLFWwindow* win;
	bool flag;//only important flag becuase its flag for selection of towers /////the other flags are just for the selection square demonstration
	bool powerUpFlag;
	bool upgradeFlag;
	bool enemyFlag;
	bool toggleItemInfo;
	std::vector<int> out;
	//vectors storing all the items in the game
	std::vector<TowerObject*> blueprints;
	std::vector<UpgradeObject*> upgrades;
	std::vector<GameObject*> gameObjectList;
	std::vector<PowerUpObject*> powerUps;
	std::vector<EnemyObject*> enemyBlueprints2;
	std::vector<Text*> textObjects;
	std::vector<HUD*> generalList;
	//These are used to store the current selected item.
	//GameObject* outGameObject;
	PowerUpObject* outPowerUP;
	UpgradeObject* outUpgrade;
	EnemyObject* outEnemy;
	TowerObject* outSelection;
	GLuint currentCursor;

	std::map<std::string, float> upmod;

	std::string round(float num, int places);


};

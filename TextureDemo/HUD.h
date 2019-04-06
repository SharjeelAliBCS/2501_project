#pragma once
#include "Shader.h"
#include "Window.h"
#include "GameObject.h"
#include "Text.h"
#include "TowerObject.h"

class HUD : public GameObject {
public:
	//constructor
	HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, float fact, std::string type, GLFWwindow* win);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void selection(double x, double y);//Used to select towers
	virtual void selectionEnemy(double x, double y);//Used to select enemies
	virtual void selectionPowerUp(double x, double y);//used to select power ups
	virtual void NewCoordinates(float&x, float&y, float objecWidth, float objectHeight);
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
	inline bool getButtonFlag() { return buttonFlag; }
	inline TowerObject* getSelection() { return outSelection; }
	inline EnemyObject* getSelectionEnemy() { return outEnemy; }
	inline GameObject* getSelectionButton() { return outButton; }
	inline GameObject* getSelectionPowerUps() { return outPowerUP; }
	inline GLuint getCursor() { return currentCursor; }

	// Setters
	inline void setCamPos(glm::vec3 newPosition) { camPos = newPosition; }
	inline void setScale(glm::vec3 newScale) { camPos = newScale; }
	inline void setZoom(float newZoom) { zoom = newZoom; }
	inline void setBlueprints(std::vector<TowerObject*> b) { blueprints = b; }
	inline void setEnemyBlueprints(std::vector<EnemyObject*> e) { enemyBlueprints2 = e; }
	inline void setFlag(bool newFlag) { flag = newFlag; }
	inline void setButtonFlag(bool newFlag) { buttonFlag = newFlag; }
	inline void setPowerUpFlag(bool power) { powerUpFlag = power; }
	inline void setEnemyFlag(bool newEnemyFlag) { enemyFlag = newEnemyFlag; }
	inline void setButtons(std::vector<GameObject*> butt) { buttons = butt; }
	inline void setPowerUPs(std::vector<GameObject*> power) { powerUps = power; }
	inline void setFactor(float newFact) { factor = newFact; }
private:
	float zoom;
	glm::vec3 camPos;
	glm::vec3 scale;
	float factor;
	std::vector<Text*> textObjects;
	bool flag;//only important flag becuase its flag for selection of towers /////the other flags are just for the selection square demonstration
	bool powerUpFlag;
	bool buttonFlag;
	bool enemyFlag;

	GLFWwindow* window;

	//vectors storing all the items in the game
	std::vector<TowerObject*> blueprints;
	std::vector<GameObject*> buttons;
	std::vector<GameObject*> powerUps;
	std::vector<EnemyObject*> enemyBlueprints2;

	//These are used to store the current selected item. 
	GameObject* outPowerUP;
	GameObject* outButton;
	EnemyObject* outEnemy;
	TowerObject* outSelection;
	GLuint currentCursor;


};

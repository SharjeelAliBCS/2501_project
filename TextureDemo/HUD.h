#pragma once
#include "Shader.h"
#include "Window.h"
#include "GameObject.h"
#include "Text.h"
#include "TowerObject.h"

class HUD : public GameObject {
public:
	//constructor
	HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, std::string type);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void selection(double x, double y);
	// Renders the GameObject using a shader
	virtual void  render(Shader &shader) override;

	void addText(Text* t); 
	// Getters
	inline glm::vec3 getCamPos() { return camPos; }
	inline float getZoom() { return zoom; }
	inline std::vector<Text*> getTextObjects() { return textObjects; }
	inline bool getFlag() { return flag; }
	inline TowerObject* getSelection() { return outSelection; }
	inline GLuint getCursor() { return currentCursor; }

	// Setters
	inline void setCamPos(glm::vec3 newPosition) { camPos = newPosition; }
	inline void setScale(glm::vec3 newScale) { camPos = newScale; }
	inline void setZoom(float newZoom) { zoom = newZoom; }
	inline void setBlueprints(std::vector<TowerObject*> b) { blueprints = b; }
	inline void setFlag(bool newFlag) { flag = newFlag; }

private:
	float zoom;
	glm::vec3 camPos;
	glm::vec3 scale;

	std::vector<Text*> textObjects;

	bool flag;
	std::vector<TowerObject*> blueprints;
	TowerObject* outSelection;
	GLuint currentCursor;


};

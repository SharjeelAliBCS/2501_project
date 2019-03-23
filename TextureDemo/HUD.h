#pragma once
#include "Shader.h"
#include "Window.h"
#include "GameObject.h"
#include "Text.h"

class HUD : public GameObject {
public:
	//constructor
	HUD(glm::vec3 &entityPos, float zoomHUD, glm::vec3 objectScale, GLuint entityTexture, GLint entityNumElements, std::string type);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(Shader &shader) override;

	void addText(Text* t); 
	// Getters
	inline glm::vec3 getCamPos() { return camPos; }
	inline float getZoom() { return zoom; }
	inline std::vector<Text*> getTextObjects() { return textObjects; }

	// Setters
	inline void setCamPos(glm::vec3 newPosition) { camPos = newPosition; }
	inline void setScale(glm::vec3 newScale) { camPos = newScale; }
	inline void setZoom(float newZoom) { zoom = newZoom; }

private:
	float zoom;
	glm::vec3 camPos;
	glm::vec3 scale;
	std::vector<Text*> textObjects;
};

#pragma once
#include "GameObject.h"
#include "EnemyObject.h"
#include "ProjectileObject.h"
#include <deque>


class Text :
	public GameObject
{
public:
	Text(glm::vec3 &entityPos, std::map<char,GLuint> tex, std::string t, GLint entityNumElements, float s, std::string type);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(Shader &shader) override;

	//setters
	inline void setText(std::string t) { text = t; }
	inline void setRenderedText(std::string t) { renderText = t; }
	inline void setCamPos(glm::vec3 newPosition) { camPos = newPosition; }
	inline void setCamZoom(float newZoom) { zoom = newZoom; }

	//getters
	inline std::string getText() {return text; }


private:
	std::map<char, GLuint> characters;
	std::string text;
	std::string renderText;
	float scale;

	float zoom;
	glm::vec3 camPos;

};


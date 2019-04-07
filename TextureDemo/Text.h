#pragma once
#include "GameObject.h"
#include "EnemyObject.h"
#include "ProjectileObject.h"
#include <deque>


class Text :
	public GameObject
{
public:
	Text(glm::vec3 &entityPos, std::map<char, GLuint> tex, std::string t, GLint entityNumElements, float s, glm::vec3 c, std::string id);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(std::vector<Shader*> shaders) override;

	//setters
	inline void setText(std::string t) { text = t; }
	inline void setRenderedText(std::string t) { renderText = t; }
	inline void setCamPos(glm::vec3 newPosition) { camPos = newPosition; }
	inline void setCamZoom(float newZoom) { zoom = newZoom; }
	inline void setColor(glm::vec3 c) { color = c; }


	inline void setID(std::string i) { id = i; }
	//getters
	inline std::string getText() { return text; }
	inline std::string getID() { return id; }

private:
	std::string id;
	std::map<char, GLuint> characters;
	std::string text;
	std::string renderText;
	float scale;
	glm::vec3 color;

	float zoom;
	glm::vec3 camPos;

};


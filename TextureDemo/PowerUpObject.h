#pragma once
#include "GameObject.h"
class PowerUpObject :
	public GameObject
{
public:
	PowerUpObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, float range=-1, float dur=0, int c=0);
	~PowerUpObject();

	inline float getRange() { return range; }
	inline float getDur() { return dur; }

protected:
	float range;
	float dur;
};


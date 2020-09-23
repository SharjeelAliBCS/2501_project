#pragma once
#include "GameObject.h"
class UpgradeObject :
	public GameObject
{
public:
	UpgradeObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, float mod = 1, int c = 0);
	~UpgradeObject();

	inline float getMod() { return mod; }

protected:
	float mod;
};


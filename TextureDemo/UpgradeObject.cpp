#include "UpgradeObject.h"



UpgradeObject::UpgradeObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, float mod, int c)
	: GameObject(entityPos, entityTexture, entityNumElements, type, 1, c) {
	this->mod = mod;
}


UpgradeObject::~UpgradeObject()
{
}

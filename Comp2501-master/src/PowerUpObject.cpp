#include "PowerUpObject.h"



PowerUpObject::PowerUpObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type, float range, float dur, int c)
	: GameObject(entityPos, entityTexture, entityNumElements, type,1,c){
	this->range = range;
	this->dur = dur;
}


PowerUpObject::~PowerUpObject()
{
}

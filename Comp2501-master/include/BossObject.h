#pragma once

#include "EnemyObject.h"

// Inherits from GameObject
class BossObject : public EnemyObject {
public:
	BossObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt,float speed=1,int c=0);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
};
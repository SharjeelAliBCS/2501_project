#pragma once

#include "EnemyObject.h"

// Inherits from GameObject
class BossObject : public EnemyObject {
public:
	BossObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
};
#pragma once

#include "GameObject.h"
#include "EnemyObject.h"

// Inherits from GameObject
class TowerObject : public GameObject {
public:
	TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	void setCurrEnemy(EnemyObject* enemy) { currentEnemy = enemy; }
private:
	float rotationSpeed;
	virtual void deathAnimation();
	void rotateTurret();

	EnemyObject* currentEnemy;


};
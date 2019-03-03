#pragma once

#include "GameObject.h"
#include "EnemyObject.h"

// Inherits from GameObject
class TowerObject : public GameObject {
public:
	TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLuint turrTex,GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(Shader &shader) override;
	void setCurrEnemy(EnemyObject* enemy) { currentEnemy = enemy; }
	
private:
	float rotationSpeed, turretAngle;
	void deathAnimation();
	void rotateTurret();

	GLuint turretTexture;

	EnemyObject* currentEnemy;


};
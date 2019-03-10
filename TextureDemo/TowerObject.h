#pragma once

#include "GameObject.h"
#include "EnemyObject.h"
#include "ProjectileObject.h"
#include <deque>

// Inherits from GameObject
class TowerObject : public GameObject {
public:
	TowerObject(glm::vec3 &entityPos, std::vector<GLuint> tex, std::vector<GLuint> explosion,GLint entityNumElements,float d,std::string type);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(Shader &shader) override;
	inline void setCurrEnemy(EnemyObject* enemy) {currentEnemy = enemy; }
	
	
private:
	float rotationSpeed;

	//*********Finite State Machine******
	enum State {
		Init,
		Idle,
		Locate,
		Fire,
		Death
	};

	State _state;
	
	void deathAnimation();
	void locateEnemy();
	void fireEnemy();
	//***********************************
	GLuint turretTexture;
	GLuint projectileTex;

	EnemyObject* currentEnemy;

	int fireRate, frames;
	float dps;
	GLint size;
	std::vector<ProjectileObject*> bullObjects;
	std::vector<GLuint> explosion_tex;


};
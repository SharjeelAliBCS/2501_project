/*
Class: ProjectileObject
Purposes: handles the projectile object (bullets, laser)
Contributers: Sharjeel Ali
*/
#pragma once
#include "GameObject.h"
#include "EnemyObject.h"
class ProjectileObject : public GameObject
{
public:
	ProjectileObject(glm::vec3 &entityPos, GLuint entityTexture, std::vector<GLuint> explosion, GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float d, float s);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void  render(std::vector<Shader*> shaders);

	//setters
	inline void setCurrEnemy(EnemyObject* enemy) { target = enemy; }

private:
	float damage;
	float distance;

	//the target that the bullet moves towards
	EnemyObject* target;

	//the explosion texture that plays once a bullet despawns. 
	std::vector<GLuint> explosion_tex;
	//explosion index. 
	int explosion_num;
};


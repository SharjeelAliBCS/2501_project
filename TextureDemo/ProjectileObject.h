#pragma once
#include "GameObject.h"
#include "EnemyObject.h"
class ProjectileObject : public GameObject
{
public:
	ProjectileObject(glm::vec3 &entityPos, GLuint entityTexture, std::vector<GLuint> explosion,GLint entityNumElements, std::string type, EnemyObject* enemy, float angle, float d, float s);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	inline void setCurrEnemy(EnemyObject* enemy) { target = enemy; }

private:
	float damage;
	float distance;
	glm::vec3 orgCoord;//stores the original coord of where it launched from. 
	EnemyObject* target;

	std::vector<GLuint> explosion_tex;
	int explosion_num;
};


#pragma once

#include "GameObject.h"
#include "Node.h"
#include "Particle.h"
// Inherits from GameObject
class EnemyObject : public GameObject {
public:
	EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt, int c=0);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void  render(std::vector<Shader*> shaders);

	void enemyHit(float damage);
	inline bool getKilled() { return killed; }

	//setters
	inline void setCur(Node* n) { cur = n; }
	inline void setCurDestId(int id) { curDestId = id == -1 ? curDestId : id; }

	//getters
	inline int getCurDestId() const { return curDestId; }
	inline Node* getCur() { return cur; }

	float oldx;
	float oldy;
protected:

	Node * cur;
	float speed, health;
	int curDestId;
	bool hit,killed;

	//These variables below are used for the enemy death particle stuff
	GLuint enemyDeathTex;
	int framesDeath;
	Particle* deathParticles;
};


#pragma once

#include "GameObject.h"
#include "Node.h"
#include "Particle.h"
// Inherits from GameObject
class EnemyObject : public GameObject {
public:
	EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt, float speed = 1, int c = 0, float regen = 0);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void  render(std::vector<Shader*> shaders);

	void enemyHit(float damage);
	void enemyBurn(float b);

	inline bool getKilled() { return killed; }
	inline bool getSpawned() { return spawned; }
	inline float getHealth() { return health; }
	inline float getRegen() { return regen; }

	//setters
	inline void setCur(Node* n) { cur = n; }
	inline void setCurDestId(int id) { curDestId = id == -1 ? curDestId : id; }
	inline void setSpawned(bool s) { spawned = s; }

	//getters
	inline int getCurDestId() const { return curDestId; }
	inline Node* getCur() { return cur; }

	inline void modCurHealthCap(float hp) { curHealthCap *= hp; health = curHealthCap; }
	inline GLuint getEnemyDeathTex() { return enemyDeathTex; }

	inline void upHardiness(float mod) { defaultHealthCap = baseHealthCap * mod; regen = baseRegen * mod; }
	inline void upCost(float mod) { cost = (int)(baseCost*mod); }
	inline void upSpeed(float mod) { defaultSpeed = std::fmin(2.5,baseSpeed*mod); }

	float oldx;
	float oldy;
protected:

	Node * cur;
	float health, defaultHealthCap, curHealthCap, baseHealthCap;
	float regen, baseRegen;
	int curDestId;
	bool hit, killed, spawned;
	float curTime;
	float timeSinceLastHeal = 1;

	int baseCost;
	float baseSpeed;

	//These variables below are used for the enemy death particle stuff
	GLuint enemyDeathTex;
	int framesDeath;
	Particle* deathParticles;
	float burn;

	enum State {
		Init,
		Hit,
		Dying,
		Dead,
		Move
	};


	State _state;
};

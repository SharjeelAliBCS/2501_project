/*
Class: EnemyObject
Purposes: handles the enemyobject creeps
Contributers: Sharjeel Ali, David Neudorf
*/
#pragma once

#include "GameObject.h"
#include "Node.h"
#include "Particle.h"
// Inherits from GameObject
class EnemyObject : public GameObject {
public:
	EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float h, std::string type, GLuint edt, float speed = 1, int c = 0, float regen = 0);
	~EnemyObject();
	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	//render function that renders the enemy. 
	virtual void  render(std::vector<Shader*> shaders);

	//deals enemy damage. 
	void enemyHit(float damage);
	//adds a burn effect to the enemy.
	void enemyBurn(float b);

	//setters
	inline void setCur(Node* n) { cur = n; }
	inline void setCurDestId(int id) { curDestId = id == -1 ? curDestId : id; }
	inline void setSpawned(bool s) { spawned = s; }
	inline void setKilled(bool s) { killed = s; }

	//getters
	inline bool getKilled() { return killed; }
	inline bool getSpawned() { return spawned; }
	inline float getHealth() { return health; }
	inline float getRegen() { return regen; }
	inline int getCurDestId() const { return curDestId; }
	inline Node* getCur() { return cur; }
	inline GLuint getEnemyDeathTex() { return enemyDeathTex; }
	inline float getHealthCap() { return defaultHealthCap; }

	//upgrades for the enemy
	inline void modCurHealthCap(float hp) { curHealthCap *= hp; health = curHealthCap; }
	inline void upHardiness(float mod) { defaultHealthCap = baseHealthCap * mod; regen = baseRegen * mod; }
	inline void upCost(float mod) { cost = (int)(baseCost*mod); }
	inline void upSpeed(float mod) { defaultSpeed = std::fmin(2.5,baseSpeed*mod); }

	float oldx;
	float oldy;

protected:

	Node * cur;
	//enemy health
	float health, defaultHealthCap, curHealthCap, baseHealthCap;
	//enemy regen
	float regen, baseRegen;
	//stores the node id that the enemy is moving to. 
	int curDestId;
	bool hit, killed, spawned;
	float curTime;
	//used for regen
	float timeSinceLastHeal = 1;

	int baseCost;
	float baseSpeed;

	//These variables below are used for the enemy death particle stuff
	GLuint enemyDeathTex;
	int framesDeath;
	Particle* deathParticles;
	float burn;

	//finite state machine
	enum State {
		Init,
		Hit,
		Dying,
		Dead,
		Move
	};


	State _state;
};

#pragma once

#include "GameObject.h"
#include "EnemyObject.h"
#include "ProjectileObject.h"
#include "Particle.h"
#include <deque>
#include "Audio.h"

// Inherits from GameObject
class TowerObject : public GameObject {
public:
	TowerObject(glm::vec3 &entityPos, std::vector<GLuint> tex, std::vector<GLuint> explosion, GLint entityNumElements, float d, std::string type, float r, float ROF = 1, int c = 0, float speed = 0.1);
	~TowerObject();
	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(std::vector<Shader*> shaders) override;
	inline void setCurrEnemy(EnemyObject* enemy) { currentEnemy = enemy; }

	//setters
	inline void setBlueprintPos(float x, float y) { blueprintX = x; blueprintY = y; }
	inline void setEnemies(std::vector<EnemyObject*> e) { allEnemies = e; }
	inline void setCurrEnemies(std::vector<EnemyObject*> e) { currentEnemies = e; }


	//getters
	//getter for the vector of objects
	inline std::vector<GLuint> getTexvec() { return texvec; }
	inline float getRange() { return range; }
	inline std::vector<GLuint> getExplosion_tex() { return explosion_tex; }
	GLuint getIcon() { return icon; }
	inline float getDamage() { return damage; }
	inline float getSpeed() { return projectileSpeed; }
	inline float getROF() { return curROF; }
	inline void setCurROF(float rof) { curROF = rof; }
	inline void modCurROF(float rof) { curROF *= rof; }

	inline void upDamage(float mod) { damage = baseDamage * mod; }
	inline void upRange(float mod) { range = baseRange * mod; }
	inline void upROF(float mod) { defaultROF = std::fmax(baseROF *(2-mod),0.05); }


private:
	float rotationSpeed;

	//*********Finite State Machine******
	enum State {
		Idle,
		Locate,
		Fire,
		SpeedUp,
		SlowDown,
		CoolDown

	};


	State _state;

	void locateEnemy();
	void fireEnemy(double deltaTime);
	void move();
	bool lineCollision(EnemyObject* enemy);
	std::vector<EnemyObject*> enemiesInRange(float range);


	//***********************************
	GLuint turretTexture;
	GLuint projectileTex;

	int currPos = 0;
	glm::vec3 positions[2];
	glm::vec3 explodePos;
	EnemyObject* currentEnemy;
	EnemyObject* prevEnemy;
	std::vector<EnemyObject*> currentEnemies;
	std::vector<EnemyObject*> allEnemies;

	float curROF, baseROF, defaultROF;
	float frames;
	double lastShotTime;
	float damage, baseDamage;
	float projectileSpeed;
	int explosion_num;
	float range, baseRange;
	float orgSpeed;
	float duration;
	GLint size;
	std::vector<ProjectileObject*> bullObjects;
	std::vector<GLuint> explosion_tex;

	Particle* particle;
	std::vector<GLuint> texvec;
	std::vector<GLuint> texvecExplosion;

	//blueprint stuff:
	GLuint icon;
	float blueprintX, blueprintY;
};
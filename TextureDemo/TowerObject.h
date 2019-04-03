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
	TowerObject(glm::vec3 &entityPos, std::vector<GLuint> tex, std::vector<GLuint> explosion,GLint entityNumElements,float d,std::string type, float r, int c=0);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	// Renders the GameObject using a shader
	virtual void  render(std::vector<Shader*> shaders) override;
	inline void setCurrEnemy(EnemyObject* enemy) {currentEnemy = enemy; }
	inline void setAudio(Audio* a) { audio = a; audio->addAudio("Audio/rocket.wav", "bullet");}

	//getters
	//getter for the vector of objects
	inline std::vector<GLuint> getTexvec() { return texvec; }
	inline float getRange() {return range;}
	inline std::vector<GLuint> getExplosion_tex() { return explosion_tex; }
	GLuint getIcon() { return icon; }
	inline float getDps() { return dps; }
	
	
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
	float range;
	GLint size;
	std::vector<ProjectileObject*> bullObjects;
	std::vector<GLuint> explosion_tex;
	GLuint icon;
	Particle* particle;
	std::vector<GLuint> texvec;
	std::vector<GLuint> texvecExplosion;

	Audio* audio;



};
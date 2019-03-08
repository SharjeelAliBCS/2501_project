#pragma once

#include "GameObject.h"
#include "Node.h"

// Inherits from GameObject
class EnemyObject : public GameObject {
public:
	EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void  render(Shader &shader);
	inline void setCur(Node* n) { cur = n; }
	void setHit(bool h) { hit = h; }
	inline Node* getCur() { return cur; }

	float oldx;
	float oldy;
protected:
	Node * cur;
	float speed;
	bool hit;

};


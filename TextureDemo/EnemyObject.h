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
	void setHit(bool h) { hit = h; }
	inline void setCur(Node* n) { cur = n; }
	
	inline void setCurDestId(int id) { curDestId = id == -1 ? curDestId : id; }

	inline int getCurDestId() const { return curDestId; }
	inline Node* getCur() { return cur; }

	float oldx;
	float oldy;
protected:
	Node * cur;
	float speed;
	int curDestId;
	bool hit;

};


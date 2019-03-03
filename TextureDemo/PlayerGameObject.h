#pragma once

#include "GameObject.h"
#include "Node.h"

// Inherits from GameObject
class PlayerGameObject : public GameObject {
public:
	PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, std::string type);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	inline void setCur(Node* n) { cur = n; }
	inline Node* getCur() { return cur; }
protected:
	Node* cur;
};

#pragma once

#include "Node.h"
#include "Shader.h"
#include "GameObject.h"
#include "Window.h"
#include "EnemyObject.h"


#include <queue>
#include <deque>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <functional>

//struct used to rank various nodes in the priority_queue
struct QNode{
	Node *node;
	int cost;
};

class Graph {
public:
	//constructor. 
	Graph(int nodeWidth, int nodeHeight, GameObject nodeSprite, std::map<std::string, GLuint> &tex, std::string fname);

	
	//Loops through array and prints out associated data for each node.
	void printData();

	//gets mouse input, updates start and end position using that information
	void update(Node*, bool block, bool clear=true);

	//returns the id of the node at the screen coordinates. If no node exists, it will return -1
	//int Graph::getNodeIdFromCoords(double x, double y);

	//returns the id of a node from an x/y coordinate
	int selectNode(double x, double y);

	//returns a reference to the node with the supplied id.
	Node& getNode(int id);

	void highlight(int n);

	//renders all the nodes in the graph
	void render(Shader &shader);

	//inline bool revStrCmp(std::string s1, std::string s2) { return -1 * s1.compare(s2); }

	bool rePath(std::vector<EnemyObject*>* creeps, bool T = false);
	//creates and marks a path from start to end
	bool pathfind(int destId, bool clear=true);

	//setters
	inline void setStart(int nodeId) { startNodeId = nodeId; }
	inline void setEnd(int nodeId) { endNodeId = nodeId; }

	inline void setZoom(float camZoom) { zoom = camZoom; }
	inline void setCamPos(glm::vec3 pos) { camPos = pos; }

	//getters
	inline int getStartId() { return startNodeId; }
	inline int getEndId() { return endNodeId; }
	inline float getZoom() { return zoom; }
	inline glm::vec3 getCamPos() { return camPos; }
	//void getCoord(double&x, double&y);
	void getHoverCoords(float &x, float&y);

	inline int getHover() { return hover; }

	inline std::set<int> getTopStartSet() { return topStartSet; }
	inline std::set<int> getBotStartSet() { return botStartSet; }
private:
	//node sprite used to draw each node.
	GameObject nodeObj;
	
	
	int hover;
	
	float zoom;

	float movementX;
	float movementY;
	float start_x;
	float start_y;

	glm::vec3 camPos;
	//id of start and endnode
	int startNodeId, endNodeId;

	//int oldEndNodeId;

	//2d vector containing all the nodes in the graph
	std::vector<std::vector<Node*>> nodes;
	int nodeWid;
	int size;
	//map used for easy access to nodes via id
	std::map<int, Node*> nodeMap;
	std::map<std::string, GLuint> texMap;
	std::map<int, int> topDestMap;
	std::map<int, int> botDestMap;
	std::set<int> topStartSet;
	std::set<int> botStartSet;


};
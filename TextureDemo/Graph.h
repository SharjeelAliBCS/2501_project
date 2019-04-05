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
	Graph(int nodeWidth, int nodeHeight, GameObject nodeSprite, std::map<std::string, GLuint> &tex, std::string fname, int x, int y, GLFWwindow* w);

	
	//Loops through array and prints out associated data for each node.
	void printData();

	//gets mouse input, updates start and end position using that information
	void update(Node*);
	//renders all the nodes in the graph
	void render(std::vector<Shader*> shaders);

	//returns the id of a node from an x/y coordinate
	int selectNode(double x, double y);

	//returns a reference to the node with the supplied id.
	

	void highlight(int n);

	
	bool rePath(std::vector<EnemyObject*>* creeps, int id, int pathCount, char side);
	//creates and marks a path from start to end
	bool pathfind(int destId, int pathCount);

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
	Node& getNode(int id);
	void getHoverCoords(float &x, float&y);
	inline int getHover() { return hover; }

	inline std::set<int> getTopStartSet() { return topStartSet; }
	inline std::set<int> getBotStartSet() { return botStartSet; }
	inline std::set<int> getStartSet(char side) { return side == 'B' ? botStartSet : topStartSet; }

	inline int getEndPoints(int i) { return endPoints[i]; }
	inline glm::vec3 getFocalPoint(int i) { return focalPoints[i]; }

	void clearNextNodeMaps();
	void startPaths();

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

	int endPoints[2];
	glm::vec3 focalPoints[2];

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

	GLFWwindow* window;

	int window_width_g = 800;
	int window_height_g = 600;

};
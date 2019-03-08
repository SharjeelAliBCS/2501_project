#include "Graph.h"

#include <iostream>	//used for debug purposes
using namespace std;

//class used to compare 2 nodeTuple structs
class compareNode
{
public:
	int operator() (const QNode &n1, const QNode &n2)
	{
		return n1.cost > n2.cost;
	}
};

//main constructor
//takes the width, height of graph, as well as a gameobject used to render each node.
Graph::Graph(int nodeWidth, int nodeHeight, GameObject nodeSprite, std::map<std::string, GLuint> &tex, std::string fname) : nodeObj(nodeSprite), camPos(glm::vec3(0.0f)) {
	//initializes the 2d nodes array and nodeMap
	nodes = std::vector<std::vector<Node*>>();
	nodeMap = std::map<int, Node*>();
	std::deque<pair<int,int>> removeNode = std::deque<pair<int,int>>();
	texMap = tex;
	nodeWid = nodeWidth;
	size = nodeWidth * nodeHeight - 1;
	//int hover = -1;
	//data for setting node positions on, screen. This works best for a 40x30 graph
	//TODO change these values based on graph size.
	//float x_b = -4.4f;
	//float y_b = 4.4f;
	//float movementX = 0.22f;
	//float movementY = -0.3f;
	//float movementX = 0.2f;

	//movementX = 0.205f;
	//movementY = -0.31f; //uncomment to see grid

	movementX = 0.225f;
	movementY = -0.3f;

	start_x = nodeWidth * movementX * -0.5;
	start_y = nodeHeight * movementY * -0.5;


	//fills the 2d nodes array with nodes.
	for (int i = 0; i < nodeHeight; i++) {
		std::vector<Node*> nodeRow = std::vector<Node*>();

		for (int j = 0; j < nodeWidth; j++) {
			//creates each node, starting at (-4.4, 4.4), topLeft, going down, right
			//with x distance .22, y distance .3
			Node* newNode = new Node(start_x + j * movementX, start_y + i * movementY, nodeObj.getTex());
			nodeRow.push_back(newNode);

		}
		nodes.push_back(nodeRow);
	}

	//connects node to each other to form a 4-connected graph with random edge weights
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {

			//if there exists a node to the right of the current node, link them together
			if (j + 1 < nodes.at(i).size()) {
				//int randWeight = 10 + (rand() % 6);	//creates a random weight between 10-15

				Node *n1 = nodes.at(i).at(j);		//referncec to current node in graph.
				Node *n2 = nodes.at(i).at(j + 1);	//reference to node to the left of the current node.

				n1->addNode(*n2, 10);			//links both nodes together
			}

			//if there exists a node below the current node, link them together
			if (i + 1 < nodes.size()) {
				//int randWeight = 10 + (rand() % 6);	//creates a random weight between 10-15

				Node *n1 = nodes.at(i).at(j);		//referncec to current node in graph.
				Node *n2 = nodes.at(i + 1).at(j);	//node below the current node.

				n1->addNode(*n2, 10);			//links both nodes together
			}
		}
	}

	//adds all nodes to map with nodeId as key and a pointer to the node
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			nodeMap.insert(std::pair<int, Node*>(nodes.at(i).at(j)->getId(), nodes.at(i).at(j)));
		}
	}


	std::ifstream in(fname);
	
	//std::map<std::string, int, std::greater<std::string>> checkpoints;
	std::map<std::string, pair<std::string,int>> checkpoints;


	std::string line, field;
	int id = 0;
	int row = 0;
	int col = 0;

	while (getline(in, line))    // get next line in file
	{
		stringstream ss(line);
		//std::cout<< id<< std::endl;
		while (getline(ss, field, ','))  // break line into comma delimitted fields
		{

			//std::cout << "field: " << field << std::endl;
			if (field.compare("-1") == 0) {
				nodeMap[id]->setTex(texMap["ELSE"]);
				removeNode.push_front(pair<int, int>(row, col));
			}
			else if (texMap.find(field) != texMap.end()) {
				nodeMap[id]->setTex(texMap[field]);
				if (field.compare("1") == 0) {
					getNode(id).setBuildable(true);
				}
				if (field.substr(0, 1).compare("0") == 0 || field.substr(0, 1).compare("6") == 0) {
					getNode(id).setPathable(false);
				}

			}
			else if (field.substr(1, 1).compare("_") == 0) {
				std::string cur = field.substr(0, 4);
				std::string next = field.substr(6);
				checkpoints.insert(std::pair<std::string, pair<std::string,int>>(cur, std::pair<std::string,int>(next,id)));
				nodeMap[id]->setTex(texMap["checkpoint"]);
				if (field.substr(2, 2).compare("HP") == 0) {
					nodeMap[id]->setTex(texMap["hp"]);
				}
			}
			else {
				nodeMap[id]->setTex(texMap["ELSE"]);
				std::cout << std::endl << "FIELD NOT FOUND : " << field << std::endl << std::endl;
				removeNode.push_front(pair<int,int>(row,col));
				
			}
			id++;
			col++;
		}
		//std::cout << std::endl;
		//id++;
		row++;
		col = 0;
	}
	in.close();
	std::cout << "file read" << std::endl;
	for (std::deque<pair<int, int>>::iterator it = removeNode.begin(); it != removeNode.end(); ++it) {
		std::vector<Node*> row = nodes[it->first];
		//std::cout << it->first <<" ==> " << it->second << std::endl;
		//std::cout << row.size();
		row.erase(row.begin()+it->second);
		nodes[it->first] = row;
		
		//break;
	}



	std::string key;
	std::string nextKey;
	for (std::map<std::string, pair<std::string,int>>::iterator it = checkpoints.begin(); it != checkpoints.end(); ++it) {
		//std::cout << it->first << " ==> " << it->second.first << ", " << it->second.second << '\n';

		if (it->first.substr(0, 1).compare("T") == 0) {
			if (it->first.substr(3, 1).compare("0") == 0) {
				topStartSet.insert(it->second.second);
			}
			if (it->second.first.compare("-1") == 0) {
				topDestMap.insert(std::pair<int, int>(it->second.second, -1));
				continue;
			}
			topDestMap.insert(std::pair<int, int>(it->second.second, checkpoints[it->second.first].second));
		}
		else {//B
			if (it->first.substr(3, 1).compare("0") == 0) {
				botStartSet.insert(it->second.second);
			}
			if (it->second.first.compare("-1") == 0) {
				botDestMap.insert(std::pair<int, int>(it->second.second, -1));
				continue;
			}
			botDestMap.insert(std::pair<int, int>(it->second.second, checkpoints[it->second.first].second));
		}
	}
	std::cout << "T\n";
	for (std::map<int, int>::iterator it = topDestMap.begin(); it != topDestMap.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
		getNode(it->first).setNextId(it->second);
		if (it->second != -1) {
			setStart(it->first);
			setEnd(it->second);
			pathfind(endNodeId,false);
		}
		else {
			setEnd(it->first);
		}
	}
	std::cout << "B\n";
	for (std::map<int, int>::iterator it = botDestMap.begin(); it != botDestMap.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
		getNode(it->first).setNextId(it->second);
		if (it->second != -1) {
			setStart(it->first);
			setEnd(it->second);
			pathfind(endNodeId, false);
		}
		else {
			setEnd(it->first);
		}
		
	}
	std::cout << "StartSets\n";
	for (std::set<int>::iterator it = botStartSet.begin(); it != botStartSet.end(); ++it) {
		std::cout << *it << '\n';
	}
	for (std::set<int>::iterator it = topStartSet.begin(); it != topStartSet.end(); ++it) {
		std::cout << *it << '\n';
	}
	std::cout <<"\n1\n\n";

	//sets the start/end nodes to the top_left and bottom_right nodes.
	//setStart(start);
	//setEnd(end);
	//oldEndNodeId = end;
	zoom = 0.2f;
	getNode(0).toggleHighlight();
	
}

//Loops through array and prints out associated data for each node.
void Graph::printData() {
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			cout << i << ", " << j << " - " << nodes.at(i).at(j)->getId() << " - " << &nodes.at(i).at(j);
			cout << " - " << nodes.at(i).at(j)->getEdges().size() << endl;
		}
	}
}

void Graph::highlight(int n) {

	getNode(hover).toggleHighlight();
	hover = n;
	getNode(hover).toggleHighlight();
}
//gets mouse input, updates start and end position using that information

void Graph::update(Node* s, bool block, bool clear) {

	int sid = s->getId();
	if (sid != -1 && sid != endNodeId) {
		setStart(sid);
	}
	double xpos, ypos;
	glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);
	int n = selectNode(xpos, ypos);
	if (n != -1 && n != endNodeId && n != sid) {
		highlight(n);
	}


	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//gets the node corresponding the mouseclick
		int n = selectNode(xpos, ypos);

		//set the start to selected node, if node exists and is not the start-node.
		if (n != -1 && n != startNodeId && getNode(n).getPathable()) {
			//oldEndNodeId = endNodeId;
			setEnd(n);
			pathfind(endNodeId,true);
		}
		//pathfind(clear);
	}
}

//returns the id of the node at the mouse coordinates
void Graph::getHoverCoords(float &x, float &y) {
	Node n = getNode(hover);
	x = n.getX();
	y = n.getY();
}

int Graph::selectNode(double x, double y) {
	unsigned int window_width_g = 800;
	unsigned int window_height_g = 600;

	//if the mouse is outside the window, return -1
	if (x < 0 || x > window_width_g || y < 0 || y > window_height_g) {
		return -1;
	}
	else {
		float cursor_x_pos = (x / (float)(window_width_g / 2)) - 0.986f;//1.0f;	//transforms cursor position to screen coordinates
		float cursor_y_pos = (y / (float)(window_height_g / 2)) - 0.986f;//1.0f;


		cursor_x_pos /= zoom;
		cursor_y_pos /= zoom;	//transforms cursor position based on screen scale. used to be const 0.2

		cursor_x_pos -= camPos.x;
		cursor_y_pos += camPos.y; // transform cursor position based on screen pos

		int x = (int)((cursor_x_pos - start_x) / movementX);
		int y = (int)((-cursor_y_pos - start_y) / movementY);


		//check if the node is 
		int tryId = nodeWid * y + x;
		if (tryId<0 || tryId>size) {
			return -1;
		}
		return tryId;

	}
}

//renders all the nodes in the graph
void Graph::render(Shader &shader) {

	//goes through each node and renders it, using the provided gameObject
	for (int j = 0; j < nodes.size(); j++) {
		
		if (nodes.size() == 0) {	//if there are no nodes, do nothing.
			return;
		}

		//get the location of the color uniform
		GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");

		for (int i = 0; i < nodes.at(j).size(); i++) {
			//gets the current node to draw
			Node* currentNode = nodes.at(j).at(i);

			//set the node 'pen' to the position of the current node.
			nodeObj.setPosition(glm::vec3(currentNode->getX(), currentNode->getY(), 0.0f));
			nodeObj.setTex(currentNode->getTex());
			
			//set the color of the node via the color uniform. Default is dark green
			glUniform3f(color_loc, -0.2f, -0.2f, -0.2f);	//dark green
			glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);	//dark green

			
			//change the color uniform depending on if the node is the start or end node.
			if (currentNode->getId() == startNodeId) {
				glUniform3f(color_loc, 1.0f, -1.0f, -1.0f);	//red = start
			}
			else if (currentNode->getId() == endNodeId) {
				glUniform3f(color_loc, -1.0f, -1.0f, 1.0f); //blue = end
			}else if (currentNode->isOnPath()) {
				glUniform3f(color_loc, 0.0f, 0.0f, 1.0f);	//light blue = on path
			}
			//else if (currentNode.isVisited()) {
			//	glUniform3f(color_loc, -1.0f, -1.0f, -1.0f);	//black = searched
			//}

			if (currentNode->getHighlight()) {
				glUniform3f(color_loc, 1.0f, 1.0f, -1.0f);	//white = cannot be pathed
			}

			nodeObj.render(shader);
		}
	}
}

//returns a reference to the node with the supplied id.
Node& Graph::getNode(int id) {
	
	//because we also store the graph as a map with the id as the key, we can easily reference each node.
	return *nodeMap.at(id);
}

/*TODO
create map for each node to store path based on dest, otherwise overwritten
*/
bool Graph::rePath(std::vector<EnemyObject*>* creeps, bool T) {
	map<int, int> mapInUse = T ? topDestMap : botDestMap;
	bool pathFound = true;
	std::cout << "got here 1\n";
	if (creeps != NULL) {
		for (std::vector<EnemyObject*>::iterator it = creeps->begin(); it != creeps->end(); ++it) {
			setStart((*it)->getCur()->getId());
			setEnd((*it)->getCurDestId());
			std::cout << startNodeId << " => " << endNodeId << std::endl;
			if (startNodeId == endNodeId) { continue; }
			if (getEndId() == -1) {
				std::cout << std::endl << "Houston we have a problem, creep dest = -1" << std::endl << std::endl;
			}
			pathFound = pathFound ? pathfind((*it)->getCurDestId(), false) : false;
			std::cout << pathFound << std::endl;
		}
	}
	std::cout << "got here 2\n";
	for (std::map<int, int>::iterator it = mapInUse.begin(); it != mapInUse.end(); ++it) {
		std::cout << it->first << " => " << it->second << '\n';
		if (it->second != -1) {
			setStart(it->first);
			setEnd(it->second);
		}
		else {
			setEnd(it->first);
		}
		if (startNodeId == endNodeId) { continue; }
		pathFound = pathFound ? pathfind(endNodeId,false) : false;
	}
	std::cout << "got here 3\n";
	return pathFound;
}


//using zombie-key based approach to Djikstra's algorithm
bool Graph::pathfind(int destId, bool clear) {

	//priority queue used in pathfinding.
	//it is created using the NodeTuple struct with a min compare function called compareNode
	priority_queue <QNode, vector<QNode>, compareNode> pq;

	//sets the costs of all nodes to infinity. reset all nodes to be off-path
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			nodes.at(i).at(j)->setCost(INT_MAX);
			if (clear) {
				nodes.at(i).at(j)->setOnPath(false);
				//nodes.at(i).at(j).setVisited(false);
			}
		}
	}

	//The startnode is added to the pq with cost 0
	QNode startNode = {&getNode(startNodeId), 0};
	pq.push(startNode);

	
	//now that the pq is setup, we can start the algorithm
	//keep in mind that the as the QNode struct has a pointer to the corresponding node
	//some function calls will use pointer syntax (->) 
	
	bool endFound = false;
	while (!pq.empty()) {
		//get the current lowest-cost node in pq
		QNode lowest = pq.top();
		//lowest.node->setVisited(true);
		//if the current node is the end node, done!
		if (lowest.node->getId() == endNodeId) {
			endFound = true;
			break;
		}

		//OPEN NODE
		vector<Edge> neighbours = lowest.node->getEdges();
		for (int i = 0; i < neighbours.size(); i++) {

			//compute cost to get to neighbouring node
			//cost = the cost to get the corrent node + cost to traverse the edge
			
			Node *n = &getNode(lowest.node->getOtherNode(neighbours.at(i)).getId());
			if (n->getPathable()) {
				//Node *n = &(lowest.node->getOtherNode(neighbours.at(i)));
				int manhattan = abs(getNode(endNodeId).getX() - neighbours.at(i).n2.getX()) +
					abs(getNode(endNodeId).getY() - neighbours.at(i).n2.getY());
				int nodeCost = lowest.cost + neighbours.at(i).cost + manhattan * 10;

				//if current node cost is higher than calculated, update node, and add QNode to queue			
				if (n->getCost() > nodeCost) {
					n->setCost(nodeCost);
					n->setPrev(lowest.node);

					QNode updatedNode = { n, nodeCost };
					pq.push(updatedNode);
				}
			}
		}

		pq.pop();	//REMOVE NODE FROM QUEUE
	}


	if (endFound) {

		//queue is done, go in reverse from END to START to determine path
		Node* nextNode = &getNode(endNodeId);
		Node* currentNode = getNode(endNodeId).getPrev();
		currentNode->setNextNode(destId,nextNode);
		//std::cout << nextNodeId << std::endl;
		//while the current node isn't null, or the end, mark the current node as on the path
		while (currentNode != NULL && currentNode->getId() != startNodeId) {
			currentNode->setOnPath(true);
			currentNode->setNextNode(destId,nextNode);
			nextNode = currentNode;
			//std::cout << nextNodeId << std::endl;
			currentNode = currentNode->getPrev();
		}
		//std::cout << nextNodeId << std::endl;
		currentNode->setNextNode(destId,nextNode);
		//std::cout << std::endl;
	}
	else {
		std::cout << "failed" << std::endl;
		//endNodeId = oldEndNodeId;
		//setEnd(oldEndNodeId);
		//delete this later
		//if (!redone) {
		//	pathfind(clear,true);
		//}
	}
	return endFound;
}
/* David Neudorf
 * 101029913
 * Mobile sprite added, an ant. At the time of writing it is only a single frame and does not rotate, 
 *	 This is not a requirement in the spec but it would be nice to have.
 * The player walks along the path determined via right click (as per diablo-style request)
 *	 to the blue circle using code from lines 192 to about 340. I spent far too many hours 
 *	 on this I was using that damned getNodeIdFromCoordinates which is screwy as hell. 
 * Obstacles (white) are now added with left click and have their weight to 100000, they are not
 *	 however impossible to path, Might fix this if I feel like it but is out of assignment scope regardless.
 *	 Might also make obstacles togglable, but again not implemented as of writing (set in update)
 * Black nodes were searched in the algorithm (set in pathfind method)
 * A* implemented line 294 of graph.cc using Manhattan distance.
 * Bonus has been implemented. Modified update and pathfind to show both paths. Red is only on one, 
 *	 but that was only for checking that the start is changing so I don't much care.
 * The only issue I'm aware of is some jumpyness when you move the window, but that is out of scope for this assignment.
 * Path color can be cleared by the faster sprite, annoying but trivial.
 */


#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Shader.h"
#include "Window.h"
#include "PlayerGameObject.h"
#include "Graph.h"
#include "TowerObject.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Pathfinding Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
//const glm::vec3 viewport_background_color_g(0.15, 0.17, 0.21);
const glm::vec3 viewport_background_color_g(1, 1, 1);

// Global texture info
GLuint tex[30];

// Global game object info
std::vector<GameObject*> gameObjects;


// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, //t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}


void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(void)
{
//	tex = new GLuint[4];
	glGenTextures(30, tex);
	//setthisTexture(tex[0], "orb.png");
	
	setthisTexture(tex[0], "Graphics/Map/0_0_boundary.png");
	setthisTexture(tex[1], "Graphics/Map/0_1_boundary.png");

	setthisTexture(tex[2], "Graphics/Map/1_empty.png");
	setthisTexture(tex[3], "Graphics/Map/3_1hp.png");
	setthisTexture(tex[4], "Graphics/Map/7_destructible.png");
	setthisTexture(tex[5], "Graphics/Map/6_spawn.png");

	setthisTexture(tex[6], "Graphics/Enemy/01_enemy.png");
	setthisTexture(tex[7], "Graphics/Enemy/monster_42.png");

	setthisTexture(tex[8], "Graphics/Map/2_checkpoint.png");
	setthisTexture(tex[9], "Graphics/Map/3_health.png");

	setthisTexture(tex[10], "Graphics/Tower/01_tower.png");
	setthisTexture(tex[11], "Graphics/Tower/01_turret.png");
	setthisTexture(tex[12], "Graphics/Tower/monster_41.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
}


// Main function that builds and runs the game
int main(void){
	try {
		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create geometry of the square
		int size = CreateSquare();

		// Set up shaders
		Shader shader("shader.vert", "shader.frag");

		// Set up the textures
		setallTexture();

		std::map<std::string,GLuint> texMap = std::map<std::string, GLuint>();
		texMap.insert(std::pair<std::string, GLuint >("0.5", tex[0]));
		texMap.insert(std::pair<std::string, GLuint >("0", tex[1]));
		texMap.insert(std::pair<std::string, GLuint >("1", tex[2]));

		texMap.insert(std::pair<std::string, GLuint >("2", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("3", tex[3]));
		texMap.insert(std::pair<std::string, GLuint >("4", tex[4]));
		texMap.insert(std::pair<std::string, GLuint >("5", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("6", tex[5]));
		texMap.insert(std::pair<std::string, GLuint >("ELSE", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("checkpoint", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("hp", tex[9]));
		texMap.insert(std::pair<std::string, GLuint >("tower", tex[10]));




		std::string fname = "map2.csv";
		int wid = 0;
		int height = 0;
		int start = 1149;
		int end = 122;

		//start = 415;
		//end = 430;
		std::ifstream in(fname);
		std::string line, field;
		while (getline(in, line)) { 
			if (!height) {
				std::stringstream ss(line);
				while (getline(ss, field, ','))  // break line into comma delimitted fields
				{
					++wid;
				}
			}
			++height;
		}

		Graph g = Graph(wid, height, GameObject(glm::vec3(0.0f), tex[0], size,"map"),texMap, fname, start,end);

		start = *(g.getBotStartSet().begin());
		std::cout << std::endl << start;
		EnemyObject *p1 = new EnemyObject(glm::vec3(g.getNode(start).getX(), g.getNode(start).getY(), 0.0f), tex[6], size,"enemy");
		EnemyObject *p2 = new EnemyObject(glm::vec3(g.getNode(start).getX(), g.getNode(start).getY(), 0.0f), tex[6], size,"enemy");

		p1->setSpeed(1.5);
		gameObjects.push_back(p1);
		float p1x = p1->getPosition().x;
		float p1y = p1->getPosition().y;
		float oldp1x = p1x;
		float oldp1y = p1y;
		gameObjects.push_back(p2);
		float p2x = p2->getPosition().x;
		float p2y = p2->getPosition().y;
		float oldp2x = p2x;
		float oldp2y = p2y;
		//int id = g.getNodeIdFromCoords(p1x, p1y);
	
		g.setStart(start);
		// Run the main loop
		double lastTime = glfwGetTime();
		Node* cur1 = &g.getNode(start);
		Node* cur2 = &g.getNode(start);

		p1->setCur(cur1);
		p2->setCur(cur2);
		float timeOfLastMove = 0.0f;

		float maxCamZoom = 0.60f;
		float minCamZoom = 0.07f;

		float cameraZoom = 0.19f;
		glm::vec3 cameraTranslatePos(glm::vec3(0.0f));
		float camShiftInc = 0.5f;
		float camZoomInc = 0.01f;

		g.setZoom(cameraZoom);
		g.setCamPos(cameraTranslatePos);
		bool toggleBlock = false;
		while (!glfwWindowShouldClose(window.getWindow())) {

	

			if (timeOfLastMove + 0.05 < glfwGetTime()) {
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
					cameraTranslatePos.y -= camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
					cameraTranslatePos.y += camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
					cameraTranslatePos.x -= camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
					cameraTranslatePos.x += camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
					cameraZoom = std::fmin(cameraZoom + camZoomInc, maxCamZoom);
					g.setZoom(cameraZoom);
					timeOfLastMove = glfwGetTime();

				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_X) == GLFW_PRESS) {
					cameraZoom = std::fmax(cameraZoom - camZoomInc, minCamZoom);
					g.setZoom(cameraZoom);
					timeOfLastMove = glfwGetTime();
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_T) == GLFW_PRESS && 
					(timeOfLastMove + 0.15 < glfwGetTime())) {
					std::cout << "T" << std::endl;
					toggleBlock = !toggleBlock;
					timeOfLastMove = glfwGetTime();

				}
				if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					//double xpos, ypos;
					//glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);
					float x;// = (float)xpos;
					float y;// = (float)ypos;
					int id = g.getHover();
					g.getHoverCoords(x,y);
					if (g.getNode(id).getBuildable()) {
						gameObjects.push_back(new TowerObject(glm::vec3(x, y, 0.0f), tex[10], tex[11],tex[12], size, "tower"));
						std::cout << g.getNode(id).getBuildable() << " => " << g.getNode(id).getPathable()<<std::endl;
						g.getNode(id).toggleTower();
						std::cout << g.getNode(id).getBuildable() << " => " << g.getNode(id).getPathable()<<std::endl<<std::endl;


					}
					

				}

			}
			//std::cout << g.getZoom() << std::endl;
			//std::cout << g.getCamPos().x << " " << g.getCamPos().y << std::endl;
			// Clear background
			window.clear(viewport_background_color_g);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// Select proper shader program to use
			shader.enable();

			// Setup camera to focus on (0, 0)
			
			
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);
			
			//animate p1
			oldp1x = p1x;
			oldp1y = p1y;
			p1x = round(p1->getPosition().x * 100) / 100;
			p1y = round(p1->getPosition().y * 100) / 100;

			if (cur1->getId() != g.getEndId()) {
				Node* next = cur1->getNextNode();
				float nextx = next->getX();
				float nexty = next->getY();
				//update velocity
				p1->setTargetPos(glm::vec3(nextx, nexty, 0.0f));
				glm::vec3 vel = p1->getVelocity();
				if (p1x < nextx) {
					vel.x = 1.0f;
				}else if (p1x > nextx) {
					vel.x = -1.0f;
				}else {
					vel.x = 0.0f;
				}
				if (p1y < nexty) {
					vel.y = 1.0f;
				}
				else if (p1y > nexty) {
					vel.y = -1.0f;
				}else {
					vel.y = 0.0f;
				}
				//if we walked over the node and we are not going to be at the end
				if (((oldp1x <= nextx && nextx <= p1x) || (oldp1x >= nextx && nextx >= p1x)) &&
					((oldp1y <= nexty && nexty <= p1y) || (oldp1y >= nexty && nexty >= p1y)) &&
					next->getNextNode() != NULL) {
					//update current node and color
					//cur1->setOnPath(false);
					cur1 = next;
					p1->setPosition(glm::vec3(nextx, nexty, 0.0f));
					p1->setCur(cur1);
					p1x = round(p1->getPosition().x * 100) / 100;
					p1y = round(p1->getPosition().y * 100) / 100;
					//test variables are legacy from messing with the cursed getNodeIdFromCoords() method
					Node* test = next->getNextNode();
					float testx = test->getX();
					float testy = test->getY();
					next = test;
					nextx = testx;
					nexty = testy;
					//update target
					p1->setTargetPos(glm::vec3(test->getX(), test->getY(), 0.0f));
					//update velocity
					if (p1x < nextx) {
						vel.x = 1.0f;
					}else if (p1x > nextx) {
						vel.x = -1.0f;
					}else {
						vel.x = 0.0f;
					}
					if (p1y < nexty) {
						vel.y = 1.0f;
					}
					else if (p1y > nexty) {
						vel.y = -1.0f;
					}
					else {
						vel.y = 0.0f;
					}
				}
				p1->setVelocity(vel);
			}
			//stop moving if we're done
			if (cur1->getId() == g.getEndId()) {
				std::cout << "fuck";
				p1->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			

			//animate p2 leaving out comments bc identical to above.
			oldp2x = p2x;
			oldp2y = p2y;
			p2x = round(p2->getPosition().x * 100) / 100;
			p2y = round(p2->getPosition().y * 100) / 100;

			if (cur2->getId() != g.getEndId()) {
				Node* next = cur2->getNextNode();
				float nextx = next->getX();
				float nexty = next->getY();

				p2->setTargetPos(glm::vec3(nextx, nexty, 0.0f));
				glm::vec3 vel = p2->getVelocity();
				if (p2x < nextx) {
					vel.x = 1.0f;
				}
				else if (p2x > nextx) {
					vel.x = -1.0f;
				}
				else {
					vel.x = 0.0f;
				}
				if (p2y < nexty) {
					vel.y = 1.0f;
				}
				else if (p2y > nexty) {
					vel.y = -1.0f;
				}
				else {
					vel.y = 0.0f;
				}
				if (((oldp2x <= nextx && nextx <= p2x) || (oldp2x >= nextx && nextx >= p2x)) &&
					((oldp2y <= nexty && nexty <= p2y) || (oldp2y >= nexty && nexty >= p2y)) &&
					next->getNextNode() != NULL) {

					//cur2->setOnPath(false);
					cur2 = next;
					p2->setPosition(glm::vec3(nextx, nexty, 0.0f));
					p2->setCur(cur2);
					p2x = round(p2->getPosition().x * 100) / 100;
					p2y = round(p2->getPosition().y * 100) / 100;

					Node* test = next->getNextNode();
					float testx = test->getX();
					float testy = test->getY();
					next = test;
					nextx = testx;
					nexty = testy;

					p2->setTargetPos(glm::vec3(test->getX(), test->getY(), 0.0f));

					if (p2x < nextx) {
						vel.x = 1.0f;
					}
					else if (p2x > nextx) {
						vel.x = -1.0f;
					}
					else {
						vel.x = 0.0f;
					}
					if (p2y < nexty) {
						vel.y = 1.0f;
					}
					else if (p2y > nexty) {
						vel.y = -1.0f;
					}
					else {
						vel.y = 0.0f;
					}
				}
				p2->setVelocity(vel);
			}
			if (cur2->getId() == g.getEndId()) {
				p2->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			}

			// Update and render all game objects
			for (int i = 0; i < gameObjects.size(); i++) {
				// Get the current object
				GameObject* currentGameObject = gameObjects[i];

				if (currentGameObject->getType().compare("tower") ){
	
					TowerObject *tower = static_cast<TowerObject *>(currentGameObject);
			
					tower->setCurrEnemy(p2);
					
				}

				// Updates game objects
				currentGameObject->update(deltaTime);

				//reset color uniform.
				GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
				glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);

				// Render game objects
				currentGameObject->render(shader);
			}

			//update graph
			g.update(p1->getCur(),toggleBlock, true);
			g.update(p2->getCur(),toggleBlock, false);
			//render graph
			g.render(shader);

			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window.getWindow());
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}

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
#include "HUD.h"


// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Pathfinding Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.15, 0.17, 0.21);
//const glm::vec3 viewport_background_color_g(1, 1, 1);

// Global texture info
GLuint tex[30];

// Global game object info
std::vector<GameObject*> gameObjects;
std::map<std::string, std::vector<EnemyObject*>*> enemyMap;


std::vector<TowerObject*> towerObjects;
std::vector<EnemyObject*> enemyObjects;
std::vector<HUD*> hudObjects;

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
	setthisTexture(tex[12], "Graphics/Tower/01_projectile.png");

	setthisTexture(tex[13], "Graphics/Explosion/tower_explode_1.png");
	setthisTexture(tex[14], "Graphics/Explosion/tower_explode_2.png");
	setthisTexture(tex[15], "Graphics/Explosion/tower_explode_3.png");
	setthisTexture(tex[16], "Graphics/Explosion/tower_explode_4.png");
	setthisTexture(tex[17], "Graphics/Explosion/tower_explode_5.png");
	setthisTexture(tex[18], "Graphics/Explosion/tower_explode_6.png");

	setthisTexture(tex[19], "Graphics/HUD/panel.png");//HUD image 

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

		std::vector<GLuint> explosion;
		for (int i = 0; i < 6; i++)explosion.push_back(tex[i + 13]);

		std::map<std::string,GLuint> texMap = std::map<std::string, GLuint>();
		texMap.insert(std::pair<std::string, GLuint >("0.5", tex[0]));
		texMap.insert(std::pair<std::string, GLuint >("0", tex[1]));
		texMap.insert(std::pair<std::string, GLuint >("1", tex[2]));

		texMap.insert(std::pair<std::string, GLuint >("2", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("3", tex[3]));
		texMap.insert(std::pair<std::string, GLuint >("4", tex[4]));
		texMap.insert(std::pair<std::string, GLuint >("5", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("6", tex[5]));
		texMap.insert(std::pair<std::string, GLuint >("ELSE", tex[7]));
		texMap.insert(std::pair<std::string, GLuint >("checkpoint", tex[8]));
		texMap.insert(std::pair<std::string, GLuint >("hp", tex[9]));
		texMap.insert(std::pair<std::string, GLuint >("tower", tex[10]));

		std::string fname = "Maps/Map1.csv";
		int wid = 0;
		int height = 0;
		int start = 1149;
		int end = 122;

		int hp = 5;
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
		std::cout << "got here at least" << std::endl;

		Graph g = Graph(wid, height, GameObject(glm::vec3(0.0f), tex[0], size,"map"),texMap, fname);
		std::cout << "got here at least" << std::endl;
		start = *(g.getBotStartSet().begin());
		std::cout << std::endl << start<<std::endl;
		std::cout << "got here at least" << std::endl;
		EnemyObject *p1 = new EnemyObject(glm::vec3(g.getNode(start).getX(), g.getNode(start).getY(), 0.0f), tex[6], size,"enemy");
		EnemyObject *p2 = new EnemyObject(glm::vec3(g.getNode(start).getX(), g.getNode(start).getY(), 0.0f), tex[6], size,"enemy");
		std::cout << "got here at least" << std::endl;
		p1->setSpeed(1.5);
		gameObjects.push_back(p1);
		gameObjects.push_back(p2);

		enemyMap["test"] = new std::vector<EnemyObject*>;
		enemyMap["test"]->push_back(p1);
		enemyMap["test"]->push_back(p2);
		std::cout << "got here at least" << std::endl;
		Node* cur = &g.getNode(start);
		for (EnemyObject* e : *enemyMap["test"]) {
			e->oldx = round(e->getPosition().x * 100) / 100;
			e->oldy = round(e->getPosition().y * 100) / 100;
			e->setCur(cur);
			e->setCurDestId(cur->getNextId());
		}

		std::cout << "got here at least" << std::endl;



		
		g.setStart(start);
		// Run the main loop
		double lastTime = glfwGetTime();
		
	
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

		float enemyX, enemyY, oldEnemyX, oldEnemyY;

		//-------------------------------------------------------------------------------- - |
		//							HUD CODE											   |
		//---------------------------------------------------------------------------------|
		glm::vec3 objectS = glm::vec3(0.5f, 0.8f, 0.0f);//this handels the size(scale) of the HUD panel 
		hudObjects.push_back(new HUD(glm::vec3(-1.55f, 0.91f, 0.0f), cameraZoom, objectS, tex[19], size, "HUD1") );//(position,camerazoom,scale,texture,numelemnets,type) **if you change the scale of the object you need to reposition it by changin it position.
		hudObjects.push_back(new HUD(glm::vec3(1.56f, 0.91f, 0.0f), cameraZoom, objectS, tex[19], size, "HUD3") );
		objectS.x = 1.25f;//this handels the size(scale) of the HUD panel
		hudObjects.push_back(new HUD(glm::vec3(0.01f, 0.91f, 0.0f), cameraZoom, objectS, tex[19], size, "HUD2") );


		std::cout << "got here at least" << std::endl;
		while (!glfwWindowShouldClose(window.getWindow())) {

	

			if (timeOfLastMove + 0.05 < glfwGetTime()) {
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
					cameraTranslatePos.y -= camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
					for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
					
					
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
					cameraTranslatePos.y += camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
					for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
					cameraTranslatePos.x -= camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
					for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
					cameraTranslatePos.x += camShiftInc * camShiftInc;
					g.setCamPos(cameraTranslatePos);
					for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
					cameraZoom = std::fmin(cameraZoom + camZoomInc, maxCamZoom);
					g.setZoom(cameraZoom);
					timeOfLastMove = glfwGetTime();
					for (HUD* h : hudObjects)h->setZoom(cameraZoom);

				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_X) == GLFW_PRESS) {
					cameraZoom = std::fmax(cameraZoom - camZoomInc, minCamZoom);
					g.setZoom(cameraZoom);
					timeOfLastMove = glfwGetTime();
					for (HUD* h : hudObjects)h->setZoom(cameraZoom);
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
						TowerObject* t = new TowerObject(glm::vec3(x, y, 0.0f), tex[10], tex[11], tex[12], explosion,size, "tower");
						towerObjects.push_back(t);
						//std::cout << g.getNode(id).getBuildable() << " => " << g.getNode(id).getPathable()<<std::endl;
						g.getNode(id).setTowerState(true);
						g.getNode(id).setTower(t);
						//std::cout << g.getNode(id).getBuildable() << " => " << g.getNode(id).getPathable()<<std::endl<<std::endl;
						if (!g.rePath(enemyMap["test"])) {
							std::cout << "\n\n\nSOUND THE FUCKING ALARMS";
						}
						std::cout << "Repath" << std::endl;

					}
					

				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS &&
					(timeOfLastMove + 0.15 < glfwGetTime())) { 
					std::cout << "spawn";
					Node* cur = &g.getNode(*g.getBotStartSet().begin());
					EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), tex[6], size, "enemy");
					enemyMap["test"]->push_back(e);
					gameObjects.push_back(e);
					e->setSpeed(1);
					e->oldx = round(e->getPosition().x * 100) / 100;
					e->oldy = round(e->getPosition().y * 100) / 100;
					e->setCur(cur);
					e->setCurDestId(cur->getNextId());
					timeOfLastMove = glfwGetTime();
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
			//glm::mat4 viewMatrix =  glm::translate(glm::mat4(1.0f), cameraTranslatePos) * glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
			shader.setUniformMat4("viewMatrix", viewMatrix);
			
			//animate p1
			//oldp1x = p1x;
			//oldp1y = p1y;

			for (HUD* h : hudObjects) {
				h->render(shader);
			}
			for (EnemyObject* e : *enemyMap["test"]) {
				oldEnemyX = e->oldx;
				oldEnemyY = e->oldy;

				enemyX = round(e->getPosition().x * 100) / 100;
				enemyY = round(e->getPosition().y * 100) / 100;

				e->oldx = enemyX;
				e->oldy = enemyY;

				cur = e->getCur();


				if (cur->getId() != g.getEndId() && cur->getNextNode(e->getCurDestId())!=NULL) {

					Node* next = cur->getNextNode(e->getCurDestId());
					float nextx = next->getX();
					float nexty = next->getY();


					//update velocity
					e->setTargetPos(glm::vec3(nextx, nexty, 0.0f));
					e->setDirection(glm::normalize(glm::vec3(nextx - enemyX, nexty - enemyY, 0.0f)));
					glm::vec3 dir = e->getDirection();
					dir.x = glm::round(dir.x);
					dir.y = glm::round(dir.y);
					e->setDirection(dir);
					//std::cout << e->getDirection().x << ", " << e->getDirection().y << std::endl;

					//if we walked over the node and we are not going to be at the end
					//std::cout << "oldx: " << oldp1x << "  x: " << p1x << "  nextx: " << nextx << std::endl;
					//std::cout << "oldY: " << oldp1y << "  y: " << p1y << "  nexty: " << nexty << std::endl;

					if ((oldEnemyY <= nexty && nexty <= enemyY) || (oldEnemyY >= nexty && nexty >= enemyY)) {
						//if (bounceY || true) {
						glm::vec3 pos = e->getPosition();
						pos.y = nexty;
						e->setPosition(pos);
						cur = next;
						e->setCur(cur);
						e->setCurDestId(cur->getNextId());
						//std::cout << "=> "<< (next->getNextNode() == NULL)<<std::endl;
						if (next->getNextNode(e->getCurDestId()) != NULL) {

							next = cur->getNextNode(e->getCurDestId());
							nextx = next->getX();
							nexty = next->getY();


							//update velocity
							e->setTargetPos(glm::vec3(nextx, nexty, 0.0f));
							e->setDirection(glm::normalize(glm::vec3(nextx - enemyX, nexty - enemyY, 0.0f)));
							glm::vec3 dir = e->getDirection();
							dir.x = glm::round(dir.x);
							dir.y = glm::round(dir.y);
							e->setDirection(dir);
							//std::cout << e->getDirection().x << ", " << e->getDirection().y << std::endl;
						}
						else {
							//std::cout << "this one";
							e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
						}
						//vel.y = 0;
						//}
						//++bounceY %= 2;
					}
					if ((oldEnemyX <= nextx && nextx <= enemyX) || (oldEnemyX >= nextx && nextx >= enemyX)) {
						//if (bounceX || true) {
						glm::vec3 pos = e->getPosition();
						pos.x = nextx;
						e->setPosition(pos);
						//vel.x = 0;
						cur = next;
						e->setCur(cur);
						e->setCurDestId(cur->getNextId());
						//}
						//++bounceX %= 2;
						if (next->getNextNode(e->getCurDestId()) != NULL) {

							next = cur->getNextNode(e->getCurDestId());
							nextx = next->getX();
							nexty = next->getY();


							//update velocity
							e->setTargetPos(glm::vec3(nextx, nexty, 0.0f));
							e->setDirection(glm::normalize(glm::vec3(nextx - enemyX, nexty - enemyY, 0.0f)));
							glm::vec3 dir = e->getDirection();
							dir.x = glm::round(dir.x);
							dir.y = glm::round(dir.y);
							e->setDirection(dir);
							//std::cout << e->getDirection().x << ", " << e->getDirection().y << std::endl;

						}
						else {
							//std::cout << "This two";
							e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
						}
						

					}


				}

				//stop moving if we're done
				if (cur->getId() == g.getEndId() || cur->getNextNode(e->getCurDestId()) == NULL) {
					--hp;
					//std::cout << "done, HP="<<hp<<std::endl;
					e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
					if (hp == 0) {
						std::cout << "GAME OVER" << std::endl;
					}

				}

			}
			

			for (TowerObject* t: towerObjects) {
				// Get the current object
				EnemyObject* closestEnemy = enemyMap["test"]->front();
				float enemyDistance = glm::length(t->getPosition() - closestEnemy->getPosition());
				for (EnemyObject* e : *enemyMap["test"]) {

					float tempDistance = glm::length(t->getPosition() - e->getPosition());
					if (tempDistance < enemyDistance) {
						enemyDistance = tempDistance;
						closestEnemy = e;
					}

				}
				t->setCurrEnemy(closestEnemy);
				// Updates game objects
				t->update(deltaTime);
				//reset color uniform.
				GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
				glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
				// Render game objects
				t->render(shader);
			}

			// Update and render all game objects
			for (int i = 0; i < gameObjects.size(); i++) {
				// Get the current object
				GameObject* currentGameObject = gameObjects[i];

				// Updates game objects
				currentGameObject->update(deltaTime);

				//reset color uniform.
				GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
				glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);

				// Render game objects
				currentGameObject->render(shader);
			}

			//update graph
			g.update(p1->getCur(),toggleBlock, false);
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


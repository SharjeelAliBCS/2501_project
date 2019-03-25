/* David Neudorf, Sharjeel Ali,Raul Rodriguez Azurdia
 * 101029913,101070889,
 Look in the README
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
#include "Graph.h"
#include "TowerObject.h"
#include "HUD.h"
#include "Text.h"


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

std::map<std::string, std::vector<GLuint>  > textures;
std::map<char, GLuint > fontTexture;

// Global game object info

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

GLuint createTexture(char *fname)
{
	GLuint w[1];
	glGenTextures(1,w);
	glBindTexture(GL_TEXTURE_2D, w[0]);

	float x = 0.2;
	float y = 0.4;
	

	float h = 0.1;
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(0.5, 0); glVertex2f(x+0.1, y);
	glTexCoord2f(0.5, 0.5); glVertex2f(x +0.2, y + h);
	glTexCoord2f(0, 0.5); glVertex2f(x, y + h);
	

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

	return w[0];
}

void setallTexture(void)
{
	
	textures["Map"].push_back(createTexture("Graphics/Map/0_0_boundary.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/0_1_boundary.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/1_empty.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/2_checkpoint.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/3_health.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/6_spawn.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/7_destructible.png"));

	textures["Enemy"].push_back(createTexture("Graphics/Enemy/0_enemy.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/1_enemy.png"));

	textures["Tower"].push_back(createTexture("Graphics/Tower/01_tower.png"));
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_turret.png"));
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_projectile.png"));

	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_1.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_2.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_3.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_4.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_5.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_6.png"));

	textures["UI"].push_back(createTexture("Graphics/HUD/panel.png"));
	textures["UI"].push_back(createTexture("Graphics/HUD/panelGameOver.png"));


	textures["Cursor"].push_back(createTexture("Graphics/Cursor/cursor.png"));

	std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz=0123456789.,;:$#'!\"/?%&()@";
	
	for (int i = 0; i < characters.size(); i++) {
		
		 
		std::string s = "Graphics/Text/text_"+ std::to_string(i + 1)+".png";
	
		char *cstr = &s[0u];
		std::cout << (i+1) << " " << characters[i] << " " << s << std::endl;
		fontTexture[characters[i]] = createTexture(cstr);
	}

}

// Main function that builds and runs the game
int main(void){
	try {

		/************************************************OPENGL INIT************************************************/

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

		/************************************************TEXTURE INIT************************************************/
		setallTexture();

		std::map<std::string,GLuint> texMap = std::map<std::string, GLuint>();
		texMap.insert(std::pair<std::string, GLuint >("0.5", textures["Map"][0]));//cliff
		texMap.insert(std::pair<std::string, GLuint >("0", textures["Map"][1]));//snow area
		texMap.insert(std::pair<std::string, GLuint >("1", textures["Map"][2]));//empty, buildable grass plains
		texMap.insert(std::pair<std::string, GLuint >("4", textures["Map"][6]));//unbuildable trees
		texMap.insert(std::pair<std::string, GLuint >("6", textures["Map"][5]));//enemy spawn portal
		texMap.insert(std::pair<std::string, GLuint >("checkpoint", textures["Map"][3]));//checkpoint
		texMap.insert(std::pair<std::string, GLuint >("hp", textures["Map"][4]));//end health
		
		GLuint cursorTex = textures["Cursor"][0];

		/************************************************VARIABLES INIT************************************************/

		int level = 1;
		std::string fname = "Levels/map"+std::to_string(level)+".csv";
		int wid = 0;
		int height = 0;
		int start = 1149;
		int end = 122;

		float hp = 5;
		float enemyHealth = 60.0f;

		bool gameOver = false;

		double lastTime = glfwGetTime();
		float timeOfLastMove = 0.0f;

		float maxCamZoom = 0.60f;
		float minCamZoom = 0.07f;
		float cameraZoom = 0.19f;
		glm::vec3 cameraTranslatePos(glm::vec3(0.0f));
		float camShiftInc = 0.5f;
		float camZoomInc = 0.01f;

		bool toggleBlock = false;

		float enemyX, enemyY, oldEnemyX, oldEnemyY;
		
		/************************************************FILE INIT************************************************/

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
		

		/************************************************GRAPH INIT************************************************/
		
		Graph g = Graph(wid, height, GameObject(glm::vec3(0.0f), textures["Map"][0],size,"map"),texMap, fname);
	
		start = *(g.getBotStartSet().begin());
		g.setZoom(cameraZoom);
		g.setCamPos(cameraTranslatePos);
	
		g.setStart(start);

	
		/************************************************ENEMY INIT************************************************/

		EnemyObject *p1 = new EnemyObject(glm::vec3(g.getNode(start).getX(), g.getNode(start).getY(), 0.0f), textures["Enemy"][0], size, enemyHealth,"enemy");
		EnemyObject *p2 = new EnemyObject(glm::vec3(g.getNode(start).getX(), g.getNode(start).getY(), 0.0f), textures["Enemy"][0], size, enemyHealth,"enemy");
		
		p1->setSpeed(1.5);
	
		enemyMap["Origin"] = new std::vector<EnemyObject*>;
		enemyMap["Origin"]->push_back(p1);
		enemyMap["Origin"]->push_back(p2);

		Node* cur = &g.getNode(start);
		for (EnemyObject* e : *enemyMap["Origin"]) {
			e->oldx = round(e->getPosition().x * 100) / 100;
			e->oldy = round(e->getPosition().y * 100) / 100;
			e->setCur(cur);
			e->setCurDestId(cur->getNextId());
		}

	
		/************************************************HUD INIT************************************************/

		GameObject* cursor = new GameObject(glm::vec3(0.0f), cursorTex, size, "cursor");

		glm::vec3 objectS = glm::vec3(0.5f, 0.8f, 0.0f);//this handels the size(scale) of the HUD panel 
		hudObjects.push_back(new HUD(glm::vec3(-1.55f, 0.91f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, "HUD1") );//(position,camerazoom,scale,texture,numelemnets,type) **if you change the scale of the object you need to reposition it by changin it position.
		hudObjects.push_back(new HUD(glm::vec3(1.56f, 0.91f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, "HUD3") );
		objectS.x = 1.25f;//this handels the size(scale) of the HUD panel
		hudObjects.push_back(new HUD(glm::vec3(0.01f, 0.91f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, "HUD2") );

		/************************************************TEXT INIT************************************************/
		hudObjects[2]->addText(new Text(glm::vec3(1.5f, 4.5f, 0.0f), fontTexture, "Player ", size, 0.2f,glm::vec3(255, 255, 255)));
		hudObjects[2]->addText(new Text(glm::vec3(-4.5f,-6.0f,0.0f), fontTexture, "Enemies = ", size, 0.1f, glm::vec3(50,175,255)));
		

		/************************************************GAME LOOP************************************************/
	
		while (!glfwWindowShouldClose(window.getWindow())) {

			/************************************************KEY INPUT********************************************/
		
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
					toggleBlock = !toggleBlock;
					timeOfLastMove = glfwGetTime();

				}
				if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
	
					float x;
					float y;
					int id = g.getHover();
					g.getHoverCoords(x,y);
					if (g.getNode(id).getBuildable()) {
						TowerObject* t = new TowerObject(glm::vec3(x, y, 0.0f), textures["Tower"], textures["Explosion"], size, 10,"tower");

						g.getNode(id).setTowerState(true);

						if (g.rePath(enemyMap["Origin"], id)) {
							towerObjects.push_back(t);
							g.getNode(id).setTower(t);
						}
						else {
							std::cout << "\n\n\nINVALID TOWER PLACEMENT";
							g.getNode(id).setTowerState(false);
							g.rePath(enemyMap["Origin"], id);
							delete(t);
						}
						std::cout << "Repath" << std::endl;

					}
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS &&
					(timeOfLastMove + 0.15 < glfwGetTime())) { 
					
					std::cout << "Spawned new enemy. Total: " << enemyMap["Origin"]->size()+1 <<  std::endl;
					Node* cur = &g.getNode(*g.getBotStartSet().begin());
					EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), textures["Enemy"][0], size, enemyHealth,"enemy");
					enemyMap["Origin"]->push_back(e);
					
					e->setSpeed(1);
					e->oldx = round(e->getPosition().x * 100) / 100;
					e->oldy = round(e->getPosition().y * 100) / 100;
					e->setCur(cur);
					e->setCurDestId(cur->getNextId());
					timeOfLastMove = glfwGetTime();
				}

			}
		
			window.clear(viewport_background_color_g);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// Select proper shader program to use
			shader.enable();

			// Setup camera to focus on zoom center
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);
		
			/************************************************OBJECT UPDATE/RENDERING********************************************/
			
			

			//**********HUD**********
			for (HUD* h : hudObjects) {
				h->update(deltaTime);
			
				for (Text* t : h->getTextObjects()) {
					
					if (t->getType().compare("Enemies = ")==0) {
						std::string temp = t->getText() + std::to_string(enemyMap["Origin"]->size());
						t->setRenderedText(temp);
					}
				}
				
				h->render(shader);
			}

			//**********Cursor**********
			float x, y;
			int id = g.getHover();
			g.getHoverCoords(x, y);
			cursor->setPosition(glm::vec3(x,y,0.0f));
			if (g.getNode(id).getBuildable()) {

				cursor->render(shader);
			}
			else {
				//glBlendColor(1.0f, 1.0f, 1.0f, 0.05f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_SRC_ALPHA);
				//glUniform4f(glGetUniformLocation(shader.getShaderID(), "colorMod"), 0.0f, 0.0f, 0.0f,0.5f);	//dark green
				glUniform3f(glGetUniformLocation(shader.getShaderID(), "colorMod"), 0.5f, -1.0f, -1.0f);
				cursor->render(shader);
				glEnable(GL_LIGHTING);
			
				glBlendFunc(GL_SRC_ALPHA, GL_ZERO),
				glUniform3f(glGetUniformLocation(shader.getShaderID(), "colorMod"), 0.0f, 0.0f, 0.0f);	//dark green
			}

			//**********Tower**********
			for (TowerObject* t : towerObjects) {

				EnemyObject* closestEnemy;

				if (enemyMap["Origin"]->size() == 0)closestEnemy = NULL;
				else {

					//get the closest enemy to the tower and point to it using closestEnemy
					closestEnemy = enemyMap["Origin"]->front();
					float enemyDistance = glm::length(t->getPosition() - closestEnemy->getPosition());

					for (EnemyObject* e : *enemyMap["Origin"]) {

						float tempDistance = glm::length(t->getPosition() - e->getPosition());
						if (tempDistance < enemyDistance) {
							enemyDistance = tempDistance;
							closestEnemy = e;
						}

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

			//**********Enemy**********
			std::deque<int> deleteEnemies;
			int delIndex = 0;

			for (EnemyObject* e : *enemyMap["Origin"]) {
				oldEnemyX = e->oldx;
				oldEnemyY = e->oldy;

				enemyX = round(e->getPosition().x * 100) / 100;
				enemyY = round(e->getPosition().y * 100) / 100;

				e->oldx = enemyX;
				e->oldy = enemyY;

				cur = e->getCur();
				
				if (!e->getExists()) {
					std::cout << "Enemy despawned/destroyed. Total: " << enemyMap["Origin"]->size()-1 << std::endl;
					deleteEnemies.push_front(delIndex);
				}

					delIndex++;



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
					
					if ((oldEnemyY <= nexty && nexty <= enemyY) || (oldEnemyY >= nexty && nexty >= enemyY)) {
						
						glm::vec3 pos = e->getPosition();
						pos.y = nexty;
						e->setPosition(pos);
						cur = next;
						e->setCur(cur);
						e->setCurDestId(cur->getNextId());
					
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
							
						}
						else {
						
							e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
						}
				
					}
					if ((oldEnemyX <= nextx && nextx <= enemyX) || (oldEnemyX >= nextx && nextx >= enemyX)) {
						
						glm::vec3 pos = e->getPosition();
						pos.x = nextx;
						e->setPosition(pos);
						cur = next;
						e->setCur(cur);
						e->setCurDestId(cur->getNextId());
					
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
							
						}
						else {
					
							e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
						}
						

					}
				}

				//stop moving if we're done
				if (cur->getId() == g.getEndId() || cur->getNextNode(e->getCurDestId()) == NULL) {
					hp -= 0.5;
					std::cout << "hp = " << hp <<  std::endl;
					
					e->setExists(false);
					if (hp == 0) {
						gameOver = true;
						std::cout << "GAME OVER" << std::endl;
						hudObjects[2]->setTex(textures["UI"][1]);
						
					}

				}

				e->update(deltaTime);

				//reset color uniform.
				GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
				glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);

				// Render game objects
				e->render(shader);

			}

			//using the indecies, delete the enemies that should be deleted. 
			for (int i = 0; i < deleteEnemies.size(); i++)enemyMap["Origin"]->erase(enemyMap["Origin"]->begin() + deleteEnemies[i]);
		
			//**********Graph**********
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


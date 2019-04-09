/* David Neudorf, Sharjeel Ali,Raul Rodriguez Azurdia
 * 101029913,101070889,
 Look in the README
 */

#include <limits>
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

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")


#include "Shader.h"
#include "Window.h"
#include "Graph.h"
#include "TowerObject.h"
#include "HUD.h"
#include "Text.h"
#include "Particle.h"
#include "Audio.h"
#include "PowerUpObject.h"
#include "UpgradeObject.h"
#include "FileUtils.h"



// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

float GameObject::cameraZoom = 0.22f;
glm::vec3 GameObject::cameraTranslatePos = glm::vec3(0.0f);

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Assault on Terra";
float ratio = 0.75f;
float window_width_g = 1000;//*1.5
float window_height_g = window_width_g * ratio;//*1.5
int Wwidth, Wheight;
float factor = window_width_g / window_width_g;//*1.5
//const glm::vec3 viewport_background_color_g(1, 1, 1);
const glm::vec3 viewport_background_color_g(50.0f/255.0f, 50.0f /255.0f, 50.0f /255.0f);

// Global texture info

std::map<std::string, std::vector<GLuint>  > textures;
std::map<char, GLuint > fontTexture;

// Global game object info

std::map<int, std::vector<EnemyObject*>*> enemyMap;
std::map<int, std::vector<TowerObject*>*> towerMap;


std::vector<TowerObject*> towerObjects;
std::vector<EnemyObject*> enemyObjects;
std::vector<HUD*> hudObjects;
std::vector<TowerObject*> blueprints;
std::vector<std::string> gameState;
std::vector<HUD*> HUDMenu;
std::vector<GameObject*>buttonTowerPanel;
std::vector<GameObject*>buttonEnemyPanel;
std::vector<GameObject*>turnButtons;
std::vector<GameObject*>button;
std::vector<EnemyObject*> enemyBlueprint;
std::vector<Shader*> shaders;
//===================================recently made
std::vector<EnemyObject*> enemyCounters;
std::vector<EnemyObject*> enemyCounters2;
std::vector<HUD*>player1;
std::vector<HUD*>player2;
std::vector<TowerObject*> towerDetailHUD;
std::vector<EnemyObject*> enemyDetailHUD;
std::vector<PowerUpObject*> powerUpListHUD;
std::vector<UpgradeObject*> upgradeListHUD;
std::vector<PowerUpObject*> powerUpList;
std::vector<UpgradeObject*> upgradeList;

int normalCount = 10000, fastCount = 10000, heartyCount = 10000, flyingCount = 10000, splitterCount = 10000, regeneratingCount = 10000, fast_heartyCount = 10000, fast_flyingCount = 10000, fast_regeneratingCount = 10000, fast_splitterCount = 10000, fast_hearty_regenerating_flyingCount = 10000;
int normalCount2 = 10000, fastCount2 = 10000, heartyCount2 = 10000, flyingCount2 = 10000, splitterCount2 = 10000, regeneratingCount2 = 10000, fast_heartyCount2 = 10000, fast_flyingCount2 = 10000, fast_regeneratingCount2 = 10000, fast_splitterCount2 = 10000, fast_hearty_regenerating_flyingCount2 = 10000;
enum State {
	MainMenu,
	Game,
	Pause,
	GameOver,
};


State _state;

void ResizeCallback(GLFWwindow*window, int width, int height) {
	if (width != window_width_g) {
		height = width * ratio;
	}
	if (height != window_height_g) {
		width = height / ratio;
	}
	//gluOrtho2D(-0.5f*ratio, 0.5*ratio, 0, 1);
	std::cout << width << "," << height << std::endl;
	window_width_g = width;
	window_height_g = height;
	glViewport(0, 0, width, height);

}

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

	textures["Map"].push_back(createTexture("Graphics/Map/0_0_boundary.png"));//0
	textures["Map"].push_back(createTexture("Graphics/Map/0_1_boundary.png"));//1
	textures["Map"].push_back(createTexture("Graphics/Map/1_empty.png"));//2
	textures["Map"].push_back(createTexture("Graphics/Map/2_checkpoint.png"));//3
	textures["Map"].push_back(createTexture("Graphics/Map/3_health.png"));//4
	textures["Map"].push_back(createTexture("Graphics/Map/6_spawn.png"));//5
	textures["Map"].push_back(createTexture("Graphics/Map/7_destructible.png"));//6

	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_01.png"));//7
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_02.png"));//8
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_03.png"));//9
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_04.png"));//10
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_05.png"));//11
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_06.png"));//12
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_07.png"));//13
	textures["Map"].push_back(createTexture("Graphics/Map/building_1/0_Boundary_08.png"));//14

	textures["Map"].push_back(createTexture("Graphics/Map/building_2/1_Boundary_01.png"));//13
	textures["Map"].push_back(createTexture("Graphics/Map/building_2/1_Boundary_02.png"));//14

	textures["Map"].push_back(createTexture("Graphics/Map/building_3/2_Boundary_03.png"));//9
	textures["Map"].push_back(createTexture("Graphics/Map/building_3/2_Boundary_04.png"));//10
	textures["Map"].push_back(createTexture("Graphics/Map/building_3/2_Boundary_05.png"));//11
	textures["Map"].push_back(createTexture("Graphics/Map/building_3/2_Boundary_06.png"));//12
	textures["Map"].push_back(createTexture("Graphics/Map/building_3/2_Boundary_07.png"));//13
	textures["Map"].push_back(createTexture("Graphics/Map/building_3/2_Boundary_08.png"));//14

	textures["Enemy"].push_back(createTexture("Graphics/Enemy/normal.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/glutton.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/speedster.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/regenerator.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/speedsterGlutton.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/regeneratingGlutton.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/regeneratingSpeedster.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/regeneratingSpeedsterGlutton.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/undying.png"));

	textures["Particle"].push_back(createTexture("Graphics/Particles/fire.png"));
	textures["Particle"].push_back(createTexture("Graphics/Particles/enemyDeath.png"));

	//basic
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_tower.png"));//0
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_turret.png"));//1
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_projectile.png"));//2
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_towerIcon.png"));//3

	//defender
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_tower.png"));//4
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_turret.png"));//5
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_projectile.png"));//6
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_towerIcon.png"));//7

	//barrier
	textures["Tower"].push_back(createTexture("Graphics/Tower/05_barrier.png"));//11
	textures["Tower"].push_back(createTexture("Graphics/Tower/05_towerIcon.png"));//8
	textures["Tower"].push_back(createTexture("Graphics/Tower/05_towerIcon.png"));//9
	textures["Tower"].push_back(createTexture("Graphics/Tower/05_towerIcon.png"));//11
	//flamethrower
	textures["Tower"].push_back(createTexture("Graphics/Tower/03_tower.png"));//8
	textures["Tower"].push_back(createTexture("Graphics/Tower/03_turret.png"));//9
	textures["Tower"].push_back(textures["Particle"][0]);//10
	textures["Tower"].push_back(createTexture("Graphics/Tower/03_towerIcon.png"));//11

	textures["Tower"].push_back(createTexture("Graphics/Tower/8_tower.png"));//4
	textures["Tower"].push_back(createTexture("Graphics/Tower/8_tower.png"));//5
	textures["Tower"].push_back(createTexture("Graphics/Tower/8_radius.png"));//6
	textures["Tower"].push_back(createTexture("Graphics/Tower/8_towericon.png"));//7

	textures["Tower"].push_back(createTexture("Graphics/Tower/06_tower.png"));//4
	textures["Tower"].push_back(createTexture("Graphics/Tower/06_turret.png"));//5
	textures["Tower"].push_back(createTexture("Graphics/Tower/06_projectile.png"));//6
	textures["Tower"].push_back(createTexture("Graphics/Tower/06_towerIcon.png"));//7

	textures["Tower"].push_back(createTexture("Graphics/Tower/04_auto.png"));//4
	textures["Tower"].push_back(createTexture("Graphics/Tower/04_auto.png"));//5
	textures["Tower"].push_back(createTexture("Graphics/Tower/06_projectile.png"));//6
	textures["Tower"].push_back(createTexture("Graphics/Tower/04_auto.png"));//7

	textures["Tower"].push_back(createTexture("Graphics/Tower/07_tower.png"));//4
	textures["Tower"].push_back(createTexture("Graphics/Tower/07_turret.png"));//5
	textures["Tower"].push_back(createTexture("Graphics/Tower/07_projectile.png"));//6
	textures["Tower"].push_back(createTexture("Graphics/Tower/07_towerIcon.png"));//7


	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_1.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_2.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_3.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_4.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_5.png"));
	textures["Explosion"].push_back(createTexture("Graphics/Explosion/tower_explode_6.png"));

	textures["UI"].push_back(createTexture("Graphics/HUD/panel_player1.png"));
	textures["UI"].push_back(createTexture("Graphics/HUD/panel_player2.png"));


	textures["Cursor"].push_back(createTexture("Graphics/Cursor/cursor.png"));
	textures["Cursor"].push_back(createTexture("Graphics/Cursor/select.png"));

	textures["MENU"].push_back(createTexture("Graphics/Buttons/placeholder.png"));//0
	textures["MENU"].push_back(createTexture("Graphics/Buttons/placeholder.png"));//0
	textures["MENU"].push_back(createTexture("Graphics/Buttons/placeholder.png"));//0
	textures["MENU"].push_back(createTexture("Graphics/Buttons/placeholder.png"));//0

	textures["Background"].push_back(createTexture("Graphics/Background/menu.png"));//0

	textures["Player"].push_back(createTexture("Graphics/playerStats/hp.png"));
	textures["Player"].push_back(createTexture("Graphics/playerStats/gold.png"));
	textures["Player"].push_back(createTexture("Graphics/playerStats/income.png"));
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Play.png"));//0
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Quit.png"));//1
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Score.png"));//2
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Option.png"));//3

	textures["Button"].push_back(createTexture("Graphics/Buttons/placeholder.png"));//0

																					//Text is rendered by creating a map where each key is a character that corrasponds to an actual character.png file. 
	std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz=0123456789.,;:$#'!\"/?%&()@-";

	for (int i = 0; i < characters.size(); i++) {

		std::string s = "Graphics/Text/text_" + std::to_string(i + 1) + ".png";

		char *cstr = &s[0u];
		std::cout << (i + 1) << " " << characters[i] << " " << s << std::endl;
		fontTexture[characters[i]] = createTexture(cstr);
	}

}

void setAudioTracks(Audio* audioObject) {

	audioObject->addAudio("Audio/Soundtrack/infected.mp3", "background");
	audioObject->volume("background", 20);
	//audioObject->playRepeat("background");
	audioObject->addAudio("Audio/Soundtrack/mainMenu.mp3", "menu");
	audioObject->volume("menu", 100);
	audioObject->playRepeat("menu");

	audioObject->addAudio("Audio/Towers/rocket.mp3", "bullet");
	audioObject->volume("bullet", 100);
	audioObject->addAudio("Audio/Towers/place.mp3", "towerPlaced");
	audioObject->volume("towerPlaced", 30);
	audioObject->addAudio("Audio/Towers/ship.mp3", "ship");
	audioObject->volume("ship", 30);
	audioObject->addAudio("Audio/Towers/laser.mp3", "laser");
	audioObject->addAudio("Audio/Towers/cooldown.mp3", "cooldown");
	audioObject->volume("cooldown", 30);

	audioObject->addAudio("Audio/HUD/menuClick.mp3", "menuClick");
	audioObject->volume("menuClick", 100);
	audioObject->addAudio("Audio/HUD/teamChange.mp3", "teamChange");
	audioObject->volume("teamChange", 200);
	audioObject->addAudio("Audio/HUD/enemySelected.mp3", "enemySelected");
	audioObject->volume("enemySelected", 100);

	audioObject->addAudio("Audio/Enemy/enemy.mp3", "enemyDeath");
	audioObject->volume("enemyDeath", 100);

	audioObject->addAudio("Audio/Voice/BaseUnderAttack.mp3", "baseAttack");
	audioObject->volume("baseAttack", 100);
	audioObject->addAudio("Audio/Voice/EnemiesApproaching.mp3", "enemiesComing");
	audioObject->volume("EnemiesComing", 100);
	audioObject->addAudio("Audio/Voice/EnemyEliminated.mp3", "enemiesDestroyed");
	audioObject->volume("EnemiesDestroyed", 100);
	audioObject->addAudio("Audio/Voice/UnitReady.mp3", "unitReady");
	audioObject->volume("unitReady", 100);



}

std::vector<EnemyObject*> enemiesInRange(Node &n, float range, std::vector<EnemyObject*>* creeps) {
	if (range == -1) {
		return *creeps;
	}
	std::vector<EnemyObject*> targets;
	float x = n.getX();
	float y = n.getY();
	for (EnemyObject* e : *creeps) {
		if (pow((e->getPosition().x - x),2) + pow(0.75*(e->getPosition().y - y),2) < pow(range,2)) {
			targets.push_back(e);
		}
	}
	return targets;
}


// Main function that builds and runs the game
int main(void){
	try {

		/************************************************AUDIO INIT************************************************/


		Audio* audioObject = new Audio();
		setAudioTracks(audioObject);

		/************************************************OPENGL INIT************************************************/

		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		if (!glfwInit()) {
			throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
		}
		// Setup window
		//Window window(window_width_g, window_height_g, window_title_g);
		GLFWwindow* window;
		window = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
		if (!window) {
			glfwTerminate();
			throw(std::runtime_error(std::string("Could not create window")));
		}

		/* Make the window's OpenGL context the current one */
		glfwMakeContextCurrent(window);


		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
		}
		glfwSetWindowAspectRatio(window, 4, 3);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create geometry of the square
		int size = CreateSquare();

		/************************************************SHADER INIT************************************************/
		
		shaders.push_back(new Shader("shader.vert", "shader.frag", 0));
		shaders[0]->disable();
		shaders.push_back(new Shader("shaderParticle.vert", "shaderParticle.frag", 1));
		shaders[1]->CreateParticleArray(1);
		shaders[1]->disable();
		shaders.push_back(new Shader("shaderEnemyDeath.vert", "shaderEnemyDeath.frag", 2));
		shaders[2]->CreateParticleArray(2);
		shaders[2]->setRadius(2);
		shaders[2]->disable();

		shaders[0]->setAttribute(0);
		
		

		/************************************************TEXTURE INIT************************************************/
		setallTexture();

		std::map<std::string, GLuint> texMap = std::map<std::string, GLuint>();
		texMap.insert(std::pair<std::string, GLuint >("0.5", textures["Map"][0]));//cliff
		texMap.insert(std::pair<std::string, GLuint >("0", textures["Map"][1]));//snow area
		texMap.insert(std::pair<std::string, GLuint >("T", textures["Map"][2]));//empty, buildable grass plains
		texMap.insert(std::pair<std::string, GLuint >("B", textures["Map"][2]));//empty, buildable grass plains
		texMap.insert(std::pair<std::string, GLuint >("4", textures["Map"][6]));//unbuildable trees
		texMap.insert(std::pair<std::string, GLuint >("6", textures["Map"][5]));//enemy spawn portal
		texMap.insert(std::pair<std::string, GLuint >("checkpoint", textures["Map"][3]));//checkpoint
		texMap.insert(std::pair<std::string, GLuint >("hp", textures["Map"][4]));//end health

		texMap.insert(std::pair<std::string, GLuint >("b0_1", textures["Map"][7]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_2", textures["Map"][8]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_3", textures["Map"][9]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_4", textures["Map"][10]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_5", textures["Map"][11]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_6", textures["Map"][12]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_7", textures["Map"][13]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b0_8", textures["Map"][14]));//end health

		texMap.insert(std::pair<std::string, GLuint >("b1_1", textures["Map"][15]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b1_2", textures["Map"][16]));//end health

		texMap.insert(std::pair<std::string, GLuint >("b2_1", textures["Map"][17]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b2_2", textures["Map"][18]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b2_3", textures["Map"][19]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b2_4", textures["Map"][20]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b2_5", textures["Map"][21]));//end health
		texMap.insert(std::pair<std::string, GLuint >("b2_6", textures["Map"][22]));//end health


		GLuint cursorTex = textures["Cursor"][0];
		GLuint selectGraphicTex = textures["Cursor"][1];

		/************************************************VARIABLES INIT************************************************/
		char turnArr[2] = { 'B','T' };
		long income[2] = { 20,20 };
		long credits[2] = { 30,30 };
		glm::vec3 hudColors[2] = { glm::vec3(50, 175, 255),glm::vec3(179, 0, 0) };

		int hp[2] = { 20,20 };
		int turnIndex = 0;
		char turn = turnArr[turnIndex];

		int level = 2;
		std::string fname = "Levels/map"+std::to_string(level)+".csv";
		int wid = 0;
		int height = 0;
		int start = 1149;
		int end = 122;

		float enemyHealth = 60.0f;
		int showRadius = 0;
		float selectionRadius = 1.5;

		bool gameOver = false;

		double lastTime = glfwGetTime();
		float timeOfLastMove = 0.0f;

		float maxCamZoom = 0.60f;
		float minCamZoom = 0.10f;
		float cameraZoom = 0.22f;
		
		float camShiftInc = 0.05f;
		float camZoomInc = 0.01f;

		bool gottaGoFast = false;
		bool hpUp = false;
		bool rainingLead = false;
		float rainingLeadEnd = 0;

		EnemyObject* selectedEnemy = NULL;

		float enemyX, enemyY, oldEnemyX, oldEnemyY;
		float spawnCount = 0;
		bool startWave;
		bool toggleBlock = false;

		GameObject* background = new GameObject(glm::vec3(3.0f, -6.3f, 0.0f), textures["Background"][0], size, "map");
		background->setImgScale(glm::vec3(90, 46, 46));

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
		in.close();

		FileUtils descriptions;

		/************************************************GRAPH INIT************************************************/
		
		Graph g = Graph(wid, height, GameObject(glm::vec3(0.0f), textures["Map"][0], size, "map"), texMap, fname, window_width_g, window_height_g, window);
		glm::vec3 cameraTranslatePos = g.getFocalPoint(turnIndex);
		start = *(g.getBotStartSet().begin());
		g.setZoom(cameraZoom);
		g.setCamPos(cameraTranslatePos);

		g.setStart(start);

	
		/************************************************ENEMY INIT************************************************/

		enemyMap[0] = new std::vector<EnemyObject*>;
		enemyMap[1] = new std::vector<EnemyObject*>;

		towerMap[0] = new std::vector<TowerObject*>;
		towerMap[1] = new std::vector<TowerObject*>;


		Node* cur;
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NEW STUFF
		glm::vec3 powerUpStartPosition = glm::vec3(3.0f, 6.0f, 0.0f);
		for (int i = 0; i < 5;i++) {
			powerUpList.push_back(new PowerUpObject(powerUpStartPosition, textures["MENU"][0],size,"power"+std::to_string(i)));
			powerUpStartPosition.x -= 1.5;
		}
		glm::vec3 upgradeUpStartPosition = glm::vec3(-6.0f, 7.3f, 0.0f);
		for (int i = 0; i < 3; i++) {
			upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][0], size, "upgradeTower" + std::to_string(i)));
			upgradeUpStartPosition.y += 1;
		}
		upgradeUpStartPosition = glm::vec3(6.0f, 7.3f, 0.0f);
		for (int i = 0; i < 3; i++) {
			upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][0], size, "upgradeEnemy" + std::to_string(i)));
			upgradeUpStartPosition.y += 1;
		}
		for (int i = 0; i < 5; i++) {
			powerUpListHUD.push_back(new PowerUpObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["MENU"][0], size, "power" + std::to_string(i)));
		}
		for (int i = 0; i < 6; i++) {
			upgradeListHUD.push_back(new UpgradeObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["MENU"][0], size, "upgrade" + std::to_string(i)));
		}
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NEW STUFF
		int index2 = 0;
		
		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 10, "C-Class BASIC", 3, 0.5, 5, 0.2)); index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 30, "C-Class DEFENDER", 2, 1, 10, 0.4)); index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 0, "C-Class BARRIER", 0, 0, 1, 0.0)); index2 += 4;

		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 7, "B-Class IGNITION Cannon", 1, 0.1, 10, 0.1)); index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 20, "AOE", 1, 1, 30, 4));index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 200, "A-Class Stealth Sniper", 20, 4, 100, 0.6));index2 += 4;

		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 200, "A-Class Auto Assault Bomber", 1.5, 2, 300, 0.5));index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(3.7f, 8.0f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 5, "A-Class High Beam Laser", 30, 1, 200, 0));


		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][0], size, 50, "Servent Minor", textures["Particle"][1], 1.0, 10, 0));//0
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][1], size, 200, "Glutton Minor", textures["Particle"][1], 0.8, 40, 0));//1
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][2], size, 100, "Fleeting Minor", textures["Particle"][1], 1.7, 70, 0));//2

		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][3], size, 150, "Lifegiver Minor", textures["Particle"][1], 1, 100, 10));//3
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][4], size, 350, "Fleeting Colossal", textures["Particle"][1], 1.5, 250, 0));//4
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][5], size, 400, "Life Colossal", textures["Particle"][1], 1.0, 400, 15));//5

		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][6], size, 300, "Fleeting Lifeblood", textures["Particle"][1], 1.6, 500, 15));//6
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][7], size, 1000, "Absolute Zenith", textures["Particle"][1], 2.0, 1500, 20));//7
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(3.7f, 8.0f, 0.0f), textures["Enemy"][8], size, 1000000, "Undying Zenith", textures["Particle"][1], 2.5, 1000000, 1000000));//7

		towerDetailHUD[0]->setDescription(descriptions.LoadTextFile("Descriptions/test.txt"));
		
		/************************************************blueprints INIT************************************************/

		//The blueprints are used to store a single type of tower, then once the player has placed one, it will use its variables
		//to create the actual tower object. 
		int index = 0;	
																																																				//damage              range  rof  cost  speed								
		blueprints.push_back(new TowerObject(glm::vec3(-7.35f, 7.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10, "C-Class BASIC", 3, 0.5, 5, 0.2)); index += 4; //the final 5 here is the cost
		blueprints.push_back(new TowerObject(glm::vec3(-8.35f, 7.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 30, "C-Class DEFENDER", 2, 1, 10, 0.4)); index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.35f, 7.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 0, "C-Class BARRIER", 0, 0, 1, 0.0)); index += 4;
		
		blueprints.push_back(new TowerObject(glm::vec3(-7.35f, 8.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 7, "B-Class IGNITION Cannon", 1, 0.1, 10, 0.1)); index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-8.35f, 8.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Button"], size, 20, "AOE", 1, 1, 30, 4));index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.35f, 8.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 200, "A-Class Stealth Sniper", 20, 4, 100, 0.6)); index += 4;
		
		blueprints.push_back(new TowerObject(glm::vec3(-7.35f, 9.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 200, "A-Class Auto Assault Bomber", 1.5, 2, 300, 0.5));index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-8.35f, 9.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 5, "A-Class High Beam Laser", 30, 1, 200, 0));//index += 4;
		
	
		
		/************************************************enemyBlueprints INIT************************************************/
//																								         health							                                     speed, cost regen
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.35f, 7.3f, 0.0f), textures["Enemy"][0], size, 50,      "Servent Minor",                         textures["Particle"][1], 1.0, 10,   0));//0
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.35f, 7.3f, 0.0f), textures["Enemy"][1], size, 200,     "Glutton Minor",                        textures["Particle"][1], 0.8, 40,   0));//1
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.35f, 7.3f, 0.0f), textures["Enemy"][2], size, 100,     "Fleeting Minor",                      textures["Particle"][1], 1.7, 70,   0));//2
		
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.35f, 8.3f, 0.0f), textures["Enemy"][3], size, 150,     "Lifegiver Minor",                    textures["Particle"][1], 1,   100,  10));//3
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.35f, 8.3f, 0.0f), textures["Enemy"][4], size, 350,     "Fleeting Colossal",              textures["Particle"][1], 1.5, 250,  0));//4
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.35f, 8.3f, 0.0f), textures["Enemy"][5], size, 400,     "Life Colossal",           textures["Particle"][1], 1.0, 400,  15));//5
		
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.35f, 9.3f, 0.0f), textures["Enemy"][6], size, 300,     "Fleeting Lifeblood",        textures["Particle"][1], 1.6, 500,  15));//6
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.35f, 9.3f, 0.0f), textures["Enemy"][7], size, 1000,    "Absolute Zenith", textures["Particle"][1], 2.0, 1500, 20));//7
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.35f, 9.3f, 0.0f), textures["Enemy"][8], size, 1000000, "Undying Zenith",                        textures["Particle"][1], 2.5, 1000000,   1000000));//7
																																						  
		/*for (TowerObject* t : blueprints) {
			towerDetailHUD.push_back(t);
		}
		for (EnemyObject* e : enemyBlueprint) {
			enemyDetailHUD.push_back(e);
		}*/
		/************************************************buttonBlueprints INIT************************************************/
																																						  /************************************************enemyCounters INIT************************************************/
		enemyCounters.push_back(new EnemyObject(glm::vec3(9.3f, 2.4f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters.push_back(new EnemyObject(glm::vec3(6.8f, 2.4f, 0.0f), textures["Enemy"][1], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters.push_back(new EnemyObject(glm::vec3(4.3f, 2.4f, 0.0f), textures["Enemy"][2], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3

		enemyCounters.push_back(new EnemyObject(glm::vec3(9.3f, 3.4f, 0.0f), textures["Enemy"][3], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters.push_back(new EnemyObject(glm::vec3(6.8f, 3.4f, 0.0f), textures["Enemy"][4], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters.push_back(new EnemyObject(glm::vec3(4.3f, 3.4f, 0.0f), textures["Enemy"][5], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3

		enemyCounters.push_back(new EnemyObject(glm::vec3(9.3f, 4.4f, 0.0f), textures["Enemy"][6], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters.push_back(new EnemyObject(glm::vec3(6.8f, 4.4f, 0.0f), textures["Enemy"][7], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters.push_back(new EnemyObject(glm::vec3(4.3f, 4.4f, 0.0f), textures["Enemy"][8], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3

		enemyCounters2.push_back(new EnemyObject(glm::vec3(-2.7f, 2.4f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-5.2f, 2.4f, 0.0f), textures["Enemy"][1], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-7.7f, 2.4f, 0.0f), textures["Enemy"][2], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3

		enemyCounters2.push_back(new EnemyObject(glm::vec3(-2.7f, 3.4f, 0.0f), textures["Enemy"][3], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-5.2f, 3.4f, 0.0f), textures["Enemy"][4], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-7.7f, 3.4f, 0.0f), textures["Enemy"][5], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3

		enemyCounters2.push_back(new EnemyObject(glm::vec3(-2.7f, 4.4f, 0.0f), textures["Enemy"][6], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-5.2f, 4.4f, 0.0f), textures["Enemy"][7], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-7.7f, 4.4f, 0.0f), textures["Enemy"][8], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3
																																								 //button.push_back(new GameObject(glm::vec3(-7.5f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));
																																								 /************************************************buttonBlueprints INIT************************************************/
		turnButtons.push_back(new GameObject(glm::vec3(1.0f, 4.0f, 0.0f), textures["MENU"][0], size, "endTurn"));//0
		turnButtons.push_back(new GameObject(glm::vec3(-1.0f, 4.0f, 0.0f), textures["MENU"][0], size, "wave"));//0

																											   /************************************************player stats INIT************************************************/

		player1.push_back(new HUD(glm::vec3(-4.9f, 5.7f, 0.0f), cameraZoom, glm::vec3(0.1f, 0.1f, 0.0f), textures["Player"][0], size, factor, "hp", window));
		player1.push_back(new HUD(glm::vec3(-8.5f, 7.8f, 0.0f), cameraZoom, glm::vec3(0.07f, 0.07f, 0.0f), textures["Player"][1], size, factor, "gold", window));
		player1.push_back(new HUD(glm::vec3(-8.5f, 8.8f, 0.0f), cameraZoom, glm::vec3(0.07f, 0.07f, 0.0f), textures["Player"][2], size, factor, "income", window));

		player2.push_back(new HUD(glm::vec3(4.9f, 5.7f, 0.0f), cameraZoom, glm::vec3(0.1f, 0.1f, 0.0f), textures["Player"][0], size, factor, "hp", window));
		player2.push_back(new HUD(glm::vec3(8.5f, 7.8f, 0.0f), cameraZoom, glm::vec3(0.07f, 0.07f, 0.0f), textures["Player"][1], size, factor, "gold", window));
		player2.push_back(new HUD(glm::vec3(8.5f, 8.8f, 0.0f), cameraZoom, glm::vec3(0.07f, 0.07f, 0.0f), textures["Player"][2], size, factor, "income", window));

		button.push_back(new GameObject(glm::vec3(-7.5f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));

		/************************************************HUD INIT************************************************/
		GameObject* cursor = new GameObject(glm::vec3(0.0f), cursorTex, size, "cursor");
		HUD* selectionGraphic = new HUD(glm::vec3(50.0f, 50.0f, 0.0f), cameraZoom, glm::vec3(0.1f, 0.1f, 0.0f), selectGraphicTex, size, factor, "selection", window);
		//HUD* //selectionGraphic2 = new HUD(glm::vec3(50.0f, 50.0f, 0.0f), cameraZoom, glm::vec3(0.1f, 0.1f, 0.0f), selectGraphicTex, size, factor, "selection", window);
		//==================================================================================================


		hudObjects.push_back(new HUD(glm::vec3(-1.17f, 3.45f, 0.0f), cameraZoom, glm::vec3(0.6f, 0.17f, 0.0f), textures["UI"][0], size, factor, "", window));//toer side//0
		hudObjects[0]->setGeneralList(player1);
		hudObjects.push_back(new HUD(glm::vec3(1.17f, 3.45f, 0.0f), cameraZoom, glm::vec3(0.6f, 0.17f, 0.0f), textures["UI"][0], size, factor, "", window));//enemy side//1
		hudObjects[1]->setGeneralList(player2);
		hudObjects.push_back(new HUD(glm::vec3(0.0f, 3.45f, 0.0f), cameraZoom, glm::vec3(0.8f, 0.17f, 0.0f), textures["UI"][0], size, factor, "", window));// power ups middle//2
		hudObjects[2]->setPowerUPs(powerUpList);

		glm::vec3 objectS = glm::vec3(0.35f, 0.5f, 0.0f);//this handels the size(scale) of the HUD panel 
		hudObjects.push_back(new HUD(glm::vec3(-2.4f, 1.5f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD0", window));// this is the tower HUD//3
		hudObjects[3]->setBlueprints(blueprints);

		hudObjects.push_back(new HUD(glm::vec3(2.4f, 1.5f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD1", window));//this is the Enemy HUD
		hudObjects[4]->setEnemyBlueprints(enemyBlueprint);

		objectS.x = 1.1f;//this handels the size(scale) of the HUD panel
		hudObjects.push_back(new HUD(glm::vec3(0.0f, 1.5f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD2", window));//detail HUD
		hudObjects[5]->setBlueprints(towerDetailHUD);//--------------------------????????????????????????????????????????????????
		hudObjects[5]->setEnemyBlueprints(enemyDetailHUD);//--------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		hudObjects[5]->setPowerUPs(powerUpListHUD);
		hudObjects[5]->setUpgrades(upgradeListHUD);
		//===================================counters(top hud)
		objectS.x = 1.0f;
		objectS.y = 0.4f;
		hudObjects.push_back(new HUD(glm::vec3(0.7f, 0.9f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD3", window));//3
		hudObjects[6]->setEnemyBlueprints(enemyCounters);
		hudObjects.push_back(new HUD(glm::vec3(-0.7f, 0.9f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4", window));//4
		hudObjects[7]->setEnemyBlueprints(enemyCounters2);
		objectS.x = 2.0f;
		hudObjects.push_back(new HUD(glm::vec3(0.0f, 0.9f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4", window));
		hudObjects[8]->setGameObjects(turnButtons);
		hudObjects.push_back(new HUD(glm::vec3(-0.158f, 2.0f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4", window));
		hudObjects[9]->setUpgrades(upgradeList);
		

		for (HUD* h : hudObjects) {
			h->setCamPos(cameraTranslatePos);
		}
		for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
		for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
		selectionGraphic->setCamPos(cameraTranslatePos);
		////selectionGraphic2->setCamPos(cameraTranslatePos);

		/************************************************MENU INIT************************************************/
		glm::vec3 buttonScale = glm::vec3(0.5f, 0.5f, 0.0f);
		//HUDMenu.push_back(new HUD(glm::vec3(0.0f, -1.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][0], size, factor, "PLAY", window));
		//HUDMenu.push_back(new HUD(glm::vec3(0.0f, -0.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][2], size, factor, "SCORE", window));
		//HUDMenu.push_back(new HUD(glm::vec3(0.0f, 0.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][3], size, factor, "OPTION", window));
		//HUDMenu.push_back(new HUD(glm::vec3(0.0f, 1.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][1], size, factor, "QUIT", window));

		/************************************************TEXT INIT************************************************/

		//Parameters: Text(coordinates, fontTexture, text, size, scale, RGBcolor)
		hudObjects[2]->addText(new Text(glm::vec3(0.5f, 4.5f, 0.0f), fontTexture, "Player ", size, 0.2f, glm::vec3(0, 0, 153), "player"));

		hudObjects[0]->addText(new Text(glm::vec3(16.3f, -15.4f, 0.0f), fontTexture, std::to_string(credits[0]), size, 0.04f, glm::vec3(50, 175, 255), "P1credits"));
		hudObjects[0]->addText(new Text(glm::vec3(16.3f, -14.0f, 0.0f), fontTexture, std::to_string(income[0]), size, 0.04f, glm::vec3(50, 175, 255), "P1income"));
		hudObjects[0]->addText(new Text(glm::vec3(11.0f, -16.0f, 0.0f), fontTexture, std::to_string(hp[0]) + "/20", size, 0.04f, glm::vec3(50, 175, 255), "P1hp"));

		hudObjects[1]->addText(new Text(glm::vec3(-24.0f, -15.4f, 0.0f), fontTexture, std::to_string(credits[1]), size, 0.04f, glm::vec3(50, 175, 255), "P2credits"));
		hudObjects[1]->addText(new Text(glm::vec3(-24.0f, -14.0f, 0.0f), fontTexture, std::to_string(income[1]), size, 0.04f, glm::vec3(50, 175, 255), "P2income"));
		hudObjects[1]->addText(new Text(glm::vec3(-13.5f, -16.0f, 0.0f), fontTexture, std::to_string(hp[1]) + "/20", size, 0.04f, glm::vec3(50, 175, 255), "P2hp"));

		//============================================================================================================================
		hudObjects[6]->addText(new Text(glm::vec3(-17.5f, -5.0f, 0.0f), fontTexture, std::to_string(normalCount), size, 0.05f, glm::vec3(50, 175, 255), "P1normal"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-12.5f, -5.0f, 0.0f), fontTexture, std::to_string(fastCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fast"));
		hudObjects[6]->addText(new Text(glm::vec3(-7.5f, -5.0f, 0.0f), fontTexture, std::to_string(heartyCount), size, 0.05f, glm::vec3(50, 175, 255), "P1hearty"));//0	|
		
		hudObjects[6]->addText(new Text(glm::vec3(-17.5f, -7.0f, 0.0f), fontTexture, std::to_string(flyingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1flying"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-12.5f, -7.0f, 0.0f), fontTexture, std::to_string(splitterCount), size, 0.05f, glm::vec3(50, 175, 255), "P1splitter"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-7.5f, -7.0f, 0.0f), fontTexture, std::to_string(regeneratingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1regenerating"));//0	|
		
		hudObjects[6]->addText(new Text(glm::vec3(-17.5f, -9.0f, 0.0f), fontTexture, std::to_string(fast_heartyCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fasthearty"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-12.5f, -9.0f, 0.0f), fontTexture, std::to_string(fast_flyingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fastflying"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-7.5f, -9.0f, 0.0f), fontTexture, std::to_string(fast_regeneratingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fastregenerating"));//0	|
		
																																														 //=============================================================================================================================
		hudObjects[7]->addText(new Text(glm::vec3(6.5f, -5.0f, 0.0f), fontTexture, std::to_string(normalCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2normal"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(11.5f, -5.0f, 0.0f), fontTexture, std::to_string(fastCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fast"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(16.5f, -5.0f, 0.0f), fontTexture, std::to_string(heartyCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2hearty"));//0	|

		hudObjects[7]->addText(new Text(glm::vec3(6.5f, -7.0f, 0.0f), fontTexture, std::to_string(flyingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2flying"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(11.5f, -7.0f, 0.0f), fontTexture, std::to_string(splitterCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2splitter"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(16.5f, -7.0f, 0.0f), fontTexture, std::to_string(regeneratingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2regenerating"));//0	|

		hudObjects[7]->addText(new Text(glm::vec3(6.5f, -9.0f, 0.0f), fontTexture, std::to_string(fast_heartyCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fasthearty"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(11.5f, -9.0f, 0.0f), fontTexture, std::to_string(fast_flyingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fastflying"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(16.5f, -9.0f, 0.0f), fontTexture, std::to_string(fast_regeneratingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fastregenerating"));//0	|
		hudObjects[2]->addText(new Text(glm::vec3(-14.0f, 13.9f, 0.0f), fontTexture, "FPS: ", size, 0.07f, glm::vec3(0, 0, 0), "FPS"));

		//==========================================================//===============================================>>>new stufff
		hudObjects[5]->addText(new Text(glm::vec3(-10.0f, -18.0f, 0.0f), fontTexture, "NAME:", size, 0.04f, glm::vec3(50, 175, 255), "name"));//0
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -19.0f, 0.0f), fontTexture, "COST:", size, 0.04f, glm::vec3(50, 175, 255), "cost"));//1
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -20.0f, 0.0f), fontTexture, "HOTKEY:", size, 0.04f, glm::vec3(50, 175, 255), "hotkey"));//2
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -21.0f, 0.0f), fontTexture, "DAMAGE:", size, 0.04f, glm::vec3(50, 175, 255), "towerdamage"));//3
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -22.0f, 0.0f), fontTexture, "ROF:", size, 0.04f, glm::vec3(50, 175, 255), "towerrof"));//4
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -23.0f, 0.0f), fontTexture, "RANGE:", size, 0.04f, glm::vec3(50, 175, 255), "towerrange"));//5
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -21.0f, 0.0f), fontTexture, "HP:", size, 0.04f, glm::vec3(50, 175, 255), "enemyhp"));//6
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -22.0f, 0.0f), fontTexture, "SPEED:", size, 0.04f, glm::vec3(50, 175, 255), "enemyspeed"));//7
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -23.0f, 0.0f), fontTexture, "REGEN:", size, 0.04f, glm::vec3(50, 175, 255), "enemyregen"));//8

		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -19.0f, 0.0f), fontTexture, "RANGE:", size, 0.04f, glm::vec3(50, 175, 255), "powerupRange"));//9
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -20.0f, 0.0f), fontTexture, "DURATION:", size, 0.04f, glm::vec3(50, 175, 255), "powerupDuration"));//10
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -19.0f, 0.0f), fontTexture, "MOD:", size, 0.04f, glm::vec3(50, 175, 255), "upgradeMod"));//11
																										
		/************************************************GAME LOOP************************************************/
		int fps = 0;
		int renderedFPS = 0;
		int oldTime = 0;
		float lastSpawnTime = 0;
		glm::vec3 targetPos;
		bool roundOver = true;
		int enemiesDestroyed = 0;
		int numEnemiesSpawned = 0;
		
		_state = MainMenu;
		//_state = Game;//comment out to see menu. 

		glfwSetFramebufferSizeCallback(window, ResizeCallback);
		while (!glfwWindowShouldClose(window)) {

			//int Wwidth, Wheight;
			glfwGetWindowSize(window, &Wwidth, &Wheight);
			factor = (float)Wwidth / 800;

			++fps;
			if (int(glfwGetTime())>oldTime) {
				oldTime = int(glfwGetTime());
				std::cout << "FPS: "<<fps << std::endl;
				renderedFPS = fps;
				fps = 0;

			}
			/************************************************KEY INPUT********************************************/
			
			switch (_state) {
			case MainMenu: {
				for (HUD* b : HUDMenu)b->setCamPos(cameraTranslatePos);

				if (timeOfLastMove + 0.05 < glfwGetTime()) {
					if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS &&
						(timeOfLastMove + 0.15 < glfwGetTime())) {
						_state = Game;
						audioObject->stop("menu");
						audioObject->playRepeat("background");
					}
				}
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					double xpos, ypos;
					glfwGetCursorPos(window, &xpos, &ypos);
					float xOut = (float)xpos;
					float yOut = (float)ypos;

					float x;
					float y;
					int id = g.getHover();
					g.getHoverCoords(x, y);

					//std::cout << xOut << "," << yOut << std::endl;
				}
				break;
			}
			case Game: {
				if (timeOfLastMove + 0.05 < glfwGetTime()) {

					if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS &&
						(timeOfLastMove + 0.5 < glfwGetTime())) {
						_state = Pause;
						audioObject->stop("menu");
						audioObject->playRepeat("background");
						timeOfLastMove = glfwGetTime();
					}

					double xpos, ypos;
					int window_width_g, window_height_g;
					glfwGetWindowSize(window, &window_width_g, &window_height_g);
					glfwGetCursorPos(window, &xpos, &ypos);
					//std::cout << "x: " << xpos << " y: " << ypos << std::endl;
					//std::cout << "w: " << window_width_g << " h: " << window_height_g << std::endl;
					bool inWindow = ypos > 0 && xpos > 0 && ypos < window_height_g && xpos < window_width_g;
					if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
						((ypos > 0 && ypos<30) && inWindow)) {
						cameraTranslatePos.y -= camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);

					}
					if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
						((ypos > window_height_g - 30 && ypos < window_height_g) && inWindow)) {
						cameraTranslatePos.y += camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
					}
					if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
						((xpos > window_width_g - 30 && xpos < window_width_g) && inWindow)) {
						cameraTranslatePos.x -= camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
					}
					if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
						((xpos > 0 && xpos < 30) && inWindow)) {
						cameraTranslatePos.x += camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
					}
					if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
						cameraZoom = std::fmin(cameraZoom + camZoomInc, maxCamZoom);
						g.setZoom(cameraZoom);
						timeOfLastMove = glfwGetTime();
						for (HUD* h : hudObjects)h->setZoom(cameraZoom);
						for (HUD* h : player1)h->setZoom(cameraZoom);
						for (HUD* h : player2)h->setZoom(cameraZoom);
						selectionGraphic->setZoom(cameraZoom);
					}
					if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
						cameraZoom = std::fmax(cameraZoom - camZoomInc, minCamZoom);
						g.setZoom(cameraZoom);
						timeOfLastMove = glfwGetTime();
						for (HUD* h : hudObjects)h->setZoom(cameraZoom);
						for (HUD* h : player1)h->setZoom(cameraZoom);
						for (HUD* h : player2)h->setZoom(cameraZoom);
						selectionGraphic->setZoom(cameraZoom);
					}
					if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
						startWave = 1;
						audioObject->playAgain("enemiesComing");
						roundOver = false;
						enemiesDestroyed = 0;
						numEnemiesSpawned = enemyMap[turnIndex]->size();

					}if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						showRadius ^= 1;
						timeOfLastMove = glfwGetTime();
					}

					
					//powerups:
					//kill all in 1.5 range
					if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						
						hudObjects[2]->setSelectionPowerUps(powerUpList[0]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[0]);
						selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setFlag(false);
						hudObjects[5]->setEnemyFlag(false);
						hudObjects[5]->setPowerUpFlag(true);

						//std::cout << hudObjects[2]->getSelectionPowerUps()->getType() << std::endl;

						selectionRadius = 1.5;
						for (EnemyObject* e : enemiesInRange(g.getNode(g.getHover()), selectionRadius, enemyMap[turnIndex])) {
							if (e->getType().compare("Undying") != 0) {
								e->setExists(false);
							}
						}
						timeOfLastMove = glfwGetTime();
					}
					//Stop all in 1.5 range for 4 seconds
					if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						
						hudObjects[2]->setSelectionPowerUps(powerUpList[1]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[1]);
						selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setFlag(false);
						hudObjects[5]->setEnemyFlag(false);
						hudObjects[5]->setPowerUpFlag(true);


						//std::cout << hudObjects[2]->getSelectionPowerUps()->getType() << std::endl;

						selectionRadius = 1.5;
						for (EnemyObject* e : enemiesInRange(g.getNode(g.getHover()), selectionRadius, enemyMap[turnIndex])) {
							e->modCurSpeed(0);
							e->setEffectDuration(4);
						}
						timeOfLastMove = glfwGetTime();
					}
					//Inc outgoing creep hp 100% for 20 seconds.
					if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						
						hudObjects[2]->setSelectionPowerUps(powerUpList[2]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[2]);
						selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setFlag(false);
						hudObjects[5]->setEnemyFlag(false);
						hudObjects[5]->setPowerUpFlag(true);

						//std::cout << hudObjects[2]->getSelectionPowerUps()->getType() << std::endl;

						hpUp = true;
						for (EnemyObject* e : *(enemyMap[turnIndex ^ 1])) {
							e->modCurHealthCap(2);
							e->setEffectDuration(20);
						}
						timeOfLastMove = glfwGetTime();
					}
					//Inc outgoing creep speed 50% for 20 seconds. up to 2.5 speed
					if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						hudObjects[2]->setSelectionPowerUps(powerUpList[3]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[3]);
						selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setFlag(false);
						hudObjects[5]->setEnemyFlag(false);
						hudObjects[5]->setPowerUpFlag(true);

						//std::cout << hudObjects[2]->getSelectionPowerUps()->getType() << std::endl;

						gottaGoFast = true;
						for (EnemyObject* e : *(enemyMap[turnIndex ^ 1])) {
							e->modCurSpeed(1.5);
							e->setEffectDuration(20);
						}
						timeOfLastMove = glfwGetTime();
					}
					//inc allied tower rof by 100% for 15 seconds
					if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
			
						hudObjects[2]->setSelectionPowerUps(powerUpList[4]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[4]);
						selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setFlag(false);
						hudObjects[5]->setEnemyFlag(false);
						hudObjects[5]->setPowerUpFlag(true);

						//std::cout << hudObjects[2]->getSelectionPowerUps()->getType() << std::endl;

						rainingLead = true;
						rainingLeadEnd = glfwGetTime() + 15;
						for (TowerObject* t : *(towerMap[turnIndex])) {
							t->modCurROF(0.5);
							t->setEffectDuration(15);
						}
						timeOfLastMove = glfwGetTime();
					}
					
					//centre screen
					if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						cameraTranslatePos = glm::vec3(0.0f);
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);

					}

					if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&
						enemyMap[turnIndex]->empty() && (timeOfLastMove + 0.15 < glfwGetTime())) {
						spawnCount = 0;
						startWave = 0;
						gottaGoFast = false;
						hpUp = false;
						rainingLead = false;
						turnIndex = turnIndex ^ 1;
						cameraTranslatePos = g.getFocalPoint(turnIndex);
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects) {
							h->setCamPos(cameraTranslatePos);
						}
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
						timeOfLastMove = glfwGetTime();
						std::cout << "T \n";
						turn = turnArr[turnIndex];
						credits[turnIndex] += income[turnIndex];
						g.clearNextNodeMaps();
						g.startPaths(turnIndex);

						audioObject->playAgain("teamChange");
					}
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
						std::cout << g.getHover() << std::endl;
						//Node* tttt = &g.getNode(g.getHover());
						g.selectRange(g.getHover(), 1.5);
						//tttt->selected = true;
						std::cout << g.getHover() << std::endl;
						Node* tttt = g.getNode(g.getHover()).getNextNode(2024);
						if (tttt == NULL) {
							tttt = g.getNode(g.getHover()).getNextNode(314);
						}
						if (tttt != NULL) {
							std::cout << tttt->getId() << std::endl;
						}
						else {
							std::cout << "null\n";
						}

					}
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.2 < glfwGetTime())) {

						double xpos, ypos;
						glfwGetCursorPos(window, &xpos, &ypos);

						float x;
						float y;
						int id = g.getHover();
						g.getHoverCoords(x, y);

						//std::cout << ypos << ", " << 400*factor << std::endl;

						if (ypos <= 400*factor) {//prints on map
							if (hudObjects[3]->getFlag()) {

								if (g.getNode(id).getBuildable(turn)) {
									TowerObject* selectedTower = hudObjects[3]->getSelection();
									std::cout << "Tower costs: " << selectedTower->getCost() << std::endl;
									if (credits[turnIndex] >= selectedTower->getCost()) {
										
								
					
										std::cout << "tower =  " << std::endl;
										TowerObject* t = new TowerObject(glm::vec3(x, y, 0.0f), selectedTower->getTexvec(),
											selectedTower->getExplosion_tex(), size, selectedTower->getDamage(),
											selectedTower->getType(), selectedTower->getRange(), selectedTower->getROF(),
											selectedTower->getCost(), selectedTower->getSpeed());
										t->setAudio(audioObject);
										
										if (t->getType().compare("denderBlueprint---2") == 0) {

										}
										g.getNode(id).setTowerState(true, turn);
										std::cout << "tower made  " << selectedTower->getCost() << std::endl;
										//
										//g.clearNextNodeMaps();
										//g.startPaths();
										if (g.startPaths(turnIndex) && g.rePath(id, turn)) {
											g.getNode(id).setTower(t);
											credits[turnIndex] -= selectedTower->getCost();
											towerMap[turnIndex]->push_back(t);
											std::cout << "GRAPH STUFF " << selectedTower->getCost() << std::endl;
											if (rainingLead) {
												t->modCurROF(0.5);
												t->setEffectDuration(rainingLeadEnd - glfwGetTime());
												if (rainingLeadEnd - glfwGetTime() <= 0) {
													rainingLead = false;
												}
											}
											audioObject->playAgain("towerPlaced");
											audioObject->playAgain("unitReady");

										}
										else {
											std::cout << "\n\n\nINVALID TOWER PLACEMENT";
											g.getNode(id).setTowerState(false, turn);
											g.clearNextNodeMaps();
											g.startPaths(turnIndex);
											g.rePath(id, turn);
											delete(t);
										}
										std::cout << "Repath" << std::endl;

											
										
									}


								}
							}
						}

						
						
						hudObjects[3]->selection(xpos, ypos);
						
						hudObjects[2]->selectionPowerUp(xpos, ypos);
						if (hudObjects[2]->getSelectionPowerUps()!=NULL) {
							selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
							//std::cout << hudObjects[2]->getSelectionPowerUps()->getType() << std::endl;
							hudObjects[3]->setFlag(false);
							hudObjects[2]->setSelectionPowerUps(NULL);
						}


						hudObjects[9]->selectionUpgrades(xpos, ypos);
						if (hudObjects[9]->getSelectionUpgrades() !=NULL) {
							selectionGraphic->setPosition(hudObjects[9]->getSelectionUpgrades()->getPosition());
							//std::cout << hudObjects[9]->getSelectionUpgrades()->getType() << std::endl;
							hudObjects[9]->setSelectionUpgrades(NULL);
						}
						//============================================================this is for the details render out put
						hudObjects[5]->selection(xpos, ypos);
						hudObjects[5]->selectionEnemy(xpos, ypos);
						hudObjects[5]->selectionPowerUp(xpos, ypos);
						hudObjects[5]->selectionUpgrades(xpos, ypos);
						//============================================================this is for the details render out put

						hudObjects[4]->selectionEnemy(xpos, ypos);
					
						if (hudObjects[4]->getSelectionEnemy() !=NULL) {
							
							
							selectedEnemy = hudObjects[4]->getSelectionEnemy();
							selectionGraphic->setPosition(hudObjects[4]->getSelectionEnemy()->getPosition());

							hudObjects[4]->setSelectionEnemy(NULL);
						}
						/*
						if (hudObjects[4]->getSelectionEnemy() != NULL)
						{
							if (credits[turnIndex] >= 5) { //>=selectedEnemy->getCost()
								std::cout << "Spawned new enemy. Total: " << enemyMap[turnIndex ^ 1]->size() + 1 << std::endl;
								income[turnIndex] += 5;
								credits[turnIndex] -= 5;
								Node* cur;
								for (int s : g.getStartSet(turnArr[turnIndex ^ 1])) {
									cur = &g.getNode(s);
									if (hudObjects[4]->getSelectionEnemy()->getType() == "normal") { normalCount += 1; }
									EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), hudObjects[4]->getSelectionEnemy()->getTex(), size, hudObjects[4]->getSelectionEnemy()->getHealth(), hudObjects[4]->getSelectionEnemy()->getType(), hudObjects[4]->getSelectionEnemy()->getEnemyDeathTex(), hudObjects[4]->getSelectionEnemy()->getDefaultSpeed(), hudObjects[4]->getSelectionEnemy()->getCost());
									e->setAudio(audioObject);
									enemyMap[turnIndex ^ 1]->push_back(e);

									e->oldx = round(e->getPosition().x * 100) / 100;
									e->oldy = round(e->getPosition().y * 100) / 100;
									e->setCur(cur);
									e->setCurDestId(cur->getNextId());
								}
								timeOfLastMove = glfwGetTime();
							}
						}
						hudObjects[4]->setSelectionEnemy(NULL);
						*/
						hudObjects[6]->turns(xpos, ypos);

						if (hudObjects[7]->turns(xpos, ypos) == "wave") {
							startWave = 1;
							audioObject->playAgain("enemiesComing");
							roundOver = false;
							enemiesDestroyed = 0;
							numEnemiesSpawned = enemyMap[turnIndex]->size();
						}

						if (hudObjects[3]->getFlag() && cursor->getTex() != hudObjects[3]->getCursor()) {
							
							cursor->setTex(hudObjects[3]->getCursor());//sets the texture for the cursor with the tower icon <-----------
							selectionGraphic->setPosition(hudObjects[3]->getSelection()->getPosition());
							audioObject->playAgain("menuClick");
						}
						timeOfLastMove = glfwGetTime();
						


						//--------end of click button------------ 
					}

					if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
						cursor->setTex(textures["Cursor"][0]);
						selectedEnemy = NULL;
						hudObjects[1]->setFlag(false);
					}
					/*
					if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
						selectedEnemy = enemyBlueprint.at(0);
					}if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
						selectedEnemy = enemyBlueprint.at(1);
					}if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
						selectedEnemy = enemyBlueprint.at(2);
					}if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
						selectedEnemy = enemyBlueprint.at(3);
					}*/
					if (selectedEnemy != NULL) {
						//std::cout << selectedEnemy->getType() << std::endl;
					}
					//hudObjects[3]->selection(xpos, ypos);
					hudObjects[4]->selectionEnemy(xpos, ypos);
					selectedEnemy = hudObjects[4]->getSelectionEnemy();
					if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS &&
						selectedEnemy != NULL && (timeOfLastMove + 0.15 < glfwGetTime())) {
						//std::cout << selectedEnemy->getType() << std::endl;
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////
						//code for getting from hud here
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////

						if (credits[turnIndex] >= selectedEnemy->getCost() && enemyMap[turnIndex ^ 1]->size() < 200) { //>=selectedEnemy->getCost()
							std::cout << "Spawned new enemy. Total: " << enemyMap[turnIndex ^ 1]->size() + 1 << std::endl;
							income[turnIndex] += selectedEnemy->getCost();
							credits[turnIndex] -= selectedEnemy->getCost();
							Node* cur;
							for (int s : g.getStartSet(turnArr[turnIndex ^ 1])) {
								cur = &g.getNode(s);
								EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), selectedEnemy->getTex(), size, selectedEnemy->getHealth(),
									selectedEnemy->getType(), selectedEnemy->getEnemyDeathTex(), selectedEnemy->getCurSpeed(), selectedEnemy->getCost(), selectedEnemy->getRegen());
								e->setAudio(audioObject);
								enemyMap[turnIndex ^ 1]->push_back(e);
								if (gottaGoFast) {
									e->modCurSpeed(1.5);
									e->setEffectDuration(20);
								}
								if (hpUp) {
									e->modCurHealthCap(2);
									e->setEffectDuration(20);
								}
								e->oldx = round(e->getPosition().x * 100) / 100;
								e->oldy = round(e->getPosition().y * 100) / 100;
								e->setCur(cur);
								e->setCurDestId(cur->getNextId());
							}
							timeOfLastMove = glfwGetTime();

							audioObject->playAgain("enemySelected");
						}
					}

				}
				break;
			}
			case Pause: {
				break;
			}
			case GameOver: {
				break;
			}
			default:
				break;



			}
			
			//==========================================>state:play/controls

			//clear window
			glClearColor(viewport_background_color_g[0],
				viewport_background_color_g[1],
				viewport_background_color_g[2], 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// Select proper shader program to use
			shaders[0]->enable();

			/************************************************SHADER CAMERA CENTERING********************************************/
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			GameObject::cameraTranslatePos = cameraTranslatePos;
			GameObject::cameraZoom = cameraZoom;
			//Here it is just looping over each shader and setting the viewmatrix. 
			for (Shader* s : shaders) {
				s->enable();
				s->setUniformMat4("viewMatrix", viewMatrix);
				s->disable();
			}
			shaders[0]->enable();
			
		
			/************************************************OBJECT UPDATE/RENDERING********************************************/
			
			switch(_state) {
			case MainMenu: {
				for (HUD* b : HUDMenu) {
					b->render(shaders);
				}
				background->render(shaders);
				break;
			}
			case Game: {
				//shader =  Shader("shader.vert", "shader.frag");

				//**********HUD**********
				selectionGraphic->render(shaders);
				//==================================================
				hudObjects[0]->updatePlayers(credits[0], income[0], hp[0], credits[1], income[1], hp[1]);
				hudObjects[1]->updatePlayers(credits[0], income[0], hp[0], credits[1], income[1], hp[1]);
				hudObjects[5]->counter(*enemyMap[0], *enemyMap[1]);
				hudObjects[6]->counter(*enemyMap[0], *enemyMap[1]);
				hudObjects[7]->counter(*enemyMap[0], *enemyMap[1]);

				selectedEnemy = hudObjects[4]->getSelectionEnemy();

				if (hudObjects[4]->updateHotkeysEnemy()) {
					hudObjects[5]->updateHotkeysEnemy();
					selectionGraphic->setPosition(hudObjects[4]->getSelectionEnemy()->getPosition());
				}

				if (hudObjects[9]->updateHotkeysUpgrade()) {
					hudObjects[5]->updateHotkeysUpgrade();
					selectionGraphic->setPosition(hudObjects[9]->getSelectionUpgrades()->getPosition());
				}
				
				

				if (hudObjects[3]->updateHotkeysTower()) {
					hudObjects[5]->updateHotkeysTower();
					
					cursor->setTex(hudObjects[3]->getCursor());//sets the texture for the cursor with the tower icon <-----------
					selectionGraphic->setPosition(hudObjects[3]->getSelection()->getPosition());
					audioObject->playAgain("menuClick");
				}
				//=====================================================>>>>>>>>>>>>>>>>>DONT ERASE!!!!!!, THIS IS THE CODE TO BUY CREEPS WITH HOTKEYS

				//=====================================================
				/*
				if (hudObjects[4]->getSelectionEnemy() != NULL)
				{
					if (credits[turnIndex] >= 5) { //>=selectedEnemy->getCost()
						std::cout << "Spawned new enemy. Total: " << enemyMap[turnIndex ^ 1]->size() + 1 << std::endl;
						income[turnIndex] += 5;
						credits[turnIndex] -= 5;
						Node* cur;
						for (int s : g.getStartSet(turnArr[turnIndex ^ 1])) {
							cur = &g.getNode(s);

							EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), hudObjects[4]->getSelectionEnemy()->getTex(), size, hudObjects[4]->getSelectionEnemy()->getHealth(), hudObjects[4]->getSelectionEnemy()->getType(), hudObjects[4]->getSelectionEnemy()->getEnemyDeathTex(), hudObjects[4]->getSelectionEnemy()->getDefaultSpeed(), hudObjects[4]->getSelectionEnemy()->getCost());
							e->setAudio(audioObject);
							enemyMap[turnIndex ^ 1]->push_back(e);

							e->oldx = round(e->getPosition().x * 100) / 100;
							e->oldy = round(e->getPosition().y * 100) / 100;
							e->setCur(cur);
							e->setCurDestId(cur->getNextId());
						}
						timeOfLastMove = glfwGetTime();
					}
				}
				hudObjects[4]->setSelectionEnemy(NULL);*/
				//=====================================================
				
				hudObjects[0]->setTex(textures["UI"][turnIndex]);
				hudObjects[0]->setFactor(factor);
				hudObjects[0]->update(deltaTime);
				hudObjects[0]->render(shaders);

				hudObjects[1]->setTex(textures["UI"][turnIndex]);
				hudObjects[1]->setFactor(factor);
				hudObjects[1]->update(deltaTime);
				hudObjects[1]->render(shaders);

				hudObjects[2]->setTex(textures["UI"][turnIndex]);
				hudObjects[2]->setFactor(factor);
				hudObjects[2]->update(deltaTime);
				hudObjects[2]->render(shaders);
				hudObjects[3]->setTex(textures["UI"][turnIndex]);
				hudObjects[3]->setFactor(factor);
				hudObjects[3]->update(deltaTime);
				hudObjects[3]->render(shaders);
				hudObjects[4]->setTex(textures["UI"][turnIndex]);
				hudObjects[4]->setFactor(factor);
				hudObjects[4]->update(deltaTime);
				hudObjects[4]->render(shaders);
				hudObjects[5]->setTex(textures["UI"][turnIndex]);
				hudObjects[5]->setFactor(factor);
				hudObjects[5]->update(deltaTime);
				hudObjects[5]->detailRender(shaders);
				

				for (HUD* h : hudObjects) {
					h->setTex(textures["UI"][turnIndex]);
					h->setFactor(factor);
					h->update(deltaTime);

					for (Text* t : h->getTextObjects()) {
						t->setColor(hudColors[turnIndex]);

						if (t->getType().compare("FPS: ") == 0) {
							std::string temp = t->getText() + std::to_string(renderedFPS);
							t->setRenderedText(temp);
						}

						if (t->getType().compare("Enemies Remaining: ") == 0) {
							std::string temp = t->getText() + std::to_string(enemyMap[turnIndex]->size());
							t->setRenderedText(temp);
						}
						if (t->getType().compare("Player ") == 0) {
							std::string temp = t->getText() + std::to_string(turnIndex + 1);
							t->setRenderedText(temp);
						}
						if (t->getType().compare("Income: ") == 0) {
							std::string temp = t->getText() + std::to_string(income[turnIndex]);
							t->setRenderedText(temp);
						}
						if (t->getType().compare("Credits: ") == 0) {
							std::string temp = t->getText() + std::to_string(credits[turnIndex]);
							t->setRenderedText(temp);
						}
						if (t->getType().compare("P1 HP: ") == 0) {
							std::string temp = t->getText() + std::to_string(hp[0]) + "/20";
							t->setRenderedText(temp);
						}
						if (t->getType().compare("P2 HP: ") == 0) {
							std::string temp = t->getText() + std::to_string(hp[1]) + "/20";
							t->setRenderedText(temp);
						}
					}

					h->render(shaders);
				}

				//**********Cursor**********
				float x, y;
				int id = g.getHover();
				g.getHoverCoords(x, y);
				cursor->setPosition(glm::vec3(x, y, 0.0f));
				if (g.getNode(id).getBuildable(turn)) {

					cursor->render(shaders);
				}
				else {
					//glBlendColor(1.0f, 1.0f, 1.0f, 0.05f);
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_SRC_ALPHA);
					//glUniform4f(glGetUniformLocation(shader.getShaderID(), "colorMod"), 0.0f, 0.0f, 0.0f,0.5f);	//dark green
					glUniform3f(glGetUniformLocation(shaders[0]->getShaderID(), "colorMod"), 0.5f, -1.0f, -1.0f);
					cursor->render(shaders);
					glEnable(GL_LIGHTING);

					glBlendFunc(GL_SRC_ALPHA, GL_ZERO),
						glUniform3f(glGetUniformLocation(shaders[0]->getShaderID(), "colorMod"), 0.0f, 0.0f, 0.0f);	//dark green
				}

				//**********Tower**********
				shaders[1]->setRadius(1);
				for (TowerObject* t : *(towerMap[turnIndex])) {
					t->setEnemies(*enemyMap[turnIndex]);


					EnemyObject* closestEnemy;

					if (enemyMap[turnIndex]->size() == 0 || !startWave) { closestEnemy = NULL; }
					else {

						//get the closest enemy to the tower and point to it using closestEnemy
						closestEnemy = enemyMap[turnIndex]->front();
						float enemyDistance = glm::length(t->getPosition() - closestEnemy->getPosition());

						for (EnemyObject* e : *enemyMap[turnIndex]) {

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
					GLint color_loc = glGetUniformLocation(shaders[0]->getShaderID(), "colorMod");
					glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
					// Render game objects
					t->render(shaders);
				}
				for (TowerObject* t : *(towerMap[turnIndex ^ 1])) {
					// Updates game objects
					t->update(deltaTime);
					//reset color uniform.
					GLint color_loc = glGetUniformLocation(shaders[0]->getShaderID(), "colorMod");
					glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
					// Render game objects
					t->render(shaders);
				}
				shaders[2]->setRadius(2);

				//**********Enemy**********
				std::deque<int> deleteEnemies;
				int delIndex = 0;
				bool tick = false;
				//spawnCount+=0.25;
				if (enemyMap[turnIndex]->size() == 0 && !roundOver && enemiesDestroyed == numEnemiesSpawned) {
					roundOver = true;
					std::cout << enemiesDestroyed << "-" << numEnemiesSpawned << std::endl;
					audioObject->playAgain("enemiesDestroyed");
				}
				if (glfwGetTime() > lastSpawnTime + 0.2) {
					//std::cout << "\n\n\n\nHERE"<<spawnCount<<"\n\n\n";
					spawnCount += 1;
					lastSpawnTime = glfwGetTime();
					tick = true;
				}

				int count = 0;
				//for (EnemyObject* e : *enemyMap[turnIndex]) {
				for (std::vector<EnemyObject*>::iterator it = enemyMap[turnIndex]->begin(); it != enemyMap[turnIndex]->end(); ++it) {
					if (!startWave) {
						if (tick) {
							//spawnCount = std::fmax(0, spawnCount - 1);
							spawnCount -= 1;
						}
						break;
					}
					if (count > spawnCount) { break; }
					++count;
					EnemyObject* e = *it;
					e->setSpawned(true);
					oldEnemyX = e->oldx;
					oldEnemyY = e->oldy;

					enemyX = round(e->getPosition().x * 100) / 100;
					enemyY = round(e->getPosition().y * 100) / 100;

					e->oldx = enemyX;
					e->oldy = enemyY;

					cur = e->getCur();

					if (!e->getExists()) {
						if (e->getHealth() <= 0.0) {
							enemiesDestroyed++;
						}
						std::cout << "Enemy despawned/destroyed. Total: " << enemyMap[turnIndex]->size() - 1 << std::endl;
						deleteEnemies.push_front(delIndex);
					}

					delIndex++;



					if (cur->getId() != g.getEndPoints(turnIndex) && cur->getNextNode(e->getCurDestId()) != NULL) {

						Node* next = cur->getNextNode(e->getCurDestId());
						if (!next->getPathable()) {
							g.setStart(cur->getId());
							g.setEnd(e->getCurDestId());
							if (!g.pathfind()) {
								std::cout << "Welll fuck";
							}
							next = cur->getNextNode(e->getCurDestId());
						}

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
							//cur->setIsCur(false);
							if (abs(oldEnemyX - next->getX()) < 0.2 &&
								abs(oldEnemyY - next->getY()) < 0.2) {
								cur = next;
							}
							//cur->setIsCur(true);
							e->setCur(cur);
							e->setCurDestId(cur->getNextId());
							if (cur->getNextNode(e->getCurDestId()) != NULL) {

								next = cur->getNextNode(e->getCurDestId());
								if (!next->getPathable()) {

									g.setStart(cur->getId());
									g.setEnd(e->getCurDestId());
									if (!g.pathfind()) {
										std::cout << "Welll fuck";
									}
									next = cur->getNextNode(e->getCurDestId());
								}
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
								std::cout << "here";
								e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
							}

						}
						if ((oldEnemyX <= nextx && nextx <= enemyX) || (oldEnemyX >= nextx && nextx >= enemyX)) {

							glm::vec3 pos = e->getPosition();
							pos.x = nextx;
							e->setPosition(pos);
							//cur->setIsCur(false);
							if (abs(oldEnemyX - next->getX()) < 0.2 &&
								abs(oldEnemyY - next->getY()) < 0.2) {
								cur = next;
							}
							//cur->setIsCur(true);
							e->setCur(cur);
							e->setCurDestId(cur->getNextId());
							if (cur->getNextNode(e->getCurDestId()) != NULL) {

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
					if (e->getExists() && (cur->getId() == g.getEndPoints(turnIndex) || (cur->getNextNode(e->getCurDestId()) == NULL))) {
						hp[turnIndex] -= 1;
						if (e->getType().compare("Undying Zenith") == 0) {
							hp[turnIndex] = 0;
						}
						std::cout << "hp = " << hp[turnIndex] << std::endl;
						audioObject->playAgain("baseAttack");
						e->setExists(false);
						if (hp[turnIndex] <= 0) {
							gameOver = true;
							_state = GameOver;
							std::cout << "GAME OVER" << std::endl;
							std::cout << "PLAYER " << (turnIndex ^ 1 + 1) << " WINS" << std::endl;
							//hudObjects[2]->setTex(textures["UI"][1]);

						}
					

					}

					e->update(deltaTime);

					//reset color uniform.
					GLint color_loc = glGetUniformLocation(shaders[0]->getShaderID(), "colorMod");
					glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);

					// Render game objects
					e->render(shaders);

				}

				//using the indices, delete the enemies that should be deleted. 
				for (int i = 0; i < deleteEnemies.size(); i++) {
					EnemyObject* e = enemyMap[turnIndex]->at(deleteEnemies[i]);
					if (e->getKilled()) {
						credits[turnIndex] += e->getCost();
					}
					enemyMap[turnIndex]->erase(enemyMap[turnIndex]->begin() + deleteEnemies[i]);
					delete(e);
				}

				//**********Graph**********
				g.update(showRadius, selectionRadius);
				//render graph
				g.render(shaders);

				break;
			}
			case Pause: {
				if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS &&
					(timeOfLastMove + 0.25 < glfwGetTime())) {
					_state = Game;
					audioObject->stop("menu");
					audioObject->playRepeat("background");
					timeOfLastMove = glfwGetTime();
				}
				break;
			}
			case GameOver: {
				break;
			}
			default:
				break;

			}
		
			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window);
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
	
}

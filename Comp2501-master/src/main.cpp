/* David Neudorf, Sharjeel Ali,Raul Rodriguez Azurdia
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
int GameObject::nextUniqueID = 0;

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Assault on Terra";
float ratio = 0.75f;
float window_width_g = 1000;//*1.5
float window_height_g = window_width_g * ratio;//*1.5
int Wwidth, Wheight;
float factor = window_width_g / window_width_g;//*1.5
//const glm::vec3 viewport_background_color_g(1, 1, 1);
const glm::vec3 viewport_background_color_g(15.0f / 255.0f, 5.0f / 255.0f, 24.0f / 255.0f);

// Global texture info

std::map<std::string, std::vector<GLuint>  > textures;
std::map<char, GLuint > fontTexture;

// Global game object info

std::map<int, std::vector<EnemyObject*>*> enemyMap;
std::map<int, std::vector<TowerObject*>*> towerMap;
std::map<std::string, GLuint> texMap = std::map<std::string, GLuint>();


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

std::vector<HUD*>mapHUD;

HUD* gameOverHUD;
HUD* pauseHUD;
Graph* g;
GLFWwindow* window;
Audio* audio;
FileUtils fileLoader;

int normalCount = 10000, fastCount = 10000, heartyCount = 10000, flyingCount = 10000, splitterCount = 10000, regeneratingCount = 10000, fast_heartyCount = 10000, fast_flyingCount = 10000, fast_regeneratingCount = 10000, fast_splitterCount = 10000, fast_hearty_regenerating_flyingCount = 10000;
int normalCount2 = 10000, fastCount2 = 10000, heartyCount2 = 10000, flyingCount2 = 10000, splitterCount2 = 10000, regeneratingCount2 = 10000, fast_heartyCount2 = 10000, fast_flyingCount2 = 10000, fast_regeneratingCount2 = 10000, fast_splitterCount2 = 10000, fast_hearty_regenerating_flyingCount2 = 10000;
enum State {
	MainMenu,
	MapMenu,
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

	textures["Map"].push_back(createTexture("../assets/Graphics/Map/0_0_boundary.png"));//0
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/0_1_boundary.png"));//1
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/1_empty.png"));//2
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/2_checkpoint.png"));//3
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/3_health.png"));//4
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/6_spawn.png"));//5
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/7_destructible.png"));//6

	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_01.png"));//7
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_02.png"));//8
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_03.png"));//9
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_04.png"));//10
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_05.png"));//11
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_06.png"));//12
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_07.png"));//13
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_1/0_Boundary_08.png"));//14

	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_2/1_Boundary_01.png"));//13
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_2/1_Boundary_02.png"));//14

	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_3/2_Boundary_03.png"));//9
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_3/2_Boundary_04.png"));//10
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_3/2_Boundary_05.png"));//11
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_3/2_Boundary_06.png"));//12
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_3/2_Boundary_07.png"));//13
	textures["Map"].push_back(createTexture("../assets/Graphics/Map/building_3/2_Boundary_08.png"));//14

	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/normal.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/glutton.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/speedster.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/regenerator.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/speedsterGlutton.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/regeneratingGlutton.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/regeneratingSpeedster.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/regeneratingSpeedsterGlutton.png"));
	textures["Enemy"].push_back(createTexture("../assets/Graphics/Enemy/undying.png"));

	textures["Particle"].push_back(createTexture("../assets/Graphics/Particles/fire.png"));
	textures["Particle"].push_back(createTexture("../assets/Graphics/Particles/enemyDeath.png"));

	//basic
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/01_tower.png"));//0
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/01_turret.png"));//1
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/01_projectile.png"));//2
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/01_towerIcon.png"));//3

	//defender
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/02_tower.png"));//4
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/02_turret.png"));//5
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/02_projectile.png"));//6
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/02_towerIcon.png"));//7

	//barrier
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/05_barrier.png"));//11
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/05_towerIcon.png"));//8
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/05_towerIcon.png"));//9
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/05_towerIcon.png"));//11
	//flamethrower
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/03_tower.png"));//8
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/03_turret.png"));//9
	textures["Tower"].push_back(textures["Particle"][0]);//10
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/03_towerIcon.png"));//11

	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/8_tower.png"));//4
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/8_tower.png"));//5
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/8_radius.png"));//6
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/8_towericon.png"));//7

	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/06_tower.png"));//4
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/06_turret.png"));//5
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/06_projectile.png"));//6
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/06_towerIcon.png"));//7

	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/04_auto.png"));//4
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/04_radio.png"));//5
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/06_projectile.png"));//6
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/04_auto.png"));//7

	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/07_tower.png"));//4
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/07_turret.png"));//5
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/07_projectile.png"));//6
	textures["Tower"].push_back(createTexture("../assets/Graphics/Tower/07_towerIcon.png"));//7


	textures["Explosion"].push_back(createTexture("../assets/Graphics/Explosion/tower_explode_1.png"));
	textures["Explosion"].push_back(createTexture("../assets/Graphics/Explosion/tower_explode_2.png"));
	textures["Explosion"].push_back(createTexture("../assets/Graphics/Explosion/tower_explode_3.png"));
	textures["Explosion"].push_back(createTexture("../assets/Graphics/Explosion/tower_explode_4.png"));
	textures["Explosion"].push_back(createTexture("../assets/Graphics/Explosion/tower_explode_5.png"));
	textures["Explosion"].push_back(createTexture("../assets/Graphics/Explosion/tower_explode_6.png"));

	textures["UI"].push_back(createTexture("../assets/Graphics/HUD/panel_player1.png"));
	textures["UI"].push_back(createTexture("../assets/Graphics/HUD/panel_player2.png"));


	textures["Cursor"].push_back(createTexture("../assets/Graphics/Cursor/cursor.png"));
	textures["Cursor"].push_back(createTexture("../assets/Graphics/Cursor/select.png"));

	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/placeholder.png"));//0
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/nullzone.png"));//1
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/timestop.png"));//2
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/morphine.png"));//3
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/stimpack.png"));//4
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/giveemlead.png"));//5

	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/damageup.png"));//5
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/rofup.png"));//5
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/rangeup.png"));//5

	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/healthup.png"));//5
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/speedup.png"));//5
	textures["MENU"].push_back(createTexture("../assets/Graphics/Buttons/costup.png"));//5

	textures["MAPBOX"].push_back(createTexture("../assets/Graphics/MENU/MapSelection/map1box.png"));
	textures["MAPBOX"].push_back(createTexture("../assets/Graphics/MENU/MapSelection/map2box.png"));
	textures["MAPBOX"].push_back(createTexture("../assets/Graphics/MENU/MapSelection/map3box.png"));
	textures["MAPBOX"].push_back(createTexture("../assets/Graphics/MENU/MapSelection/map4box.png"));
	textures["MAPBOX"].push_back(createTexture("../assets/Graphics/MENU/MapSelection/map5box.png"));

	textures["Background"].push_back(createTexture("../assets/Graphics/Background/menu.png"));//0
	textures["Background"].push_back(createTexture("../assets/Graphics/Background/mapMenu.png"));//0
	textures["Background"].push_back(createTexture("../assets/Graphics/Background/pauseMenu.png"));



	textures["Player"].push_back(createTexture("../assets/Graphics/playerStats/hp.png"));
	textures["Player"].push_back(createTexture("../assets/Graphics/playerStats/gold2.png"));
	textures["Player"].push_back(createTexture("../assets/Graphics/playerStats/income2.png"));
	textures["MENUHUD"].push_back(createTexture("../assets/Graphics/MENU/Play.png"));//0
	textures["MENUHUD"].push_back(createTexture("../assets/Graphics/MENU/Quit.png"));//1

	textures["Button"].push_back(createTexture("../assets/Graphics/Buttons/placeholder.png"));//0
	textures["Button"].push_back(createTexture("../assets/Graphics/Buttons/turn.png"));//0
	textures["Button"].push_back(createTexture("../assets/Graphics/Buttons/wave.png"));//0

	//Text is rendered by creating a map where each key is a character that corrasponds to an actual character.png file.
	std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz=0123456789.,;:$#'!\"/?%&()@-";

	for (int i = 0; i < characters.size(); i++) {

		std::string s = "../assets/Graphics/Text/text_" + std::to_string(i + 1) + ".png";

		char *cstr = &s[0u];
		std::cout << (i + 1) << " " << characters[i] << " " << s << std::endl;
		fontTexture[characters[i]] = createTexture(cstr);
	}

}

void setAudioTracks() {

	audio->addAudio("../assets/Audio/Soundtrack/infected.mp3", "background");
	audio->volume("background", 20);
	//audio->playRepeat("background");
	audio->addAudio("../assets/Audio/Soundtrack/mainMenu.mp3", "menu");
	audio->volume("menu", 100);
	audio->playRepeat("menu");

	audio->addAudio("../assets/Audio/Towers/rocket.mp3", "bullet");
	audio->volume("bullet", 100);
	audio->addAudio("../assets/Audio/Towers/place.mp3", "towerPlaced");
	audio->volume("towerPlaced", 30);
	audio->addAudio("../assets/Audio/Towers/ship.mp3", "ship");
	audio->volume("ship", 30);
	audio->addAudio("../assets/Audio/Towers/laser.mp3", "laser");
	audio->addAudio("../assets/Audio/Towers/cooldown.mp3", "cooldown");
	audio->volume("cooldown", 30);

	audio->addAudio("../assets/Audio/HUD/menuClick.mp3", "menuClick");
	audio->volume("menuClick", 100);
	audio->addAudio("../assets/Audio/HUD/teamChange.mp3", "teamChange");
	audio->volume("teamChange", 200);
	audio->addAudio("../assets/Audio/HUD/enemySelected.mp3", "enemySelected");
	audio->volume("enemySelected", 100);

	audio->addAudio("../assets/Audio/Enemy/enemy.mp3", "enemyDeath");
	audio->volume("enemyDeath", 100);

	audio->addAudio("../assets/Audio/Voice/BaseUnderAttack.mp3", "baseAttack");
	audio->volume("baseAttack", 100);
	audio->addAudio("../assets/Audio/Voice/EnemiesApproaching.mp3", "enemiesComing");
	audio->volume("EnemiesComing", 100);
	audio->addAudio("../assets/Audio/Voice/EnemyEliminated.mp3", "enemiesDestroyed");
	audio->volume("EnemiesDestroyed", 100);
	audio->addAudio("../assets/Audio/Voice/UnitReady.mp3", "unitReady");
	audio->volume("unitReady", 100);



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


void startGame(std::string fname, glm::vec3 &cameraTranslatePos, float cameraZoom, GameObject* background, HUD* selectionGraphic, int size) {

	_state = Game;
	audio->stop("menu");
	audio->playRepeat("background");

	std::pair<int, int> tempSize = fileLoader.loadMapSize(fname);
	float wid = tempSize.first;
	float height = tempSize.second;
	int start = 1149;

	g = new Graph(wid, height, GameObject(glm::vec3(0.0f), textures["Map"][0], size, "map"), texMap, fname, window_width_g, window_height_g, window);

	std::cout << "aydddydd" << std::endl;

	cameraTranslatePos = g->getFocalPoint(0);
	start = *(g->getBotStartSet().begin());
	g->setZoom(cameraZoom);
	g->setCamPos(cameraTranslatePos);

	g->setStart(start);

	g->setStart(start);
	for (HUD* h : hudObjects) {
		h->setCamPos(cameraTranslatePos);
	}
	for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
	for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
	selectionGraphic->setCamPos(cameraTranslatePos);


	std::cout << "ayydd" << std::endl;
	background->setTex(textures["Background"][2]);
	background->setImgScale(glm::vec3(41, 41, 1));
	background->setPosition(glm::vec3(-0.1f, -5.0f, 0.0f));

}

// Main function that builds and runs the game
int main(void){
	try {

		/************************************************AUDIO INIT************************************************/


		audio = new Audio();

		setAudioTracks();

		/************************************************OPENGL INIT************************************************/

		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		if (!glfwInit()) {
			throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
		}
		// Setup window

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

		shaders.push_back(new Shader("resource/shader.vert", "resource/shader.frag", 0));
		shaders[0]->disable();
		shaders.push_back(new Shader("resource/shaderParticle.vert", "resource/shaderParticle.frag", 1));
		shaders[1]->CreateParticleArray(1);
		shaders[1]->disable();
		shaders.push_back(new Shader("resource/shaderEnemyDeath.vert", "resource/shaderEnemyDeath.frag", 2));
		shaders[2]->CreateParticleArray(2);
		shaders[2]->setRadius(2);
		shaders[2]->disable();

		shaders[0]->setAttribute(0);



		/************************************************TEXTURE INIT************************************************/
		setallTexture();

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
		std::map<std::string, float> upgrades[2];
		std::string upgradeKeys[6] = { "Upgrade Damage","Upgrade Rate of Fire","Upgrade Range","Upgrade Hardiness","Upgrade Speed","Increase Cost" };
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 6; ++j) {
				upgrades[i][upgradeKeys[j]]=1;
			}
		}
		std::map<std::string, long> stats[2];
		std::string statKeys[5] = { "Creeps Killed: ", "Creeps Bought: ", "Damage Dealt: ", "Towers Built: ", "Total Credits Spent: "};
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 5; ++j) {
				stats[i][statKeys[j]] = 0;
			}
		}
		int roundNum=1;
		long gameTime = glfwGetTime();

		int hours = gameTime / 3600;
		int minutes = (gameTime - hours * 3600) / 60;

		int seconds = gameTime - hours * 3600 - minutes * 60;
		std::string m = std::to_string(minutes);

		if (m.length() == 1) {

			m.insert(0, "0");

		}

		std::string s = std::to_string(seconds);

		if (s.length() == 1) {

			s.insert(0, "0");

		}

		glm::vec3 hudColors[2] = { glm::vec3(50, 175, 255),glm::vec3(179, 0, 0) };

		int hp[2] = { 20,20 };
		int turnIndex = 0;
		char turn = turnArr[turnIndex];

		int level = 2;
		std::string fname = "../assets/Levels/map"+std::to_string(level)+".csv";
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
		float currentCameraZoom = cameraZoom;

		float camShiftInc = 0.05f;
		float camZoomInc = 0.01f;

		bool gottaGoFast = false;
		bool hpUp = false;
		bool rainingLead = false;
		float rainingLeadEnd = 0;

		EnemyObject* selectedEnemy = NULL;


		float enemyX, enemyY, oldEnemyX, oldEnemyY;
		float spawnCount = 0;
		bool startWave=1;
		bool toggleBlock = false;

		GameObject* background = new GameObject(glm::vec3(3.0f, -6.3f, 0.0f), textures["Background"][0], size, "map");
		background->setImgScale(glm::vec3(90, 46, 46));

		/************************************************FILE INIT************************************************/

		std::vector < std::string>  discriptionTexts = fileLoader.LoadVectorTextFile("../assets/Descriptions/discriptions.txt");
		std::cout << "size = " << discriptionTexts.size() << std::endl;
		/************************************************GRAPH INIT************************************************/

		glm::vec3 cameraTranslatePos = glm::vec3(0.1125f, 6.3f, 0.0f);
		glm::vec3 currentCameraTranslatePos = cameraTranslatePos;



		/************************************************ENEMY INIT************************************************/

		enemyMap[0] = new std::vector<EnemyObject*>;
		enemyMap[1] = new std::vector<EnemyObject*>;

		towerMap[0] = new std::vector<TowerObject*>;
		towerMap[1] = new std::vector<TowerObject*>;


		Node* cur;
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NEW STUFF
		glm::vec3 numbersStartPosition = glm::vec3(7.5f, 7.0f, 0.0f);
		glm::vec3 powerUpStartPosition = glm::vec3(3.0f, 6.0f, 0.0f);

		powerUpList.push_back(new PowerUpObject(powerUpStartPosition, textures["MENU"][1], size, "Null Zone", 1.5, 0, 5000));
		powerUpStartPosition.x -= 1.5;
		powerUpList.push_back(new PowerUpObject(powerUpStartPosition, textures["MENU"][2], size, "Time Stop", 1.5, 4, 2500));
		powerUpStartPosition.x -= 1.5;
		powerUpList.push_back(new PowerUpObject(powerUpStartPosition, textures["MENU"][3], size, "Morphine", -1, 20, 1000));
		powerUpStartPosition.x -= 1.5;
		powerUpList.push_back(new PowerUpObject(powerUpStartPosition, textures["MENU"][4], size, "Stimpack", -1, 20, 2000));
		powerUpStartPosition.x -= 1.5;
		powerUpList.push_back(new PowerUpObject(powerUpStartPosition, textures["MENU"][5], size, "Give 'Em Lead", -1, 15, 1250));

		glm::vec3 upgradeUpStartPosition = glm::vec3(-6.0f, 7.3f, 0.0f);
		upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][6], size, "Upgrade Damage",1.2, 500));
		upgradeUpStartPosition.y += 1;
		upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][7], size, "Upgrade Rate of Fire",1.1,1000));
		upgradeUpStartPosition.y += 1;
		upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][8], size, "Upgrade Range",1.1,750));

		upgradeUpStartPosition = glm::vec3(6.0f, 7.3f, 0.0f);
		upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][9], size, "Upgrade Hardiness",1.2,500));
		upgradeUpStartPosition.y += 1;
		upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][10], size, "Upgrade Speed",1.1,1000));
		upgradeUpStartPosition.y += 1;
		upgradeList.push_back(new UpgradeObject(upgradeUpStartPosition, textures["MENU"][11], size, "Increase Cost",1.5,0));

		powerUpListHUD.push_back(new PowerUpObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][1], size, "Null Zone", 1.5, 0, 5000));
		powerUpListHUD.push_back(new PowerUpObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][2], size, "Time Stop", 1.5, 4, 2500));
		powerUpListHUD.push_back(new PowerUpObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][3], size, "Morphine", -1, 20, 1000));
		powerUpListHUD.push_back(new PowerUpObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][4], size, "Stimpack", -1, 20, 2000));
		powerUpListHUD.push_back(new PowerUpObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][5], size, "Give 'Em Lead", -1, 15, 1250));

		upgradeListHUD.push_back(new UpgradeObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][6], size, "Upgrade Damage", 1.2, 500));
		upgradeListHUD.push_back(new UpgradeObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][7], size, "Upgrade Rate of Fire", 1.1, 1000));
		upgradeListHUD.push_back(new UpgradeObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][8], size, "Upgrade Range", 1.1, 750));

		upgradeListHUD.push_back(new UpgradeObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][9], size, "Upgrade Hardiness", 1.2, 500));
		upgradeListHUD.push_back(new UpgradeObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][10], size, "Upgrade Speed", 1.1, 1000));
		upgradeListHUD.push_back(new UpgradeObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["MENU"][11], size, "Increase Cost", 1.5, 0));

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NEW STUFF
		int index2 = 0;

		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 5,   "C-Class BASIC",    2, 0.5, 10, 0.2)); index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 25,  "C-Class DEFENDER", 1.5, 1, 20, 0.4)); index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 0,   "C-Class BARRIER",  0, 0, 5, 0.0)); index2 += 4;

		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 7,   "B-Class IGNITION Cannon", 1, 0.1, 50,  0.1)); index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 10,  "AOE",                     1, 1,   75,  4));index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 500, "A-Class Stealth Sniper",  5, 4,   150, 0.6));index2 += 4;

		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 200, "A-Class Auto Assault Bomber", 0.85, 2, 10000, 0.5));index2 += 4;
		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index2, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index2), textures["Explosion"], size, 100,   "A-Class High Beam Laser",     4, 4, 500, 0));
		towerDetailHUD.push_back(new TowerObject(glm::vec3(1.8f, 3.8f, 0.0f), std::vector<GLuint>(createTexture("../assets/Graphics/Tower/sell.png"), createTexture("../assets/Graphics/Tower/sell.png")) , textures["Explosion"], size, 0, "Sell", 0, 0, 0, 0));


		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][0], size, 50, "Servent Minor", textures["Particle"][1], 1.0, 10, 0));//0
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][1], size, 200, "Glutton Minor", textures["Particle"][1], 0.8, 40, 0));//1
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][2], size, 100, "Fleeting Minor", textures["Particle"][1], 1.7, 70, 0));//2

		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][3], size, 150, "Lifegiver Minor", textures["Particle"][1], 1, 100, 10));//3
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][4], size, 350, "Fleeting Colossal", textures["Particle"][1], 1.5, 250, 0));//4
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][5], size, 400, "Life Colossal", textures["Particle"][1], 1.0, 400, 15));//5

		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][6], size, 300, "Fleeting Lifeblood", textures["Particle"][1], 1.6, 500, 15));//6
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][7], size, 1000, "Absolute Zenith", textures["Particle"][1], 2.0, 1500, 20));//7
		enemyDetailHUD.push_back(new EnemyObject(glm::vec3(1.8f, 3.8f, 0.0f), textures["Enemy"][8], size, 1000000, "Undying Zenith", textures["Particle"][1], 2.5, 1000000, 1000000));//7


		for (int i = 0; i < towerDetailHUD.size(); i++) {
			towerDetailHUD[i]->setDescription(discriptionTexts[i]);
		}
		for (int i = 0; i < enemyDetailHUD.size(); i++) {
			enemyDetailHUD[i]->setDescription(discriptionTexts[i+9]);
		}
		for (int i = 0; i < powerUpListHUD.size(); i++) {
			powerUpListHUD[i]->setDescription(discriptionTexts[i+18]);
		}
		for (int i = 0; i < upgradeListHUD.size(); i++) {
			upgradeListHUD[i]->setDescription(discriptionTexts[i+23]);
		}

		towerDetailHUD[0]->setHotKey("U");
		towerDetailHUD[1]->setHotKey("I");
		towerDetailHUD[2]->setHotKey("O");
		towerDetailHUD[3]->setHotKey("J");
		towerDetailHUD[4]->setHotKey("K");
		towerDetailHUD[5]->setHotKey("L");
		towerDetailHUD[6]->setHotKey("N");
		towerDetailHUD[7]->setHotKey("M");
		towerDetailHUD[8]->setHotKey(",");

		enemyDetailHUD[0]->setHotKey("Q");
		enemyDetailHUD[1]->setHotKey("W");
		enemyDetailHUD[2]->setHotKey("E");
		enemyDetailHUD[3]->setHotKey("A");
		enemyDetailHUD[4]->setHotKey("S");
		enemyDetailHUD[5]->setHotKey("D");
		enemyDetailHUD[6]->setHotKey("Z");
		enemyDetailHUD[7]->setHotKey("X");
		enemyDetailHUD[8]->setHotKey("C");

		powerUpListHUD[0]->setHotKey("1");
		powerUpListHUD[1]->setHotKey("2");
		powerUpListHUD[2]->setHotKey("3");
		powerUpListHUD[3]->setHotKey("4");
		powerUpListHUD[4]->setHotKey("5");

		upgradeListHUD[0]->setHotKey("Y");
		upgradeListHUD[1]->setHotKey("H");
		upgradeListHUD[2]->setHotKey("B");
		upgradeListHUD[3]->setHotKey("R");
		upgradeListHUD[4]->setHotKey("F");
		upgradeListHUD[5]->setHotKey("V");


		/************************************************blueprints INIT************************************************/

		//The blueprints are used to store a single type of tower, then once the player has placed one, it will use its variables
		//to create the actual tower object.
		int index = 0;
																																																			                        	//damage                            range  rof  cost  speed
		blueprints.push_back(new TowerObject(glm::vec3(-7.35f, 7.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 5,   "C-Class BASIC",               2,   0.5, 10,  0.2)); index += 4; //the final 5 here is the cost
		blueprints.push_back(new TowerObject(glm::vec3(-8.35f, 7.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 25,  "C-Class DEFENDER",            1.5, 1,   20, 0.4)); index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.35f, 7.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 0,   "C-Class BARRIER",             0,   0,   5,  0.0)); index += 4;

		blueprints.push_back(new TowerObject(glm::vec3(-7.35f, 8.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10,   "B-Class IGNITION Cannon",     1,   3, 50,  0.1)); index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-8.35f, 8.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Button"], size,    10,  "AOE",                         1,   3,   75,  4));index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.35f, 8.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 500, "A-Class Stealth Sniper",      5,   4,   150, 0.6)); index += 4;

		blueprints.push_back(new TowerObject(glm::vec3(-7.35f, 9.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 200, "A-Class Auto Assault Bomber", 0.85,  2,   10000, 0.5));index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-8.35f, 9.3f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 100,   "A-Class High Beam Laser",     4,  4,   500, 0));//index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.35f, 9.3f, 0.0f), std::vector<GLuint>(createTexture("../assets/Graphics/Tower/sell.png"), createTexture("../assets/Graphics/Tower/sell.png")), textures["Explosion"], size, 5,   "Sell",     0,  0,   0, 0));//index += 4;



		/************************************************enemyBlueprints INIT************************************************/
//																								         health				                                    speed, cost regen
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.35f, 7.3f, 0.0f), textures["Enemy"][0], size, 50,      "Servent Minor",      textures["Particle"][1], 1.0, 10,   0));//0
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.35f, 7.3f, 0.0f), textures["Enemy"][1], size, 200,     "Glutton Minor",      textures["Particle"][1], 0.8, 40,   0));//1
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.35f, 7.3f, 0.0f), textures["Enemy"][2], size, 100,     "Fleeting Minor",     textures["Particle"][1], 1.7, 70,   0));//2

		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.35f, 8.3f, 0.0f), textures["Enemy"][3], size, 150,     "Lifegiver Minor",    textures["Particle"][1], 1,   100,  10));//3
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.35f, 8.3f, 0.0f), textures["Enemy"][4], size, 350,     "Fleeting Colossal",  textures["Particle"][1], 1.5, 250,  0));//4
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.35f, 8.3f, 0.0f), textures["Enemy"][5], size, 400,     "Life Colossal",      textures["Particle"][1], 1.0, 400,  15));//5

		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.35f, 9.3f, 0.0f), textures["Enemy"][6], size, 300,     "Fleeting Lifeblood", textures["Particle"][1], 1.6, 500,  15));//6
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.35f, 9.3f, 0.0f), textures["Enemy"][7], size, 1000,    "Absolute Zenith",    textures["Particle"][1], 2.0, 1500, 20));//7
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.35f, 9.3f, 0.0f), textures["Enemy"][8], size, 1000000, "Undying Zenith",     textures["Particle"][1], 2.5, 1000000,   1000000));//7

		/*for (TowerObject* t : blueprints) {
			towerDetailHUD.push_back(t);
		}
		for (EnemyObject* e : enemyBlueprint) {
			enemyDetailHUD.push_back(e);
		}*/
		/************************************************buttonBlueprints INIT************************************************/
																																						  /************************************************enemyCounters INIT************************************************/
		enemyCounters.push_back(new EnemyObject(glm::vec3(12.0f, 4.4f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters.push_back(new EnemyObject(glm::vec3(9.5f, 4.4f, 0.0f), textures["Enemy"][1], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters.push_back(new EnemyObject(glm::vec3(7.0f, 4.4f, 0.0f), textures["Enemy"][2], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3
		enemyCounters.push_back(new EnemyObject(glm::vec3(4.5f, 4.4f, 0.0f), textures["Enemy"][3], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0

		enemyCounters.push_back(new EnemyObject(glm::vec3(12.0f, 5.4f, 0.0f), textures["Enemy"][4], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters.push_back(new EnemyObject(glm::vec3(9.5f, 5.4f, 0.0f), textures["Enemy"][5], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3
		enemyCounters.push_back(new EnemyObject(glm::vec3(7.0f, 5.4f, 0.0f), textures["Enemy"][6], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters.push_back(new EnemyObject(glm::vec3(4.5f, 5.4f, 0.0f), textures["Enemy"][7], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4

		enemyCounters2.push_back(new EnemyObject(glm::vec3(-2.8f, 4.4f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-5.3f, 4.4f, 0.0f), textures["Enemy"][1], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-7.8f, 4.4f, 0.0f), textures["Enemy"][2], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-10.3f, 4.4f, 0.0f), textures["Enemy"][3], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3

		enemyCounters2.push_back(new EnemyObject(glm::vec3(-2.8f, 5.4f, 0.0f), textures["Enemy"][4], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-5.3f, 5.4f, 0.0f), textures["Enemy"][5], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-7.8f, 5.4f, 0.0f), textures["Enemy"][6], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3
		enemyCounters2.push_back(new EnemyObject(glm::vec3(-10.3f, 5.4f, 0.0f), textures["Enemy"][7], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0

		/************************************************buttonBlueprints INIT************************************************/
		turnButtons.push_back(new GameObject(glm::vec3(-0.5f, 1.75f, 0.0f), textures["Button"][1], size, "T"));
		turnButtons.push_back(new GameObject(glm::vec3(0.5f, 1.75f, 0.0f), textures["Button"][2], size, "G"));

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
		glm::vec3 objectS9 = glm::vec3(0.7f, 1.0f, 0.0f);
		HUD* selectionGraphic = new HUD(glm::vec3(50.0f, 50.0f, 0.0f), cameraZoom, glm::vec3(0.1f, 0.1f, 0.0f), selectGraphicTex, size, factor, "selection", window);
		HUD* selectionGraphic2 = new HUD(glm::vec3(50.0f, 50.0f, 50.0f), cameraZoom, objectS9, selectGraphicTex, size, factor, "selection", window);

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
		objectS.y = 0.23f;
		hudObjects.push_back(new HUD(glm::vec3(0.65f, 1.7f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD3", window));//3
		hudObjects[6]->setEnemyBlueprints(enemyCounters);
		hudObjects.push_back(new HUD(glm::vec3(-0.65f, 1.7f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4", window));//4

		hudObjects[7]->setEnemyBlueprints(enemyCounters2);
		objectS.x = 0.35f;
		hudObjects.push_back(new HUD(glm::vec3(0.0f, 1.7f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4", window));//extra hud is out of frame here original position glm::vec3(0.0f, 0.9f, 0.0f)
		hudObjects[8]->setGameObjects(turnButtons);
		objectS.x = 2.0f;
		objectS.y = 0.4f;
		hudObjects.push_back(new HUD(glm::vec3(-0.158f, 2.0f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4", window));
		hudObjects[9]->setUpgrades(upgradeList);

		//MAP SELECTION HUD
		float mapBoxSize = 0.6f;
		objectS = glm::vec3(1.0f*mapBoxSize, 1.3f*mapBoxSize, 1.0f*mapBoxSize);
		float mapBoxOffset = 0.04f;
		mapHUD.push_back(new HUD(glm::vec3(1.1 + mapBoxOffset, 1.1f, 0.0f), cameraZoom, objectS, textures["MAPBOX"][0], size, factor, "map1", window));
		mapHUD.push_back(new HUD(glm::vec3(0.0f + mapBoxOffset, 1.1f, 0.0f), cameraZoom, objectS, textures["MAPBOX"][1], size, factor, "map2", window));
		mapHUD.push_back(new HUD(glm::vec3(-1.1f + mapBoxOffset, 1.1f, 0.0f), cameraZoom, objectS, textures["MAPBOX"][2], size, factor, "map3", window));
		mapHUD.push_back(new HUD(glm::vec3(0.65f + mapBoxOffset, 2.35f, 0.0f), cameraZoom, objectS, textures["MAPBOX"][3], size, factor, "map4", window));
		mapHUD.push_back(new HUD(glm::vec3(-0.65f + mapBoxOffset, 2.35f, 0.0f), cameraZoom, objectS, textures["MAPBOX"][4], size, factor, "map5", window));

		gameOverHUD = new HUD(glm::vec3(2.0, 2.1f, 0.0f), cameraZoom, glm::vec3(0.4f, 0.4f, 0.0f), textures["Enemy"][0], size, factor, "GameOver", window);
		pauseHUD = new HUD(glm::vec3(2000.0, 2.1f, 0.0f), cameraZoom, glm::vec3(0.4f, 0.4f, 0.0f), textures["Enemy"][0], size, factor, "Pause", window);


		/************************************************MENU INIT************************************************/
		glm::vec3 buttonScale = glm::vec3(0.5f, 0.5f, 0.0f);
		HUDMenu.push_back(new HUD(glm::vec3(0.0f, -0.7f, 0.0f), cameraZoom, buttonScale, textures["MENUHUD"][0], size, factor, "PLAY", window));
		HUDMenu.push_back(new HUD(glm::vec3(0.0f, 0.7f, 0.0f), cameraZoom, buttonScale, textures["MENUHUD"][1], size, factor, "QUIT", window));

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
		hudObjects[6]->addText(new Text(glm::vec3(-18.5f, -7.0f, 0.0f), fontTexture, std::to_string(normalCount), size, 0.05f, glm::vec3(50, 175, 255), "P1normal"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-14.5f, -7.0f, 0.0f), fontTexture, std::to_string(fastCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fast"));
		hudObjects[6]->addText(new Text(glm::vec3(-10.5f, -7.0f, 0.0f), fontTexture, std::to_string(heartyCount), size, 0.05f, glm::vec3(50, 175, 255), "P1hearty"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-6.5f, -7.0f, 0.0f), fontTexture, std::to_string(flyingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1flying"));//0	|

		hudObjects[6]->addText(new Text(glm::vec3(-18.5f, -8.5f, 0.0f), fontTexture, std::to_string(splitterCount), size, 0.05f, glm::vec3(50, 175, 255), "P1splitter"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-14.5f, -8.5f, 0.0f), fontTexture, std::to_string(regeneratingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1regenerating"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-10.5f, -8.5f, 0.0f), fontTexture, std::to_string(fast_heartyCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fasthearty"));//0	|
		hudObjects[6]->addText(new Text(glm::vec3(-6.5f, -8.5f, 0.0f), fontTexture, std::to_string(fast_flyingCount), size, 0.05f, glm::vec3(50, 175, 255), "P1fastflying"));//0	|


		//=============================================================================================================================
		hudObjects[7]->addText(new Text(glm::vec3(5.3f, -7.0f, 0.0f), fontTexture, std::to_string(normalCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2normal"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(9.3f, -7.0f, 0.0f), fontTexture, std::to_string(fastCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fast"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(13.3f, -7.0f, 0.0f), fontTexture, std::to_string(heartyCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2hearty"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(17.3f, -7.0f, 0.0f), fontTexture, std::to_string(flyingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2flying"));//0	|

		hudObjects[7]->addText(new Text(glm::vec3(5.3f, -8.5f, 0.0f), fontTexture, std::to_string(splitterCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2splitter"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(9.3f, -8.5f, 0.0f), fontTexture, std::to_string(regeneratingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2regenerating"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(13.3f, -8.5f, 0.0f), fontTexture, std::to_string(fast_heartyCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fasthearty"));//0	|
		hudObjects[7]->addText(new Text(glm::vec3(17.3f, -8.5f, 0.0f), fontTexture, std::to_string(fast_flyingCount2), size, 0.05f, glm::vec3(50, 175, 255), "P2fastflying"));//0	|

		hudObjects[2]->addText(new Text(glm::vec3(-14.0f, 13.9f, 0.0f), fontTexture, "FPS: ", size, 0.07f, glm::vec3(0, 0, 0), "FPS"));

		//==========================================================//===============================================>>>new stufff
		hudObjects[5]->addText(new Text(glm::vec3(-12.0f, -18.0f, 0.0f), fontTexture, "NAME:", size, 0.04f, glm::vec3(50, 175, 255), "name"));//0
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -19.0f, 0.0f), fontTexture, "COST:", size, 0.04f, glm::vec3(50, 175, 255), "cost"));//1
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -20.0f, 0.0f), fontTexture, "HOTKEY:", size, 0.04f, glm::vec3(50, 175, 255), "hotkey"));//2
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -21.0f, 0.0f), fontTexture, "DAMAGE:", size, 0.04f, glm::vec3(50, 175, 255), "towerdamage"));//3
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -22.0f, 0.0f), fontTexture, "RATE OF FIRE:", size, 0.04f, glm::vec3(50, 175, 255), "towerrof"));//4
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -23.0f, 0.0f), fontTexture, "RANGE:", size, 0.04f, glm::vec3(50, 175, 255), "towerrange"));//5
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -21.0f, 0.0f), fontTexture, "HP:", size, 0.04f, glm::vec3(50, 175, 255), "enemyhp"));//6
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -22.0f, 0.0f), fontTexture, "SPEED:", size, 0.04f, glm::vec3(50, 175, 255), "enemyspeed"));//7
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -23.0f, 0.0f), fontTexture, "REGEN:", size, 0.04f, glm::vec3(50, 175, 255), "enemyregen"));//8

		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -21.0f, 0.0f), fontTexture, "RANGE:", size, 0.04f, glm::vec3(50, 175, 255), "powerupRange"));//9
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -22.0f, 0.0f), fontTexture, "DURATION:", size, 0.04f, glm::vec3(50, 175, 255), "powerupDuration"));//10
		hudObjects[5]->addText(new Text(glm::vec3(-7.0f, -21.0f, 0.0f), fontTexture, "MOD:", size, 0.04f, glm::vec3(50, 175, 255), "upgradeMod"));//11
																																				  //=============================================================================================================================
		HUD* off = new HUD(glm::vec3(0.0f, 1.5f, 0.0f), 0.22f, glm::vec3(1.0f, 1.0f, 0.0f), textures["MENU"][0], size, factor, "", window);
		off->addText(new Text(glm::vec3(-6.0f, -10.0f, 0.0f), fontTexture, "PRESS 9 TO FOR MAIN MENU", size, 0.09f, glm::vec3(50, 175, 255), "upgradeMod"));//11
		off->addText(new Text(glm::vec3(-6.0f, -23.0f, 0.0f), fontTexture, "PRESS 9 TO FOR MAIN MENU", size, 0.09f, glm::vec3(50, 175, 255), "upgradeMod"));//11
		hudObjects[5]->addText(new Text(glm::vec3(-12.0f, -19.0f, 0.0f), fontTexture, "s: ", size, 0.04f, glm::vec3(50, 175, 255), "discription"));//11

		mapHUD[0]->addText(new Text(glm::vec3(-17.8f, -25.5f, 0.0f), fontTexture, "New Texas (Easy)", size, 0.05f, glm::vec3(255, 255, 255), "mapbox"));//11
		mapHUD[1]->addText(new Text(glm::vec3(-5.3f, -25.5f, 0.0f), fontTexture, "Havan City (NORMAL)", size, 0.05f, glm::vec3(255, 255, 255), "mapbox"));//11
		mapHUD[2]->addText(new Text(glm::vec3(7.8f, -25.5f, 0.0f), fontTexture, "Three Sols (NORMAL)", size, 0.05f, glm::vec3(255, 255, 255), "mapbox"));//11
		mapHUD[3]->addText(new Text(glm::vec3(-14.0f, -45.0f, 0.0f), fontTexture, "New World Order (HARD)", size, 0.05f, glm::vec3(255, 255, 255), "mapbox"));//11
		mapHUD[4]->addText(new Text(glm::vec3(0.0f, -45.0f, 0.0f), fontTexture, "Infernal Battlefield (HARD)", size, 0.05f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-5.5f, -8.0f, 0.0f), fontTexture, "VICTOR:", size, 0.1f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-5.5f, -9.0f, 0.0f), fontTexture, "Round:", size, 0.1f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-5.5f, -10.0f, 0.0f), fontTexture, "Game Time:", size, 0.1f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-5.8f, -3.0f, 0.0f), fontTexture, "CLICK ANYWHERE TO QUIT", size, 0.15f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-8.0f, -16.0f, 0.0f), fontTexture, "STATS", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-3.0f, -16.0f, 0.0f), fontTexture, "PLAYER1", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(2.5f, -16.0f, 0.0f), fontTexture, "PLAYER2", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-8.0f, -18.0f, 0.0f), fontTexture, "Creeps\nKilled", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-8.0f, -21.0f, 0.0f), fontTexture, "Creeps\nBought", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-8.0f, -24.0f, 0.0f), fontTexture, "Damage\nDealt", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-8.0f, -27.0f, 0.0f), fontTexture, "Towers\nBuilt", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(-8.0f, -30.0f, 0.0f), fontTexture, "Total\nCredits\nSpent", size, 0.07f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-4.0f, -22.0f, 0.0f), fontTexture,"0Creeps Killed: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(2.4f, -22.0f, 0.0f), fontTexture, "1Creeps Killed: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-4.0f, -25.5f, 0.0f), fontTexture, "0Creeps Bought: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(2.4f, -25.5f, 0.0f), fontTexture, "1Creeps Bought: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-4.0f, -29.0f, 0.0f), fontTexture, "0Damage Dealt: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(2.4f, -29.0f, 0.0f), fontTexture, "1Damage Dealt: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-4.0f, -33.0f, 0.0f), fontTexture, "0Towers Built: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(2.4f, -33.0f, 0.0f), fontTexture, "1Towers Built: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11

		gameOverHUD->addText(new Text(glm::vec3(-4.0f, -37.0f, 0.0f), fontTexture, "0Total Credits Spent: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11
		gameOverHUD->addText(new Text(glm::vec3(2.4f, -37.0f, 0.0f), fontTexture, "1Total Credits Spent: ", size, 0.06f, glm::vec3(255, 255, 255), "mapbox"));//11

		pauseHUD->addText(new Text(glm::vec3(-5.8f, -3.0f, 0.0f), fontTexture, "CLICK ANYWHERE TO QUIT", size, 0.15f, glm::vec3(255, 255, 255), "mapbox"));//11

		for (HUD* h : hudObjects) {
			h->setUpmod(upgrades[turnIndex]);
		}

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

		glfwSetFramebufferSizeCallback(window, ResizeCallback);
		while (!glfwWindowShouldClose(window)) {
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

				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);


				for (HUD* b : HUDMenu)b->setCamPos(cameraTranslatePos);

				if (timeOfLastMove + 0.05 < glfwGetTime()) {
					if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))
					{
						_state = MapMenu;
						timeOfLastMove = glfwGetTime();
						background->setTex(textures["Background"][1]);
					}

				}
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					if ((300.8 * factor <= xpos && xpos <= 496 * factor) && (124.8 * factor <= ypos && ypos <= 265.6 * factor)) {//start first row
						_state = MapMenu;
						timeOfLastMove = glfwGetTime();

						background->setTex(textures["Background"][1]);
					}
					if ((300.8 * factor <= xpos && xpos <= 496 * factor) && (333.6 * factor <= ypos && ypos <= 472.8 * factor)) {//start first row
						glfwSetWindowShouldClose(window, 1);
					}
				}
				break;
			}
			case MapMenu: {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				if ((16 * factor <= xpos && xpos <= 254.4 * factor) && (26.4 * factor <= ypos && ypos <= 256.8 * factor)) {//start first row
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.15< glfwGetTime()))startGame("../assets/Levels/map" + std::to_string(1) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
						selectionGraphic2->setPosition(glm::vec3(1.0f, 0.85f, 0.0f));

				}
				if ((282.4 * factor <= xpos && xpos <= 517.6 * factor) && (26.4 * factor <= ypos && ypos <= 256.8 * factor)) {//start first row
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime()))startGame("../assets/Levels/map" + std::to_string(2) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
						selectionGraphic2->setPosition(glm::vec3(0.07f, 0.85f, 0.0f));
				}
				if ((546.6* factor <= xpos && xpos <= 781.6 * factor) && (26.4 * factor <= ypos && ypos <= 256.8 * factor)) {//start first row
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.15< glfwGetTime()))startGame("../assets/Levels/map" + std::to_string(3) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
						selectionGraphic2->setPosition(glm::vec3(-0.9f, 0.85f, 0.0f));

				}
				//lower levels
				if ((127.2 * factor <= xpos && xpos <= 360.8 * factor) && (317.6 * factor <= ypos && ypos <= 550.4 * factor)) {//start first row
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.15< glfwGetTime()))startGame("../assets/Levels/map" + std::to_string(4) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
						selectionGraphic2->setPosition(glm::vec3(0.6f, 1.82f, 0.0f));
				}
				if ((436 * factor <= xpos && xpos <= 676.8 * factor) && (317.6 * factor <= ypos && ypos <= 550.4 * factor)) {//start first row
						if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.15< glfwGetTime()))startGame("../assets/Levels/map" + std::to_string(5) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
						selectionGraphic2->setPosition(glm::vec3(-0.5f, 1.82f, 0.0f));

				}
				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
					timeOfLastMove = glfwGetTime();
					_state = MainMenu;
					background->setTex(textures["Background"][0]);
				}

				if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
					timeOfLastMove = glfwGetTime();
					startGame("../assets/Levels/map" + std::to_string(1) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);

				}
				if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
					timeOfLastMove = glfwGetTime();
					startGame("../assets/Levels/map" + std::to_string(2) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
				}
				if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
					timeOfLastMove = glfwGetTime();
					startGame("../assets/Levels/map" + std::to_string(3) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
				}
				if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
					timeOfLastMove = glfwGetTime();
					startGame("../assets/Levels/map" + std::to_string(4) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
				}
				if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
					timeOfLastMove = glfwGetTime();
					startGame("../assets/Levels/map" + std::to_string(5) + ".csv", cameraTranslatePos, cameraZoom, background, selectionGraphic, size);
				}

				break;

			}


			

			case Game: {


				gameTime = glfwGetTime();
				if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && (timeOfLastMove + 0.3 < glfwGetTime()))
				{
					hudObjects[5]->setToggleInfo();
					timeOfLastMove = glfwGetTime();

				}
				if (timeOfLastMove + 0.05 < glfwGetTime()) {

					if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS &&
						(timeOfLastMove + 0.25 < glfwGetTime())) {
						_state = Pause;
						currentCameraTranslatePos = cameraTranslatePos;
						currentCameraZoom = cameraZoom;
						audio->stop("background");
						audio->playRepeat("menu");
						timeOfLastMove = glfwGetTime();

					}
					//begin zoom/pan
					double xpos, ypos;
					int window_width_g, window_height_g;
					glfwGetWindowSize(window, &window_width_g, &window_height_g);
					glfwGetCursorPos(window, &xpos, &ypos);
					bool inWindow = ypos > 0 && xpos > 0 && ypos < window_height_g && xpos < window_width_g;
					if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
						((ypos > 0 && ypos<30) && inWindow)) {
						cameraTranslatePos.y -= camShiftInc / cameraZoom;
					}
					if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
						((ypos > window_height_g - 30 && ypos < window_height_g) && inWindow)) {
						cameraTranslatePos.y += camShiftInc / cameraZoom;
					}
					if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
						((xpos > window_width_g - 30 && xpos < window_width_g) && inWindow)) {
						cameraTranslatePos.x -= camShiftInc / cameraZoom;
					}
					if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
						((xpos > 0 && xpos < 30) && inWindow)) {
						cameraTranslatePos.x += camShiftInc / cameraZoom;
					}
					if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
						cameraZoom = std::fmin(cameraZoom + camZoomInc, maxCamZoom);
						timeOfLastMove = glfwGetTime();
					}
					if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
						cameraZoom = std::fmax(cameraZoom - camZoomInc, minCamZoom);
						timeOfLastMove = glfwGetTime();
					}
					g->setZoom(cameraZoom);
					g->setCamPos(cameraTranslatePos);
					for (HUD* h : hudObjects) {
						h->setCamPos(cameraTranslatePos);
						h->setZoom(cameraZoom);
					}
					for (HUD* h : player1) {
						h->setCamPos(cameraTranslatePos);
						h->setZoom(cameraZoom);
					}
					for (HUD* h : player2) {
						h->setCamPos(cameraTranslatePos);
						h->setZoom(cameraZoom);
					}
					selectionGraphic->setCamPos(cameraTranslatePos);
					selectionGraphic->setZoom(cameraZoom);
					//end of zoom/pan

					if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !startWave) {
						startWave = 1;
						audio->playAgain("enemiesComing");
						roundOver = false;
						enemiesDestroyed = 0;
						numEnemiesSpawned = enemyMap[turnIndex]->size();


					}
					//powerup selection that is for some reason not handled in hud:

					//kill all in 1.5 range
					if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						audio->playAgain("menuClick");
						hudObjects[2]->setSelectionPowerUps(powerUpList[0]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[0]);



					}
					//Stop all in 1.5 range for 4 seconds
					if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						audio->playAgain("menuClick");
						hudObjects[2]->setSelectionPowerUps(powerUpList[1]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[1]);
					}
					//Inc outgoing creep hp 100% for 20 seconds.
					if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						audio->playAgain("menuClick");
						hudObjects[2]->setSelectionPowerUps(powerUpList[2]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[2]);

					}
					//Inc outgoing creep speed 50% for 20 seconds. up to 2.5 speed
					if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						audio->playAgain("menuClick");
						hudObjects[2]->setSelectionPowerUps(powerUpList[3]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[3]);

					}
					//inc allied tower rof by 100% for 15 seconds
					if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						audio->playAgain("menuClick");
						hudObjects[2]->setSelectionPowerUps(powerUpList[4]);
						hudObjects[5]->setSelectionPowerUps(powerUpListHUD[4]);
					}

					if ((glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS ||
						glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS ||
						glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS ||
						glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS ||
						glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) &&
						(timeOfLastMove + 0.15 < glfwGetTime())) {
						selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
						hudObjects[3]->setSelection(NULL);
						hudObjects[3]->setFlag(false);
						hudObjects[9]->setSelectionUpgrades(NULL);
						hudObjects[9]->setUpgradeFlag(false);
						hudObjects[4]->setSelectionEnemy(NULL);
						hudObjects[4]->setEnemyFlag(false);
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setFlag(false);
						hudObjects[5]->setEnemyFlag(false);
						hudObjects[5]->setPowerUpFlag(true);
						hudObjects[5]->setUpgradeFlag(false);
						cursor->setTex(textures["Cursor"][0]);
						selectedEnemy = NULL;
					}
					//end power up selection.

					//centre screen
					if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {
						cameraTranslatePos = g->getFocalPoint(turnIndex);
						g->setCamPos(cameraTranslatePos);
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
						cameraTranslatePos = g->getFocalPoint(turnIndex);
						g->setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects) {
							h->setCamPos(cameraTranslatePos);
							h->setUpmod(upgrades[turnIndex]);
						}
						for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
						for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
						timeOfLastMove = glfwGetTime();
						turn = turnArr[turnIndex];
						credits[turnIndex] += income[turnIndex];

						if (!turnIndex) {
							roundNum += 1;
						}
						g->clearNextNodeMaps();
						g->startPaths(turnIndex);

						audio->playAgain("teamChange");
					}

					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (timeOfLastMove + 0.2 < glfwGetTime())) {

						double xpos, ypos;
						glfwGetCursorPos(window, &xpos, &ypos);

						float x;
						float y;
						int id = g->getHover();
						g->getHoverCoords(x, y);


						if (ypos <= 390*factor) {//prints on map
							if (hudObjects[3]->getFlag()) {

								TowerObject* selectedTower = hudObjects[3]->getSelection();
								//selling towers
								if (selectedTower->getType().compare("Sell") == 0) {
									TowerObject *t = g->getNode(id).getTower();
									if (t != NULL) {
										audio->playAgain("menuClick");
										credits[turnIndex] += (int)(t->getCost() / 2);
										g->getNode(id).setTower(NULL);
										g->getNode(id).setTowerState(false, turn);
										g->startPaths(turnIndex);
										g->rePath(id, turn);

										for (std::vector<TowerObject*>::iterator it = towerMap[turnIndex]->begin(); it != towerMap[turnIndex]->end(); ++it) {
											if ((*it)->getUniqueID() == t->getUniqueID()) {
												towerMap[turnIndex]->erase(it);
												delete(t);
												break;
											}
										}

									}
								}
								else if (g->getNode(id).getBuildable(turn)) { //building towers
									if (credits[turnIndex] >= selectedTower->getCost()) { //if affordable
										TowerObject* t = new TowerObject(glm::vec3(x, y, 0.0f), selectedTower->getTexvec(),
											selectedTower->getExplosion_tex(), size, selectedTower->getDamage(),
											selectedTower->getType(), selectedTower->getRange(), selectedTower->getROF(),
											selectedTower->getCost(), selectedTower->getSpeed());
										t->setAudio(audio);
										t->upDamage(upgrades[turnIndex]["Upgrade Damage"]);
										t->upRange(upgrades[turnIndex]["Upgrade Range"]);
										t->upROF(upgrades[turnIndex]["Upgrade Rate of Fire"]);
										//apply upgrades

										g->getNode(id).setTowerState(true, turn);
										//if possible to path around
										if (g->startPaths(turnIndex) && g->rePath(id, turn)) {
											g->getNode(id).setTower(t);
											stats[turnIndex]["Towers Built: "] += 1;
											credits[turnIndex] -= selectedTower->getCost();
											stats[turnIndex]["Total Credits Spent: "] += selectedTower->getCost();
											towerMap[turnIndex]->push_back(t);

											if (rainingLead) {
												t->modCurROF(0.5);
												t->setEffectDuration(rainingLeadEnd - glfwGetTime());
												if (rainingLeadEnd - glfwGetTime() <= 0) {
													rainingLead = false;
												}
											}
											audio->playAgain("towerPlaced");
											audio->playAgain("unitReady");

										}
										else { //invalid location
											audio->playAgain("menuClick");
											g->getNode(id).setTowerState(false, turn);
											g->clearNextNodeMaps();
											g->startPaths(turnIndex);
											g->rePath(id, turn);
											delete(t);
										}
									}
								}
							}
						}
						else {
							showRadius = 0;
						}

						hudObjects[3]->selection(xpos, ypos);
						if (hudObjects[3]->getFlag()) {

							cursor->setTex(hudObjects[3]->getCursor());//sets the texture for the cursor with the tower icon <-----------
							selectionGraphic->setPosition(hudObjects[3]->getSelection()->getPosition());
							audio->playAgain("menuClick");

							hudObjects[2]->setSelectionPowerUps(NULL);
							hudObjects[5]->setSelectionPowerUps(NULL);
							hudObjects[2]->setPowerUpFlag(false);
							hudObjects[9]->setSelectionUpgrades(NULL);
							hudObjects[9]->setUpgradeFlag(false);
							hudObjects[4]->setSelectionEnemy(NULL);
							hudObjects[4]->setEnemyFlag(false);
							selectedEnemy = NULL;
						}

						hudObjects[2]->selectionPowerUp(xpos, ypos);
						if (hudObjects[2]->getSelectionPowerUps()!=NULL) {
							selectionGraphic->setPosition(hudObjects[2]->getSelectionPowerUps()->getPosition());
							hudObjects[3]->setFlag(false);
							hudObjects[2]->setSelectionPowerUps(NULL);

							audio->playAgain("menuClick");

							hudObjects[3]->setSelection(NULL);
							hudObjects[3]->setFlag(false);
							hudObjects[9]->setSelectionUpgrades(NULL);
							hudObjects[9]->setUpgradeFlag(false);
							hudObjects[4]->setSelectionEnemy(NULL);
							hudObjects[4]->setEnemyFlag(false);

							cursor->setTex(textures["Cursor"][0]);
							selectedEnemy = NULL;


						}


						hudObjects[9]->selectionUpgrades(xpos, ypos);
						if (hudObjects[9]->getSelectionUpgrades() !=NULL) {
							selectionGraphic->setPosition(hudObjects[9]->getSelectionUpgrades()->getPosition());
							hudObjects[2]->setSelectionPowerUps(NULL);
							hudObjects[5]->setSelectionPowerUps(NULL);
							hudObjects[2]->setPowerUpFlag(false);
							hudObjects[3]->setSelection(NULL);
							hudObjects[3]->setFlag(false);
							hudObjects[4]->setSelectionEnemy(NULL);
							hudObjects[4]->setEnemyFlag(false);

							showRadius = 0;
							cursor->setTex(textures["Cursor"][0]);
							selectedEnemy = NULL;
							hudObjects[9]->setSelectionUpgrades(NULL);
							audio->playAgain("menuClick");
						}
						//============================================================this is for the details render out put
						hudObjects[5]->selection(xpos, ypos);
						hudObjects[5]->selectionEnemy(xpos, ypos);
						hudObjects[5]->selectionPowerUp(xpos, ypos);
						hudObjects[5]->selectionUpgrades(xpos, ypos);
						//============================================================this is for the details render out put

						hudObjects[4]->selectionEnemy(xpos, ypos);

						if (hudObjects[4]->getSelectionEnemy() !=NULL) {

							hudObjects[2]->setSelectionPowerUps(NULL);
							hudObjects[5]->setSelectionPowerUps(NULL);
							hudObjects[2]->setPowerUpFlag(false);
							hudObjects[3]->setSelection(NULL);
							hudObjects[3]->setFlag(false);
							hudObjects[9]->setSelectionUpgrades(NULL);
							hudObjects[9]->setUpgradeFlag(false);
							audio->playAgain("menuClick");
							cursor->setTex(textures["Cursor"][0]);
							selectedEnemy = hudObjects[4]->getSelectionEnemy();
							selectionGraphic->setPosition(hudObjects[4]->getSelectionEnemy()->getPosition());
							showRadius = 0;
							hudObjects[4]->setSelectionEnemy(NULL);
						}

						hudObjects[6]->turns(xpos, ypos);

						if (hudObjects[7]->turns(xpos, ypos) == "wave" && !startWave) {
							startWave = 1;
							audio->playAgain("enemiesComing");
							roundOver = false;
							enemiesDestroyed = 0;
							numEnemiesSpawned = enemyMap[turnIndex]->size();
						}

						if (hudObjects[6]->turns(xpos, ypos) == "turn"  && enemyMap[turnIndex]->empty() && (timeOfLastMove + 0.15 < glfwGetTime())) {
							spawnCount = 0;
							startWave = 0;
							gottaGoFast = false;
							hpUp = false;
							rainingLead = false;
							turnIndex = turnIndex ^ 1;
							cameraTranslatePos = g->getFocalPoint(turnIndex);
							g->setCamPos(cameraTranslatePos);
							for (HUD* h : hudObjects) {
								h->setCamPos(cameraTranslatePos);
								h->setUpmod(upgrades[turnIndex]);
							}
							for (HUD* h : player1)h->setCamPos(cameraTranslatePos);
							for (HUD* h : player2)h->setCamPos(cameraTranslatePos);
							selectionGraphic->setCamPos(cameraTranslatePos);
							timeOfLastMove = glfwGetTime();
							turn = turnArr[turnIndex];
							credits[turnIndex] += income[turnIndex];
							g->clearNextNodeMaps();
							g->startPaths(turnIndex);
						}
						timeOfLastMove = glfwGetTime();

						//--------end of click button------------
					}

					hudObjects[4]->selectionEnemy(xpos, ypos);
					selectedEnemy = hudObjects[4]->getSelectionEnemy();
					PowerUpObject* selectedPower = hudObjects[5]->getSelectionPowerUps();
					UpgradeObject* selectedUpgrade = hudObjects[5]->getSelectionUpgrades();
					TowerObject* selectedTower = hudObjects[3]->getSelection();

					if (selectedPower != NULL) {
						showRadius = 1;
						selectionRadius = selectedPower->getRange();
						if (selectedPower->getType().compare("Null Zone") == 0 || selectedPower->getType().compare("Time Stop") == 0) {
							cursor->setTex(selectedPower->getTex());
						}
					}
					else if (selectedTower != NULL) {
						showRadius = 1;
						selectionRadius = selectedTower->getRange()*upgrades[turnIndex]["Upgrade Range"]*0.7;
						cursor->setTex(hudObjects[3]->getCursor());
					}

					//deselect
					if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
						cursor->setTex(textures["Cursor"][0]);
						selectedEnemy = NULL;
						selectedPower = NULL;
						selectedUpgrade = NULL;
						selectedTower = NULL;
						hudObjects[3]->setSelection(NULL);
						hudObjects[3]->setFlag(false);
						hudObjects[5]->setPowerUpFlag(false);
						hudObjects[5]->setSelectionPowerUps(NULL);
						hudObjects[2]->setSelectionPowerUps(NULL);
						showRadius = 0;
						selectionGraphic->setPosition(glm::vec3(1000.0f));
					}

					if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && (timeOfLastMove + 0.15 < glfwGetTime())) {

						if (selectedEnemy != NULL) { //if buying enemy
							if (credits[turnIndex] >= (int)(selectedEnemy->getCost()*upgrades[turnIndex]["Increase Cost"]) && enemyMap[turnIndex ^ 1]->size() < 200) {
								income[turnIndex] += (int)(selectedEnemy->getCost()*upgrades[turnIndex]["Increase Cost"]);
								credits[turnIndex] -= (int)(selectedEnemy->getCost()*upgrades[turnIndex]["Increase Cost"]);
								stats[turnIndex]["Total Credits Spent: "] += (int)(selectedEnemy->getCost()*upgrades[turnIndex]["Increase Cost"]);
								Node* cur;
								for (int s : g->getStartSet(turnArr[turnIndex ^ 1])) {
									cur = &g->getNode(s);
									stats[turnIndex]["Creeps Bought: "] += 1;
									EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), selectedEnemy->getTex(), size, selectedEnemy->getHealth(),
										selectedEnemy->getType(), selectedEnemy->getEnemyDeathTex(), selectedEnemy->getCurSpeed(), selectedEnemy->getCost(), selectedEnemy->getRegen());
									e->setAudio(audio);
									//apply upgrades
									e->upHardiness(upgrades[turnIndex]["Upgrade Hardiness"]);
									e->upSpeed(upgrades[turnIndex]["Upgrade Speed"]);
									e->upCost(upgrades[turnIndex]["Increase Cost"]);
									enemyMap[turnIndex ^ 1]->push_back(e);
									//powerups
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

								audio->playAgain("enemySelected");
							}
						}

						else if (selectedPower != NULL) {
							showRadius = 1;
							selectionRadius = selectedPower->getRange();
							if (credits[turnIndex] >= selectedPower->getCost()) {
								credits[turnIndex] -= selectedPower->getCost();

								if(selectedPower->getType().compare("Null Zone")==0){
									selectionRadius = 1.5;
									for (EnemyObject* e : enemiesInRange(g->getNode(g->getHover()), selectionRadius, enemyMap[turnIndex])) {
										if (e->getType().compare("Undying") != 0) {
											e->setExists(false);
											e->setKilled(true);
											credits[turnIndex] -= e->getCost();
										}
									}
								}
								else if(selectedPower->getType().compare("Time Stop") == 0){
									selectionRadius = 1.5;
									for (EnemyObject* e : enemiesInRange(g->getNode(g->getHover()), selectionRadius, enemyMap[turnIndex])) {
										e->modCurSpeed(0);
										e->setEffectDuration(4);
									}
								}
								else if(selectedPower->getType().compare("Morphine") == 0){
									hpUp = true;
									for (EnemyObject* e : *(enemyMap[turnIndex ^ 1])) {
										e->modCurHealthCap(2);
										e->setEffectDuration(20);
									}
								}
								else if(selectedPower->getType().compare("Stimpack") == 0){
									gottaGoFast = true;
									for (EnemyObject* e : *(enemyMap[turnIndex ^ 1])) {
										e->modCurSpeed(1.5);
										e->setEffectDuration(20);
									}
								}
								else if(selectedPower->getType().compare("Give 'Em Lead") == 0){
									rainingLead = true;
									rainingLeadEnd = glfwGetTime() + 15;
									for (TowerObject* t : *(towerMap[turnIndex])) {
										t->modCurROF(0.5);
										t->setEffectDuration(15);
									}
								}


							}
						}
						if (selectedUpgrade != NULL) {

							if (credits[turnIndex] >= selectedUpgrade->getCost()) {
								credits[turnIndex] -= selectedUpgrade->getCost();
								upgrades[turnIndex][selectedUpgrade->getType()] *= selectedUpgrade->getMod();
								for (TowerObject* t : *(towerMap[turnIndex])) {
									t->upDamage(upgrades[turnIndex]["Upgrade Damage"]);
									t->upRange(upgrades[turnIndex]["Upgrade Range"]);
									t->upROF(upgrades[turnIndex]["Upgrade Rate of Fire"]);
								}
								for (EnemyObject* e : *(enemyMap[turnIndex])) {
									e->upHardiness(upgrades[turnIndex]["Upgrade Hardiness"]);
									e->upSpeed(upgrades[turnIndex]["Upgrade Speed"]);
									e->upCost(upgrades[turnIndex]["Increase Cost"]);
								}
								for (HUD* h : hudObjects) {
									h->setUpmod(upgrades[turnIndex]);
								}
							}
						}
						timeOfLastMove = glfwGetTime();
					}

				}
				break;
			}
			case Pause: {
				gameTime = glfwGetTime();
				cameraTranslatePos = glm::vec3(0.1125f, 6.3f, 0.0f);
				cameraZoom = 0.22f;
				if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)&&
					(timeOfLastMove + 0.25 < glfwGetTime())) {
					_state = Game;
					audio->stop("menu");
					audio->playRepeat("background");
					timeOfLastMove = glfwGetTime();
					g->getFocalPoint(0);
					cameraTranslatePos = currentCameraTranslatePos;
					cameraZoom = currentCameraZoom;


				}


				break;
			}
			case GameOver: {
				cameraTranslatePos = glm::vec3(0.1125f, 6.3f, 0.0f);
				cameraZoom = 0.22f;
				if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					glfwSetWindowShouldClose(window, 1);
				}
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
				for (HUD* b : HUDMenu)b->render(shaders);

				background->render(shaders);
				break;
			}

			case MapMenu: {


				selectionGraphic2->render(shaders);
				for (HUD* h : mapHUD) {

					h->setFactor(factor);
					h->update(deltaTime);

					for (Text* t : h->getTextObjects()) {
						t->render(shaders);
					}

					h->render(shaders);

				}
				background->render(shaders);
				break;

			}
			case Game: {

				std::cout << "ssdddf" << std::endl;

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
					hudObjects[5]->setSelectionPowerUps(NULL);
					hudObjects[2]->setSelectionPowerUps(NULL);
					hudObjects[2]->setPowerUpFlag(false);
					hudObjects[3]->setSelection(NULL);
					hudObjects[3]->setFlag(false);
					hudObjects[9]->setSelectionUpgrades(NULL);
					hudObjects[9]->setUpgradeFlag(false);
					showRadius = 0;
					cursor->setTex(textures["Cursor"][0]);
					hudObjects[5]->updateHotkeysEnemy();
					hudObjects[5]->updateHotkeysEnemy();
					selectionGraphic->setPosition(hudObjects[4]->getSelectionEnemy()->getPosition());
					audio->playAgain("menuClick");
				}

				if (hudObjects[9]->updateHotkeysUpgrade()) {

					hudObjects[5]->setSelectionPowerUps(NULL);
					hudObjects[2]->setSelectionPowerUps(NULL);
					hudObjects[2]->setPowerUpFlag(false);
					hudObjects[3]->setSelection(NULL);
					hudObjects[3]->setFlag(false);
					hudObjects[4]->setSelectionEnemy(NULL);
					hudObjects[4]->setEnemyFlag(false);
					showRadius = 0;
					hudObjects[5]->updateHotkeysUpgrade();
					selectionGraphic->setPosition(hudObjects[9]->getSelectionUpgrades()->getPosition());
					audio->playAgain("menuClick");
				}



				if (hudObjects[3]->updateHotkeysTower()) {

					hudObjects[5]->setSelectionPowerUps(NULL);
					hudObjects[2]->setSelectionPowerUps(NULL);
					hudObjects[2]->setPowerUpFlag(false);
					hudObjects[9]->setSelectionUpgrades(NULL);
					hudObjects[9]->setUpgradeFlag(false);
					hudObjects[4]->setSelectionEnemy(NULL);
					hudObjects[4]->setEnemyFlag(false);
					selectedEnemy = NULL;

					cursor->setTex(textures["Cursor"][0]);
					hudObjects[5]->updateHotkeysTower();

					cursor->setTex(hudObjects[3]->getCursor());//sets the texture for the cursor with the tower icon <-----------
					selectionGraphic->setPosition(hudObjects[3]->getSelection()->getPosition());
					audio->playAgain("menuClick");
				}
				//=====================================================>>>>>>>>>>>>>>>>>DONT ERASE!!!!!!, THIS IS THE CODE TO BUY CREEPS WITH HOTKEYS

				std::cout << "ddfd" << std::endl;

				for (int i = 0; i < 10; i++) {
					hudObjects[i]->setTex(textures["UI"][turnIndex]);
					hudObjects[i]->setFactor(factor);
					hudObjects[i]->update(deltaTime);
				}
				hudObjects[0]->render(shaders);
				hudObjects[1]->render(shaders);
				hudObjects[2]->render(shaders);
				hudObjects[3]->render(shaders);
				hudObjects[4]->render(shaders);
				hudObjects[5]->detailRender(shaders);
				hudObjects[6]->newRender(shaders);
				hudObjects[7]->newRender(shaders);
				hudObjects[8]->buttonRender(shaders);
				hudObjects[9]->render(shaders);
				for (HUD* h : hudObjects) {

					for (Text* t : h->getTextObjects()) {
						t->setColor(hudColors[turnIndex]);

						if (t->getType().compare("FPS: ") == 0) {
							std::string temp = t->getText() + std::to_string(renderedFPS);
							t->setRenderedText(temp);
						}
						if (t->getType().compare("Player ") == 0) {
							std::string temp = t->getText() + std::to_string(turnIndex + 1);
							t->setRenderedText(temp);
						}

					}

				}

				std::cout << "eee" << std::endl;
				//**********Cursor**********
				float x = 0, y = 0;

				std::cout << "akkk" << std::endl;
				int id = g->getHover();

				g->getHoverCoords(x, y);

				std::cout << "tttt" << std::endl;
				cursor->setPosition(glm::vec3(x, y, 0.0f));


				if (g->getNode(id).getBuildable(turn)) {

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
				if (enemyMap[turnIndex]->size() == 0 && !roundOver && enemiesDestroyed == numEnemiesSpawned) {
					roundOver = true;
				}
				if (glfwGetTime() > lastSpawnTime + 0.2) { //spawn enemies gradually
					spawnCount += 1;
					lastSpawnTime = glfwGetTime();
					tick = true;
				}

				int count = 0;
				for (std::vector<EnemyObject*>::iterator it = enemyMap[turnIndex]->begin(); it != enemyMap[turnIndex]->end(); ++it) {
					if (!startWave) {
						if (tick) {
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

					if (!e->getExists()) { //free enemies after they die or reach end
						if (e->getHealth() <= 0.0) {
							enemiesDestroyed++;
						}
						deleteEnemies.push_front(delIndex);
					}

					delIndex++;



					if (cur->getId() != g->getEndPoints(turnIndex) && cur->getNextNode(e->getCurDestId()) != NULL) {

						Node* next = cur->getNextNode(e->getCurDestId());
						if (!next->getPathable()) { //if hit an obstruction
							g->setStart(cur->getId());
							g->setEnd(e->getCurDestId());
							//enemy trapped, die
							if (!g->pathfind()) {
								e->setExists(false);
								e->setKilled(true);
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
						//if we jumped over the node we want
						if ((oldEnemyY <= nexty && nexty <= enemyY) || (oldEnemyY >= nexty && nexty >= enemyY)) {

							glm::vec3 pos = e->getPosition();
							pos.y = nexty;
							e->setPosition(pos);
							if (abs(oldEnemyX - next->getX()) < 0.2 &&
								abs(oldEnemyY - next->getY()) < 0.2) {
								cur = next;
							}
							e->setCur(cur);
							e->setCurDestId(cur->getNextId());
							if (cur->getNextNode(e->getCurDestId()) != NULL) {

								next = cur->getNextNode(e->getCurDestId());
								//if next is obstructed
								if (!next->getPathable()) {

									g->setStart(cur->getId());
									g->setEnd(e->getCurDestId());
									//enemy trapped, die
									if (!g->pathfind()) {
										e->setExists(false);
										e->setKilled(true);
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
								e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
							}

						}
						if ((oldEnemyX <= nextx && nextx <= enemyX) || (oldEnemyX >= nextx && nextx >= enemyX)) {

							glm::vec3 pos = e->getPosition();
							pos.x = nextx;
							e->setPosition(pos);
							if (abs(oldEnemyX - next->getX()) < 0.2 &&
								abs(oldEnemyY - next->getY()) < 0.2) {
								cur = next;
							}
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
					if (e->getExists() && (cur->getId() == g->getEndPoints(turnIndex) || (cur->getNextNode(e->getCurDestId()) == NULL))) {
						hp[turnIndex] -= 1;
						stats[turnIndex]["Creeps Killed: "] -= 1;
						stats[turnIndex]["Damage Dealt: "] -= e->getHealthCap();

						if (e->getType().compare("Undying Zenith") == 0) {
							hp[turnIndex] = 0;
						}
						audio->playAgain("baseAttack");
						e->setExists(false);
						if (hp[turnIndex] <= 0) {
							gameOver = true;
							_state = GameOver;
							deleteEnemies.push_front(delIndex-1);
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
					stats[turnIndex]["Creeps Killed: "] += 1;
					stats[turnIndex]["Damage Dealt: "] += e->getHealthCap();
					if (e->getKilled()) {
						credits[turnIndex] += e->getCost();
					}
					enemyMap[turnIndex]->erase(enemyMap[turnIndex]->begin() + deleteEnemies[i]);
					delete(e);
				}

				//**********Graph**********
				g->update(showRadius, selectionRadius);
				//render graph
				g->render(shaders);

				break;
			}
			case Pause: {

				hours = gameTime / 3600;
				minutes = (gameTime - hours * 3600) / 60;

				seconds = gameTime - hours * 3600 - minutes * 60;
				std::string m = std::to_string(minutes);

				if (m.length() == 1) {
					m.insert(0, "0");
				}
				std::string s = std::to_string(seconds);

				if (s.length() == 1) {
					s.insert(0, "0");

				}


				for (int i = 0; i < gameOverHUD->getTextObjects().size(); i++) {
					Text* t = gameOverHUD->getTextObjects()[i];
					if (t->getType().compare("VICTOR:") == 0) {
						std::string temp = "";
						t->setRenderedText(temp);
					}
					if (t->getType().compare("Round:") == 0) {
						std::string temp = t->getText() + std::to_string(roundNum);
						t->setRenderedText(temp);
					}
					else if (t->getType().compare("Game Time:") == 0) {
						std::string temp = t->getText() + std::to_string(hours) + ":" + m + ":" + s;
						t->setRenderedText(temp);
					}
					else if (i > 11) {
						std::string temp = std::to_string(stats[t->getText()[0] - '0'][t->getText().substr(1)]);
						t->setRenderedText(temp);

					}

					t->render(shaders);
				}
				background->render(shaders);

				break;
			}
			case GameOver: {

				hours = gameTime / 3600;
				minutes = (gameTime - hours * 3600) / 60;

				seconds = gameTime - hours * 3600 - minutes * 60;
				std::string m = std::to_string(minutes);

				if (m.length() == 1) {
					m.insert(0, "0");
				}
				std::string s = std::to_string(seconds);

				if (s.length() == 1) {
					s.insert(0, "0");

				}


				for (int i = 0; i < gameOverHUD->getTextObjects().size(); i++) {
					Text* t = gameOverHUD->getTextObjects()[i];
					if (t->getType().compare("VICTOR:") == 0) {
						std::string temp = t->getText() + "PLAYER " + std::to_string((turnIndex+1)%2+1);
						t->setRenderedText(temp);
					}
					else if (t->getType().compare("Round:") == 0) {
						std::string temp = t->getText() + std::to_string(roundNum);
						t->setRenderedText(temp);
					}else if (t->getType().compare("Game Time:") == 0) {
						std::string temp = t->getText() + std::to_string(hours) + ":" + m + ":" + s;
						t->setRenderedText(temp);
					}
					else if (i > 11) {
						std::string temp = std::to_string(stats[t->getText()[0] - '0'][t->getText().substr(1)]);
						t->setRenderedText(temp);

					}

					t->render(shaders);
				}
				background->render(shaders);



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

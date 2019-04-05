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



// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl


// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Assault on Terra";
float ratio = 0.75f;
float window_width_g = 1000;//*1.5
float window_height_g = window_width_g * ratio;//*1.5
int Wwidth, Wheight;
float factor = window_width_g / window_width_g;//*1.5
//const glm::vec3 viewport_background_color_g(1, 1, 1);
const glm::vec3 viewport_background_color_g(0.15, 0.17, 0.21);

// Global texture info

std::map<std::string, std::vector<GLuint>  > textures;
std::map<char, GLuint > fontTexture;

// Global game object info

std::map<int, std::vector<EnemyObject*>*> enemyMap;


std::vector<TowerObject*> towerObjects;
std::vector<EnemyObject*> enemyObjects;
std::vector<HUD*> hudObjects;
std::vector<TowerObject*> blueprints;
std::vector<std::string> gameState;
std::vector<HUD*> HUDMenu;
std::vector<GameObject*>buttonTowerPanel;
std::vector<GameObject*>buttonEnemyPanel;
std::vector<GameObject*>buttonPowerUpsPanel;
std::vector<GameObject*>button;
std::vector<EnemyObject*> enemyBlueprint;
std::vector<Shader*> shaders;


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
	
	textures["Map"].push_back(createTexture("Graphics/Map/0_0_boundary.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/0_1_boundary.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/1_empty.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/2_checkpoint.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/3_health.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/6_spawn.png"));
	textures["Map"].push_back(createTexture("Graphics/Map/7_destructible.png"));

	textures["Enemy"].push_back(createTexture("Graphics/Enemy/0_enemy.png"));
	textures["Enemy"].push_back(createTexture("Graphics/Enemy/1_enemy.png"));

	textures["Particle"].push_back(createTexture("Graphics/Particles/fire.png"));
	textures["Particle"].push_back(createTexture("Graphics/Particles/enemyDeath.png"));

	textures["Tower"].push_back(createTexture("Graphics/Tower/01_tower.png"));//0
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_turret.png"));//1
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_projectile.png"));//2
	textures["Tower"].push_back(createTexture("Graphics/Tower/01_towerIcon.png"));//3
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_tower.png"));//4
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_turret.png"));//5
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_projectile.png"));//6
	textures["Tower"].push_back(createTexture("Graphics/Tower/02_towerIcon.png"));//7
	textures["Tower"].push_back(createTexture("Graphics/Tower/03_tower.png"));//8
	textures["Tower"].push_back(createTexture("Graphics/Tower/03_turret.png"));//9
	textures["Tower"].push_back(textures["Particle"][0]);//10
	textures["Tower"].push_back(createTexture("Graphics/Tower/03_towerIcon.png"));//11


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
																		

	//textures["MENU"].push_back(createTexture("Graphics/MENU/Play.png"));//0
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Quit.png"));//1
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Score.png"));//2
	//textures["MENU"].push_back(createTexture("Graphics/MENU/Option.png"));//3

	textures["Button"].push_back(createTexture("Graphics/Buttons/placeholder.png"));//0

	//Text is rendered by creating a map where each key is a character that corrasponds to an actual character.png file. 
	std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz=0123456789.,;:$#'!\"/?%&()@-";
	
	for (int i = 0; i < characters.size(); i++) {
		
		std::string s = "Graphics/Text/text_"+ std::to_string(i + 1)+".png";
	
		char *cstr = &s[0u];
		std::cout << (i+1) << " " << characters[i] << " " << s << std::endl;
		fontTexture[characters[i]] = createTexture(cstr);
	}

}

void setAudioTracks(Audio* audioObject) {

	audioObject->addAudio("Audio/Soundtrack/test.mp3", "background");
	audioObject->volume("background", 50);
	//audioObject->playRepeat("background");
	audioObject->addAudio("Audio/Soundtrack/mainMenu.mp3", "menu");
	audioObject->volume("menu", 100);
	audioObject->playRepeat("menu");

	audioObject->addAudio("Audio/Towers/rocket.mp3", "bullet");
	audioObject->volume("bullet", 100);
	audioObject->addAudio("Audio/Towers/place.mp3", "towerPlaced");
	audioObject->volume("towerPlaced", 30);

	audioObject->addAudio("Audio/HUD/menuClick.mp3", "menuClick");
	audioObject->volume("menuClick", 100);
	audioObject->addAudio("Audio/HUD/teamChange.mp3", "teamChange");
	audioObject->volume("teamChange", 200);

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

// Main function that builds and runs the game
int main(void){
	try {

		/************************************************AUDIO INIT************************************************/

		//PlaySound(TEXT("Audio/warlords.wav"), NULL, SND_FILENAME | SND_ASYNC); // remember you need 2 slashes these "\\"
		//system("pause");
		//PlaySound(L"Audio/warlords.wav", NULL, SND_ASYNC);
		//mciSendString(s2ws("play \"Audio/warlords.wav\" repeat"), NULL, 0, NULL);
		//mciSendString(s2ws("open Audio/warlords.wav alias MY_SND"), NULL, 0, NULL);
		//mciSendString(s2ws("play MY_SND"), NULL, 0, NULL);

		//mciSendString(L"open Audio/warlords.wav alias t", NULL, 0, 0);
		//mciSendString(L"open Audio/warlords.wav alias h", NULL, 0, 0);
		//mciSendString(L"play t", NULL, 0, 0);

		Audio* audioObject = new Audio();
		setAudioTracks(audioObject);
		


		
		
		
		
		
		



		std::cout << L"dd";
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

		GLuint cursorTex = textures["Cursor"][0];
		GLuint selectGraphicTex = textures["Cursor"][1];

		/************************************************VARIABLES INIT************************************************/
		char turnArr[2] = { 'B','T' };
		long income[2] = { 0,0 };
		long credits[2] = { 20,20 };
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

		bool gameOver = false;

		double lastTime = glfwGetTime();
		float timeOfLastMove = 0.0f;

		float maxCamZoom = 0.60f;
		float minCamZoom = 0.07f;
		float cameraZoom = 0.15f;
		
		float camShiftInc = 0.05f;
		float camZoomInc = 0.01f;


		float enemyX, enemyY, oldEnemyX, oldEnemyY;
		int pathCount = 1;
		float spawnCount = 0;
		bool startWave;
		bool toggleBlock = false;

		GameObject* background = new GameObject(glm::vec3(3.0f,-6.3f,0.0f), textures["Background"][0], size, "map");
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


		Node* cur;

		/************************************************blueprints INIT************************************************/

		//The blueprints are used to store a single type of tower, then once the player has placed one, it will use its variables
		//to create the actual tower object. 
		int index = 0;
		blueprints.push_back(new TowerObject(glm::vec3(-6.3f, 6.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 5,  "baseBlueprint---0",   3, 0.5,   5)); index += 4; //the final 5 here is the cost
		blueprints.push_back(new TowerObject(glm::vec3(-7.3f, 6.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 15, "denderBlueprint---1",  2, 1, 10));index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-8.3f, 6.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 4,  "denderBlueprint---2",  1, 0.1, 15));//index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.3f, 6.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10, "denderBlueprint",     10));//index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-6.3f, 7.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10, "baseBlueprint",       10));//index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-7.3f, 7.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10, "denderBlueprint",     10));//index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-8.3f, 7.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10, "denderBlueprint",     10));//index += 4;
		blueprints.push_back(new TowerObject(glm::vec3(-9.3f, 7.2f, 0.0f), std::vector<GLuint>(textures["Tower"].begin() + index, textures["Tower"].end() - 4 * (textures["Tower"].size() / 4 - 1) + index), textures["Explosion"], size, 10, "denderBlueprint",     10));
		/************************************************enemyBlueprints INIT************************************************/
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(6.2f, 6.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//0
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.2f, 6.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//1
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.2f, 6.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//2
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.2f, 6.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//3
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(6.2f, 7.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//4
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(7.2f, 7.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//5
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(8.2f, 7.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//6
		enemyBlueprint.push_back(new EnemyObject(glm::vec3(9.2f, 7.2f, 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5));//7
																																 /************************************************buttonBlueprints INIT************************************************/
		buttonTowerPanel.push_back(new GameObject(glm::vec3(-6.3f, 8.2f, 0.0f), textures["Button"][0], size, "Button1"));//panel1 0
		buttonTowerPanel.push_back(new GameObject(glm::vec3(-7.3f, 8.2f, 0.0f), textures["Button"][0], size, "Button2"));//panel1 1
		buttonTowerPanel.push_back(new GameObject(glm::vec3(-8.3f, 8.2f, 0.0f), textures["Button"][0], size, "Button3"));//panel1 2
		buttonTowerPanel.push_back(new GameObject(glm::vec3(-9.3f, 8.2f, 0.0f), textures["Button"][0], size, "Button4"));//panel1 3

		buttonEnemyPanel.push_back(new GameObject(glm::vec3(6.2f, 8.2f, 0.0f), textures["Button"][0], size, "Button5"));//panel2
		buttonEnemyPanel.push_back(new GameObject(glm::vec3(7.2f, 8.2f, 0.0f), textures["Button"][0], size, "Button6"));//panel2
		buttonEnemyPanel.push_back(new GameObject(glm::vec3(8.2f, 8.2f, 0.0f), textures["Button"][0], size, "Button7"));//panel2
		buttonEnemyPanel.push_back(new GameObject(glm::vec3(9.2f, 8.2f, 0.0f), textures["Button"][0], size, "Button8"));//panel2


		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(4.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//0
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(3.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//1
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(2.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//2
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(1.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//3
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(0.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//4
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(-1.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//5
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(-2.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//6
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(-3.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//7
		buttonPowerUpsPanel.push_back(new GameObject(glm::vec3(-4.0f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));//8



		button.push_back(new GameObject(glm::vec3(-7.5f, 4.0f, 0.0f), textures["Button"][0], size, "Button9"));
		/************************************************HUD INIT************************************************/
		GameObject* cursor = new GameObject(glm::vec3(0.0f), cursorTex, size, "cursor");
		HUD* selectionGraphic = new HUD(glm::vec3(50.0f, 50.0f, 0.0f), cameraZoom, glm::vec3(0.1f, 0.1f, 0.0f), selectGraphicTex, size, factor, "selection");


		glm::vec3 objectS = glm::vec3(0.45f, 0.5f, 0.0f);//this handels the size(scale) of the HUD panel 
		hudObjects.push_back(new HUD(glm::vec3(1.73f, 1.5f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD1"));//0(position,camerazoom,scale,texture,numelemnets,type) **if you change the scale of the object you need to reposition it by changin it position.
		hudObjects.push_back(new HUD(glm::vec3(-1.73f, 1.5f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD3"));//1



		hudObjects[1]->setBlueprints(blueprints);
		hudObjects[1]->setButtons(buttonTowerPanel);
		hudObjects[0]->setEnemyBlueprints(enemyBlueprint);
		hudObjects[0]->setButtons(buttonEnemyPanel);


		objectS.x = 1.12f;//this handels the size(scale) of the HUD panel
		hudObjects.push_back(new HUD(glm::vec3(0.0f, 1.5f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD2"));//2
		objectS.x = 0.45f;
		objectS.y = 0.2f;
		hudObjects.push_back(new HUD(glm::vec3(1.73f, 2.0f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD5"));//3
		hudObjects.push_back(new HUD(glm::vec3(-1.73f, 2.0f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD5"));//4//this is the start/end hud
		hudObjects[4]->setButtons(button);
		objectS.x = 1.25f;
		objectS.y = 0.2f;
		hudObjects.push_back(new HUD(glm::vec3(0.01f, 2.0f, 0.0f), cameraZoom, objectS, textures["UI"][0], size, factor, "HUD4"));//5
		hudObjects[5]->setPowerUPs(buttonPowerUpsPanel);

		for (HUD* h : hudObjects) {
			h->setCamPos(cameraTranslatePos);
		}
		selectionGraphic->setCamPos(cameraTranslatePos);

		/************************************************MENU INIT************************************************/
		glm::vec3 buttonScale = glm::vec3(0.5f, 0.5f, 0.0f);
		HUDMenu.push_back(new HUD(glm::vec3(0.0f, -1.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][0], size, factor, "PLAY"));
		HUDMenu.push_back(new HUD(glm::vec3(0.0f, -0.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][2], size, factor, "SCORE"));
		HUDMenu.push_back(new HUD(glm::vec3(0.0f, 0.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][3], size, factor, "OPTION"));
		HUDMenu.push_back(new HUD(glm::vec3(0.0f, 1.5f, 0.0f), cameraZoom, buttonScale, textures["MENU"][1], size, factor, "QUIT"));

		/************************************************TEXT INIT************************************************/

		//Parameters: Text(coordinates, fontTexture, text, size, scale, RGBcolor)
		hudObjects[2]->addText(new Text(glm::vec3(0.5f, 4.5f, 0.0f), fontTexture, "Player ", size, 0.2f,glm::vec3(0, 0, 153)));
		hudObjects[2]->addText(new Text(glm::vec3(-6.6f,-8.4f,0.0f), fontTexture, "Enemies Remaining: ", size, 0.07f, glm::vec3(50,175,255)));
		hudObjects[2]->addText(new Text(glm::vec3(-6.8f, -12.5f, 0.0f), fontTexture, "Credits: ", size, 0.07f, glm::vec3(50, 175, 255)));
		hudObjects[2]->addText(new Text(glm::vec3(-6.9f, -11.5f, 0.0f), fontTexture, "Income: ", size, 0.07f, glm::vec3(50, 175, 255)));
		hudObjects[2]->addText(new Text(glm::vec3(-6.9f, -10.5f, 0.0f), fontTexture, "P1 HP: ", size, 0.07f, glm::vec3(50, 175, 255)));
		hudObjects[2]->addText(new Text(glm::vec3(-6.9f, -9.5f, 0.0f), fontTexture, "P2 HP: ", size, 0.07f, glm::vec3(50, 175, 255)));

		/************************************************GAME LOOP************************************************/
		int fps = 0;
		int oldTime = 0;
		float lastSpawnTime = 0;
		bool roundOver = true;
		gameState.push_back("menu");
		//gameState.push_back("play"); // comment this line for menu
		glfwSetFramebufferSizeCallback(window, ResizeCallback);
		while (!glfwWindowShouldClose(window)) {

			//int Wwidth, Wheight;
			glfwGetWindowSize(window, &Wwidth, &Wheight);
			factor = (float)Wwidth / 800;

			++fps;
			if (int(glfwGetTime())>oldTime) {
				oldTime = int(glfwGetTime());
				std::cout << "FPS: "<<fps << std::endl;
				fps = 0;
			}
			/************************************************KEY INPUT********************************************/
			
			if (gameState.back() == "menu") {
				for (HUD* b : HUDMenu)b->setCamPos(cameraTranslatePos);

				if (timeOfLastMove + 0.05 < glfwGetTime()) {
					if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS &&
						(timeOfLastMove + 0.15 < glfwGetTime())) {
						gameState.push_back("play");
						
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

					std::cout << xOut << "," << yOut << std::endl;
				}
			}
			//==========================================>state:play/controls
			if (gameState.back() == "play") {

				if (timeOfLastMove + 0.05 < glfwGetTime()) {
					double xpos, ypos;
					int window_width_g, window_height_g;
					glfwGetWindowSize(window, &window_width_g, &window_height_g);
					glfwGetCursorPos(window, &xpos, &ypos);
					//std::cout << "x: " << xpos << " y: " << ypos << std::endl;
					//std::cout << "w: " << window_width_g << " h: " << window_height_g << std::endl;
					bool inWindow = ypos > 0 && xpos > 0 && ypos < window_height_g && xpos < window_width_g;
					if ( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || 
					   ((ypos > 0 && ypos<30) && inWindow)) {
						cameraTranslatePos.y -= camShiftInc/cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);

					}
					if ( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || 
					   ((ypos > window_height_g - 30 && ypos < window_height_g) && inWindow)) {
						cameraTranslatePos.y += camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
					}
					if ( glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || 
					   ((xpos > window_width_g-30 && xpos < window_width_g) && inWindow)) {
						cameraTranslatePos.x -= camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
					}
					if ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || 
					   ((xpos > 0 && xpos < 30) && inWindow)) {
						cameraTranslatePos.x += camShiftInc / cameraZoom;
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects)h->setCamPos(cameraTranslatePos);
						selectionGraphic->setCamPos(cameraTranslatePos);
					}
					if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
						cameraZoom = std::fmin(cameraZoom + camZoomInc, maxCamZoom);
						g.setZoom(cameraZoom);
						timeOfLastMove = glfwGetTime();
						for (HUD* h : hudObjects)h->setZoom(cameraZoom);
						selectionGraphic->setZoom(cameraZoom);
					}
					if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
						cameraZoom = std::fmax(cameraZoom - camZoomInc, minCamZoom);
						g.setZoom(cameraZoom);
						timeOfLastMove = glfwGetTime();
						for (HUD* h : hudObjects)h->setZoom(cameraZoom);
						selectionGraphic->setZoom(cameraZoom);
					}
					if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
						startWave = 1;
						audioObject->playAgain("enemiesComing");
						roundOver = false;
					}
					if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&
						enemyMap[turnIndex]->empty() && (timeOfLastMove + 0.15 < glfwGetTime())) {
						audioObject->playAgain("teamChange");
						pathCount = 1;
						spawnCount = 0;
						startWave = 0;
						turnIndex = turnIndex ^ 1;
						cameraTranslatePos = g.getFocalPoint(turnIndex);
						g.setCamPos(cameraTranslatePos);
						for (HUD* h : hudObjects) {
							h->setCamPos(cameraTranslatePos);
						}
						selectionGraphic->setCamPos(cameraTranslatePos);
						timeOfLastMove = glfwGetTime();
						std::cout << "T \n";
						turn = turnArr[turnIndex];
						credits[turnIndex] += income[turnIndex];
						g.clearNextNodeMaps();
						g.startPaths();


					}
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
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
					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

						double xpos, ypos;
						glfwGetCursorPos(window, &xpos, &ypos);

						float x;
						float y;
						int id = g.getHover();
						g.getHoverCoords(x, y);


						if (ypos <= 440) {//prints on map

							if (hudObjects[1]->getFlag()) {
								


								if (g.getNode(id).getBuildable(turn)) {
									TowerObject* selectedTower = hudObjects[1]->getSelection();
									std::cout << "Tower costs: " << selectedTower->getCost() << std::endl;
									
									if (credits[turnIndex] >= selectedTower->getCost()) {
										audioObject->playAgain("towerPlaced");
										audioObject->playAgain("unitReady");
										TowerObject* t = new TowerObject(glm::vec3(x, y, 0.0f), selectedTower->getTexvec(), 
																		 selectedTower->getExplosion_tex(), size, selectedTower->getDamage(),
											                             selectedTower->getType(), selectedTower->getRange(), selectedTower->getROF(),
																		 selectedTower->getCost());
										t->setAudio(audioObject);
										if (t->getType().compare("denderBlueprint---2") == 0) {

										}
										g.getNode(id).setTowerState(true, turn);
										if (g.rePath(enemyMap[turnIndex], id, ++pathCount, turn)) {
											towerObjects.push_back(t);
											g.getNode(id).setTower(t);
											credits[turnIndex] -= selectedTower->getCost();
										}
										else {
											std::cout << "\n\n\nINVALID TOWER PLACEMENT";
											g.getNode(id).setTowerState(false, turn);
											g.rePath(enemyMap[turnIndex], id, --pathCount, turn);
											delete(t);
										}
										std::cout << "Repath" << std::endl;
									}
								}
							}
						}
						hudObjects[1]->selection(xpos, ypos);

						if (hudObjects[1]->getFlag()) {
							audioObject->playAgain("menuClick");
							cursor->setTex(hudObjects[1]->getCursor());//sets the texture for the cursor with the tower icon <-----------
							selectionGraphic->setPosition(hudObjects[1]->getSelection()->getPosition());
						}



						//--------end of click button------------ 
					}

					if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
						cursor->setTex(textures["Cursor"][0]);
						hudObjects[1]->setFlag(false);
					}


					if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS &&
						(timeOfLastMove + 0.15 < glfwGetTime())) {

						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////
						//code for getting from hud here
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////
						/////////////////////////////////

						if (credits[turnIndex] >= 5) { //>=selectedEnemy->getCost()
							std::cout << "Spawned new enemy. Total: " << enemyMap[turnIndex ^ 1]->size() + 1 << std::endl;
							income[turnIndex] += 5;
							credits[turnIndex] -= 5;
							Node* cur;
							for (int s : g.getStartSet(turnArr[turnIndex ^ 1])) {
								cur = &g.getNode(s);
								EnemyObject* e = new EnemyObject(glm::vec3(cur->getX(), cur->getY(), 0.0f), textures["Enemy"][0], size, enemyHealth, "enemy", textures["Particle"][1], 1, 5);
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

				}
			}

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

			//Here it is just looping over each shader and setting the viewmatrix. 
			for (Shader* s : shaders) {
				s->enable();
				s->setUniformMat4("viewMatrix", viewMatrix);
				s->disable();
			}
			shaders[0]->enable();
			
		
			/************************************************OBJECT UPDATE/RENDERING********************************************/
			
			if (gameState.back() == "menu") {
				for (HUD* b : HUDMenu) {
					b->render(shaders);
				}
				background->render(shaders);
			}
			//==========================================>state:play/render
			if (gameState.back() == "play") {
				//shader =  Shader("shader.vert", "shader.frag");
			
			//**********HUD**********
				selectionGraphic->render(shaders);
				for (HUD* h : hudObjects) {
					h->setTex(textures["UI"][turnIndex]);
					h->setFactor(factor);
					h->update(deltaTime);

					for (Text* t : h->getTextObjects()) {
						t->setColor(hudColors[turnIndex]);

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
				for (TowerObject* t : towerObjects) {

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


				//**********Enemy**********
				std::deque<int> deleteEnemies;
				int delIndex = 0;
				bool tick = false;
				//spawnCount+=0.25;
				
				if (enemyMap[turnIndex]->size() == 0 && !roundOver) {
					roundOver = true;
					audioObject->playAgain("enemiesDestroyed");
				}
				if (glfwGetTime() > lastSpawnTime + 0.075) {
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
					cur->setIsCur(true);

					if (!e->getExists()) {
						std::cout << "Enemy despawned/destroyed. Total: " << enemyMap[turnIndex]->size() - 1 << std::endl;
						deleteEnemies.push_front(delIndex);
					}

					delIndex++;



					if (cur->getId() != g.getEndPoints(turnIndex) && cur->getNextNode(e->getCurDestId()) != NULL) {

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
							cur->setIsCur(false);
							if (abs(oldEnemyX - next->getX()) < 0.2 &&
								abs(oldEnemyY - next->getY()) < 0.2) {
								cur = next;
							}
							cur->setIsCur(true);
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
								std::cout << "here";
								e->setDirection(glm::vec3(0.0f, 0.0f, 0.0f));
							}

						}
						if ((oldEnemyX <= nextx && nextx <= enemyX) || (oldEnemyX >= nextx && nextx >= enemyX)) {

							glm::vec3 pos = e->getPosition();
							pos.x = nextx;
							e->setPosition(pos);
							cur->setIsCur(false);
							if (abs(oldEnemyX - next->getX()) < 0.2 &&
								abs(oldEnemyY - next->getY()) < 0.2) {
								cur = next;
							}
							cur->setIsCur(true);
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
					if (e->getExists() && (cur->getId() == g.getEndPoints(turnIndex) || cur->getNextNode(e->getCurDestId()) == NULL)) {
						hp[turnIndex] -= 1;
						std::cout << "hp = " << hp[turnIndex] << std::endl;
						audioObject->playAgain("baseAttack");
						e->setExists(false);
						if (hp[turnIndex] == 0) {
							gameOver = true;
							std::cout << "GAME OVER" << std::endl;
							std::cout << "PLAYER " << (turnIndex^1+1) << " WINS" << std::endl;
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

				//using the indecies, delete the enemies that should be deleted. 
				for (int i = 0; i < deleteEnemies.size(); i++) {
					if (enemyMap[turnIndex]->at(deleteEnemies[i])->getKilled()) {
						credits[turnIndex] += enemyMap[turnIndex]->at(deleteEnemies[i])->getCost();
					}
					enemyMap[turnIndex]->erase(enemyMap[turnIndex]->begin() + deleteEnemies[i]);
				}

				//**********Graph**********
				g.update(NULL);
				//render graph
				g.render(shaders);

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


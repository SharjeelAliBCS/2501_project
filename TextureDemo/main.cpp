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

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Transform Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 800;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);

// Global texture info
GLuint tex[6];

// Global game object info
std::vector<GameObject*> gameObjects;
std::vector<GameObject*> backGround;

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
		0, 1, 2, // t1
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
	glGenTextures(6, tex);
	setthisTexture(tex[0], "0_boundary.png");
	setthisTexture(tex[1], "1_empty.png");
	setthisTexture(tex[2], "3_1hp.png");
	setthisTexture(tex[3], "7_destructible.png");
	setthisTexture(tex[4], "6_spawn.png");
	setthisTexture(tex[5], "orb.png");

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

		// Set up the background (position, scale, rotation, texture, vertex count)
		backGround.push_back(new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[0], size));
		backGround.push_back(new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[1], size));
		backGround.push_back(new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[2], size));
		backGround.push_back(new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[3], size));
		backGround.push_back(new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[4], size));
		// Setup the player object (position, texture, vertex count)
		
		gameObjects.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex[5], size));
		

		// Setup other objects
		//gameObjects.push_back(new GameObject(glm::vec3(-5.0f, 5.0f, 0.0f), tex[2], size));
		//gameObjects.push_back(new GameObject(glm::vec3(1.0f, -0.5f, 0.0f), tex[2], size));


		// Run the main loop
		double lastTime = glfwGetTime();
	
		float cameraZoom = 0.04f;
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(viewport_background_color_g);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// Select proper shader program to use
			shader.enable();

			// Setup camera to focus on the player object (the first object in the gameObjects array)
			glm::vec3 cameraTranslatePos(-gameObjects[0]->getPosition());
			//glm::vec3 cameraTranslatePos(-20,-20,0);

			if (glfwGetKey(Window::getWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
				// This is where you should change the velocity vector of the player
				cameraZoom += 0.001;
			}

			if (glfwGetKey(Window::getWindow(), GLFW_KEY_X) == GLFW_PRESS) {
				// This is where you should change the velocity vector of the player
				cameraZoom -= 0.001;
			}
			
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);


			// Update and render all game objects
			for (int i = 0; i < gameObjects.size(); i++) {
				// Get the current object
				GameObject* currentGameObject = gameObjects[i];

				// Updates game objects
				currentGameObject->update(deltaTime);

				// Check for collision between game objects
				for (int j = i + 1; j < gameObjects.size(); j++) {
					GameObject* otherGameObject = gameObjects[j];

					float distance = glm::length(currentGameObject->getPosition() - otherGameObject->getPosition());
					if (distance < 0.1f) {

						// This is where you would perform collision response between objects
					}
				}

				// Render game objects
				currentGameObject->render(shader);
			}

			//------------------------------------------------------------------
			using namespace std;

			ifstream in("map.csv");

			string line, field;

			vector< vector<string> > array;  // the 2D array
			vector<string> v;                // array of values for one line only
			float j = -20.0;

			while (getline(in, line))    // get next line in file
			{

				v.clear();
				stringstream ss(line);
				//for(int i =0;i<s)
				//cout << line+"\n";
				float i = -20.0;
				while (getline(ss, field, ','))  // break line into comma delimitted fields
				{
					//cout << field+"\n";
					if (field == "0") {
						glm::vec3 out = glm::vec3(i, j, 0.0f);
						backGround[0]->setPosition(out);
						backGround[0]->render(shader);
					}
					else if (field == "1") {
						glm::vec3 out = glm::vec3(i, j, 0.0f);
						backGround[1]->setPosition(out);
						backGround[1]->render(shader);
					}
					else if (field == "3") {
						glm::vec3 out = glm::vec3(i, j, 0.0f);
						backGround[2]->setPosition(out);
						backGround[2]->render(shader);
					}
					else if (field == "4") {
						glm::vec3 out = glm::vec3(i, j, 0.0f);
						backGround[3]->setPosition(out);
						backGround[3]->render(shader);
					}
					else if (field == "6") {
						glm::vec3 out = glm::vec3(i, j, 0.0f);
						backGround[4]->setPosition(out);
						backGround[4]->render(shader);
					}
					i++;
				}
				j++;
			}
			//------------------------------------------------------------------

			/* Draw background
			for (int i = 0; i < backGround.size(); i++) {
				GameObject* currentBackGround = backGround[i];
			
				currentBackGround->render(shader);
				//backGround[1]->render(shader);
			}
			*/
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

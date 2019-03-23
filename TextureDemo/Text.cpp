#include "Text.h"

#include "Window.h"

/*
PlayerGameObject inherits from GameObject
It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

Text::Text(glm::vec3 &entityPos, std::map<char, GLuint> tex, std::string t, GLint entityNumElements, float s, std::string type)
	: GameObject(entityPos, tex['G'], entityNumElements, type) {

	text = t;
	characters = tex;
	scale = s;
	renderText = t;
	

}

// Update function
void Text::update(double deltaTime) {

}




void Text::render(Shader &shader) {
	
	glm::vec3 textPos = position;
	for (int i = 0; i < renderText.size();i++) {
		char c = renderText[i];
		//std::cout << c << std::endl;
		
		GLuint tex = characters[c];
		
		glBindTexture(GL_TEXTURE_2D, tex);

		// Setup the transformation matrix for the shader
		glm::vec3 testCam = glm::vec3(-camPos.x, -camPos.y, -camPos.z);
		//std::cout<< testCam.x<<std::endl;
		//glm::mat4 translationMatrix = glm::translate(glm::translate(glm::mat4(1.0f), testCam), position);
		glm::mat4 camMat = glm::translate(glm::mat4(1.0f), testCam);
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), textPos);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale,scale,1.0f));

		glm::mat4 oMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-textPos.x, -textPos.y, -textPos.z));
		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1 / zoom, 1 / zoom, 1 / zoom));

		glm::mat4 transformationMatrix = camMat * zoomMatrix *scaleMatrix* posMat;
		//glm::mat4 transformationMatrix = posMat  * scaleMatrix;
		shader.setUniformMat4("transformationMatrix", transformationMatrix);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		if (i < text.size() - 1 && (text[i+1]==' ' || text[i]==' ') ) {
			textPos.x += 0.4f;
		}
		else {


			textPos.x += 0.6f;
		}
	}
}



#pragma once

// GL Includes
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace std;


// Light class can be used with set of shaders
class Light
{
	glm::vec3 position;
	glm::vec3 color;
	GLfloat intensity;
	GLfloat ambient;

public:
	// Constructor
	Light(glm::vec3 position = glm::vec3(0.0, 10.0, 0.0), GLfloat intensity = 1.0, GLfloat ambient = 0.5,
		glm::vec3 color = glm::vec3(1.0, 1.0, 1.0)) {
		this->position = position;
		this->intensity = intensity;
		this->ambient = ambient;
		this->color = color;
	}

	// Applies light to set of shader programs
	void use(vector<ShaderProgram> shaders) const {
		for (int i = 0; i < shaders.size(); ++i) {
			GLint lightLoc = glGetUniformLocation(shaders[i].get_programID(), "lightColor");
			glUniform3fv(lightLoc, 1, glm::value_ptr(this->color));

			GLint lightPosLoc = glGetUniformLocation(shaders[i].get_programID(), "lightPosition");
			glUniform3fv(lightPosLoc, 1, glm::value_ptr(this->position));

			GLint lightStrengthLoc = glGetUniformLocation(shaders[i].get_programID(), "lightIntensity");
			glUniform1f(lightStrengthLoc, this->intensity);

			GLint lightAmbientLoc = glGetUniformLocation(shaders[i].get_programID(), "ambientIntensity");
			glUniform1f(lightAmbientLoc, this->ambient);
		}
	}
	
	// Sets intensity of diffuse light
	void changeIntensity(GLfloat delta) {
		this->intensity += delta;
		if (this->intensity < 0.0)
			this->intensity = 0.0;
	}

	// Sets intensity of diffuse light
	void changeAmbient(GLfloat delta) {
		this->ambient += delta;
		if (this->ambient < 0.0)
			this->ambient = 0.0;
	}

	// Sets new position of light
	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
	}

	// Get light position
	glm::vec3 getPosition() {
		return this->position;
	}
};
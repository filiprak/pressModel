#pragma once
#include "model.h"

class Barrel : public Model {

	// Width - x size, height - y size. depth - z size
	GLfloat radius, height;
	GLuint sides;


public:
	// Constructors
	Barrel();

	Barrel(GLfloat radius, GLfloat height, GLuint sides,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 texScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

private:
	// Override
	void prepareMeshes(glm::vec3 texScale);
};


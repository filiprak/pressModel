#pragma once
#include "model.h"

class Barrel : public Model {

	// Width - x size, height - y size. depth - z size
	GLfloat radius, height;
	GLuint sides;

	glm::vec3 texScale;

public:
	bool deformed = false;
	// Constructors
	Barrel();

	Barrel(GLfloat radius, GLfloat height, GLuint sides,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 texScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

	GLfloat getRadius() {
		return this->radius;
	}

	void changeSize(GLfloat radius, GLfloat height) {
		this->height = height;
		this->radius = radius;
		this->clear();
		this->prepareMeshes(this->texScale);
	}

private:
	// Override
	void prepareMeshes(glm::vec3 texScale);
};


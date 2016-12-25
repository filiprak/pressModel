#pragma once
#include "model.h"


class Cuboid : public Model {

	// Width - x size, height - y size. depth - z size
	GLfloat width, height, depth;


public:
	// Constructors
	Cuboid();

	Cuboid(GLfloat width, GLfloat height, GLfloat depth,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

private:
	// Override
	void prepareMeshes();
};


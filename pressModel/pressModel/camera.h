#pragma once

// GL Includes
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "shader_prog.h"

using namespace std;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraDir {
	VERTICAL,
	HORIZONTAL
};


// Camera class: calculates view matrix based on current position and angles
class Camera {

	// Camera movement sphere radious
	GLfloat radius;

	// Camera transformation matrices
	glm::mat4 viewMat, vrotationMat, hrotationMat, positionMat;

	// View angles
	GLfloat vrads = 0.0f;
	GLfloat hrads = 0.0f;

	// True if view matrix needs to be recalculated
	bool update = true;

public:
	// Camera position constraints
	GLfloat minRadius = 10.0f, maxRadius = 100.0f;

	// Camera movement speeds
	GLfloat zoomSpeed = 0.9f, rotateSpeed = 0.2f;

	// Constructor
	Camera(GLfloat radius, GLfloat vrads, GLfloat hrads);

	// Returns the view matrix
	glm::mat4 getViewMatrix();

	// Sets uniform variables for every shader in vector
	void use(vector<ShaderProgram> shaders, GLuint width, GLuint height, GLfloat fovy = 45.0f,
		GLfloat znear = 0.1, GLfloat zfar = 100.0);

	// Returns position vector
	glm::vec3 getPosition();

	// Changes camera position vector
	void move(GLfloat distance);

	// Changes camera vertical or horizontal angles
	void rotate(CameraDir direction, GLfloat angle);
};
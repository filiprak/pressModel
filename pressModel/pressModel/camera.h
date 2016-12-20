#pragma once

// GL Includes
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "shader_prog.h"

using namespace std;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraDir {
	ZOOM,
	VERTICAL,
	HORIZONTAL
};


// Camera class: calculates view matrix based on current position and angles
class Camera {

	// Camera position
	glm::vec3 position;

	// Camera transformation matrices
	glm::mat4 viewMat, vrotationMat, hrotationMat, positionMat;

	// View angles
	GLfloat vrads = 0.0f;
	GLfloat hrads = 0.0f;

	// True if view matrix needs to be recalculated
	bool update = true;

public:
	// Camera movement speeds
	GLfloat zoomSpeed = 0.5f, rotateSpeed = 0.2f, moveSpeed = 0.01f;

	// Constructor
	Camera(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat vrads, GLfloat hrads);

	// Returns the view matrix
	glm::mat4 getViewMatrix();

	// Sets uniform variables for every shader in vector
	void use(vector<ShaderProgram> shaders, GLuint width, GLuint height, GLfloat fovy = 45.0f,
		GLfloat znear = 0.1, GLfloat zfar = 100.0);

	// Returns position vector
	glm::vec3 getPosition();

	// Changes camera position vector
	void move(CameraDir direction, GLfloat distance);

	// Changes camera vertical or horizontal angles
	void rotate(CameraDir direction, GLfloat angle);
};
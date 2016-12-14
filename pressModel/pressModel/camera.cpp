#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

Camera::Camera(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat vrads, GLfloat hrads) {
	this->position = glm::vec3(xpos, ypos, zpos);
	this->vrads = vrads;
	this->hrads = hrads;

	this->positionMat = glm::translate(this->positionMat, this->position);
	this->vrotationMat = glm::rotate(this->vrotationMat, -glm::radians(this->vrads), glm::vec3(0.0f, 1.0f, 0.0f));
	this->hrotationMat = glm::rotate(this->hrotationMat, -glm::radians(this->hrads), glm::vec3(1.0f, 0.0f, 0.0f));
	this->viewMat = getViewMatrix();
	this->update = false;
}

void Camera::move(CameraDir direction, GLfloat distance)
{
	glm::vec3 deltaPos = glm::vec3(0.0, 0.0, 0.0);

	if (direction == ZOOM)
		deltaPos.z += distance * (this->zoomSpeed);
	else if (direction == VERTICAL)
		deltaPos.x -= distance * (this->moveSpeed);
	else if (direction == HORIZONTAL)
		deltaPos.y -= distance * (this->moveSpeed);
	else return;

	this->position += deltaPos;
	this->positionMat = glm::translate(glm::mat4(), position);
	
	this->update = true;
}

void Camera::rotate(CameraDir direction, GLfloat angle) {
	if (direction == VERTICAL) {
		this->vrads += angle * (this->rotateSpeed);
		this->vrotationMat = glm::rotate(this->vrotationMat, glm::radians(angle * (this->rotateSpeed)), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (direction == HORIZONTAL) {
		this->hrads += angle * (this->rotateSpeed);
		this->hrotationMat = glm::rotate(this->hrotationMat, -glm::radians(angle * (this->rotateSpeed)), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else return;

	this->update = true;
}

glm::mat4 Camera::getViewMatrix() {
	if (update) {
		this->viewMat = (this->positionMat) * (this->vrotationMat) * (this->hrotationMat);
	}
	this->update = false;
	return (this->viewMat);
}
#include "camera.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera(GLfloat radius, GLfloat vrads, GLfloat hrads) {
	this->radius = glm::abs(radius);
	if (this->radius < this->minRadius)
		this->radius = this->minRadius;
	if (this->radius > this->maxRadius)
		this->radius = this->maxRadius;
	this->vrads = vrads;
	this->hrads = hrads;

	this->positionMat = glm::translate(this->positionMat, glm::vec3(0.0, 0.0, -this->radius));
	this->vrotationMat = glm::rotate(this->vrotationMat, -glm::radians(this->vrads), glm::vec3(0.0f, 1.0f, 0.0f));
	this->hrotationMat = glm::rotate(this->hrotationMat, -glm::radians(this->hrads), glm::vec3(1.0f, 0.0f, 0.0f));
	this->viewMat = getViewMatrix();
	this->update = false;
}

void Camera::move(GLfloat distance) {
	GLfloat delta = distance * (this->zoomSpeed);
	this->radius -= delta;
	if (this->radius < this->minRadius)
		this->radius = this->minRadius;
	if (this->radius > this->maxRadius)
		this->radius = this->maxRadius;

	this->positionMat = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, -this->radius));

	this->update = true;
}

void Camera::rotate(CameraDir direction, GLfloat angle) {
	if (direction == VERTICAL) {
		this->vrads -= angle * (this->rotateSpeed);
		this->vrotationMat = glm::rotate(this->vrotationMat, glm::radians(angle * (this->rotateSpeed)), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (direction == HORIZONTAL) {
		this->hrads -= angle * (this->rotateSpeed);
		this->hrotationMat = glm::rotate(this->hrotationMat, -glm::radians(angle * (this->rotateSpeed)), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else return;

	this->update = true;

}

glm::mat4 Camera::getViewMatrix() {
	if (update) {
		this->viewMat = (this->positionMat) * (this->hrotationMat) * (this->vrotationMat);
	}
	this->update = false;
	return (this->viewMat);
}

glm::vec3 Camera::getPosition() {
	GLfloat x, y, z;
	x = this->radius * glm::cos(glm::radians(this->hrads)) * glm::sin(glm::radians(this->vrads));
	y = this->radius * glm::sin(glm::radians(this->hrads));
	z = this->radius * glm::cos(glm::radians(this->hrads)) * glm::cos(glm::radians(this->vrads));
	return glm::vec3(x, y, z);
}

void Camera::use(vector<ShaderProgram> shaders, GLuint width, GLuint height, GLfloat fovy, GLfloat znear, GLfloat zfar) {
	for (int i = 0; i < shaders.size(); ++i) {
		GLint viewLoc = glGetUniformLocation(shaders[i].get_programID(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->getViewMatrix()));

		glm::mat4 projection = glm::perspective(fovy, (GLfloat) width / height, znear, zfar);
		GLint projLoc = glGetUniformLocation(shaders[i].get_programID(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLint camPos = glGetUniformLocation(shaders[i].get_programID(), "camPosition");
		glUniform3fv(camPos, 1, glm::value_ptr(this->getPosition()));
	}
}
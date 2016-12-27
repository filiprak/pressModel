#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "mesh.h"
#include "shader_prog.h"


// Abstract class of model as a set of meshes
class Model {

	// Model consists of meshes
	vector<Mesh> meshes;

	// Position, rotation and scale relational to world space
	glm::vec3 positionVec, dynamicPos;
	glm::vec3 rotationVec;
	glm::vec3 scaleVec;

	// Model matrix calculated basing on position, rotation and scale
	glm::mat4 modelMatrix;
	glm::mat4 transformMatrix;

public:
	// Constructors
	Model() {
		this->positionVec = glm::vec3(0.0f, 0.0f, 0.0f);
		this->dynamicPos = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationVec = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scaleVec = glm::vec3(1.0f, 1.0f, 1.0f);

		this->modelMatrix = glm::mat4();
		this->transformMatrix = glm::mat4();
	}

	Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->positionVec = position;
		this->dynamicPos = position;
		this->rotationVec = rotation;
		this->scaleVec = scale;

		this->modelMatrix = glm::translate(this->modelMatrix, position);
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->modelMatrix = glm::scale(this->modelMatrix, scale);

		this->transformMatrix = glm::mat4();
	}

	// Draws whole model by using specified shader to render all meshes
	void draw(ShaderProgram shader) {
		shader.Use();
		GLint modelLoc = glGetUniformLocation(shader.get_programID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));

		GLuint transformLoc = glGetUniformLocation(shader.get_programID(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->transformMatrix));

		GLuint normalModLoc = glGetUniformLocation(shader.get_programID(), "normalModel");
		glUniformMatrix4fv(normalModLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(inverse(this->modelMatrix))));
		
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].draw(shader);
	}

	// Translates object in space
	void move(glm::vec3 movement) {
		this->transformMatrix = glm::translate(this->transformMatrix, movement);
		this->dynamicPos += movement;
	}

	// Rotates object around x, y, z axis an angle
	void rotate(glm::vec3 rotation) {
		this->transformMatrix = glm::rotate(this->transformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		this->transformMatrix = glm::rotate(this->transformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->transformMatrix = glm::rotate(this->transformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	// Scales object along x, y, z axis
	void scale(glm::vec3 scale) {
		this->transformMatrix = glm::scale(this->transformMatrix, scale);
	}

	// Abstract method, have to be overriden to define meshes of the model
	virtual void prepareMeshes(glm::vec3 texScale) = 0;

	// Add mesh to mesh vector
	void addMesh(Mesh mesh) {
		meshes.push_back(mesh);
	}

	// Clear model meshes
	void clear() {
		meshes.clear();
	}

	// Adds texture to all meshes. Texture may be used later in shaders.
	void addTexture(const Texture texture) {
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].addTexture(texture);
	}

	// Sets color of every mesh that belongs to this model
	void setColor(glm::vec3 color) {
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].setColor(color);
	}

	// Returns matrix defining model position in world space
	glm::mat4 getModelMatrix() {
		return this->modelMatrix;
	}

	// Returns actual position of model in worldspace
	glm::vec3 getPosition() {
		return this->dynamicPos;
	}
};
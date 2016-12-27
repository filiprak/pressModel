#include "barrel.h"
#include <iostream>
using namespace std;

Barrel::Barrel() {

}

Barrel::Barrel(GLfloat radius, GLfloat height, GLuint sides,
	glm::vec3 position, glm::vec3 texScale, glm::vec3 rotation, glm::vec3 scale)
	: Model(position, rotation, scale) {
	this->radius = radius;
	this->height = height;
	this->sides = sides;
	this->texScale = texScale;

	this->prepareMeshes(texScale);
}

void Barrel::prepareMeshes(glm::vec3 texScale) {
	vector<glm::vec3> upVertices;
	vector<glm::vec3> downVertices;

	GLfloat y = this->height / 2;

	if (sides < 3)
		throw exception("Barrel must have more than 2 side walls");

	GLfloat angle = glm::radians(360.0 / sides);
	GLfloat angleSum = 0.0;

	for (int i = 0; i < this->sides; ++i) {
		GLfloat x = this->radius * glm::cos(angleSum);
		GLfloat z = this->radius * glm::sin(angleSum);
		glm::vec3 vertex1(x, y, z);
		upVertices.push_back(vertex1);
		glm::vec3 vertex2(x, -y, z);
		downVertices.push_back(vertex2);
		angleSum += angle;
	}

	Mesh sideSurface = Mesh();
	GLfloat tx = 0.0;
	GLfloat sideLenght = this->radius * glm::sin(angle / 2);
	GLuint n = 2 * this->sides + 2;
	for (int i = 0; i < this->sides + 1; ++i) {
		glm::vec3 norm = upVertices[i % sides];
		norm.y = 0;
		norm = glm::normalize(norm);
		sideSurface.addVertex(upVertices[i % sides], norm, tx * texScale.x, this->height * texScale.y);
		sideSurface.addVertex(downVertices[i % sides], norm, tx * texScale.x, 0.0);
		tx += sideLenght;
		sideSurface.addIndex((2 * i) % n);
		sideSurface.addIndex((2 * i + 1) % n);
		sideSurface.addIndex((2 * i + 2) % n);

		sideSurface.addIndex((2 * i + 1) % n);
		sideSurface.addIndex((2 * i + 2) % n);
		sideSurface.addIndex((2 * i + 3) % n);
	}
	sideSurface.initMesh();
	this->addMesh(sideSurface);

	Mesh topSurface = Mesh();
	Mesh bottomSurface = Mesh();
	angleSum = 0.0;

	glm::vec3 norm(0, 1, 0);
	glm::vec3 upMiddle(0, y, 0);
	topSurface.addVertex(upMiddle, norm, this->radius * texScale.z, this->radius * texScale.z);
	bottomSurface.addVertex(-upMiddle, -norm, this->radius * texScale.z, this->radius * texScale.z);
	for (int i = 0; i < this->sides + 1; ++i) {
		GLfloat tx = this->radius * (glm::cos(angleSum) + 1.0) * texScale.z;
		GLfloat ty = this->radius * (glm::sin(angleSum) + 1.0) * texScale.z;
		topSurface.addVertex(upVertices[i % sides], norm, tx, ty);
		bottomSurface.addVertex(downVertices[i % sides], -norm, tx, ty);
		angleSum += angle;
		topSurface.addIndex(i % sides + 1);
		topSurface.addIndex((i + 1) % sides + 1);
		topSurface.addIndex(0);

		bottomSurface.addIndex(i % sides + 1);
		bottomSurface.addIndex((i + 1) % sides + 1);
		bottomSurface.addIndex(0);
	}

	topSurface.initMesh();
	this->addMesh(topSurface);

	bottomSurface.initMesh();
	this->addMesh(bottomSurface);
}
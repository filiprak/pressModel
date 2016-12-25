#include "cuboid.h"

Cuboid::Cuboid() {

}

Cuboid::Cuboid(GLfloat width, GLfloat height, GLfloat depth,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Model(position, rotation, scale) {
	this->width = width;
	this->height = height;
	this->depth = depth;

	this->prepareMeshes();
}

void Cuboid::prepareMeshes() {
	GLfloat x = this->width / 2;
	GLfloat y = this->height / 2;
	GLfloat z = this->depth / 2;

	struct WallHelper {
		GLint i1, i2, i3, i4;
		GLfloat nx, ny, nz;
		GLfloat texCor[4][2];
	};

	struct VertexHelper {
		GLfloat xpos, ypos, zpos;
	};

	VertexHelper verHelper[8] = {
		{ x, -y, z },
		{ -x, -y, z },
		{ -x, y, z },
		{ x, y, z },
		{ x, -y, -z },
		{ -x, -y, -z },
		{ -x, y, -z },
		{ x, y, -z },
	};
	WallHelper wallHelper[6] = {
		{ 0, 1, 2, 3, 0, 0, 1, { { this->width, 0 }, { 0, 0 }, { 0, this->height }, { this->width, this->height } } }, //front
		{ 5, 4, 7, 6, 0, 0, -1, { { this->width, 0 }, { 0, 0 }, { 0, this->height }, { this->width, this->height } } }, //back
		{ 1, 5, 6, 2, -1, 0, 0, { { this->depth, 0 }, { 0, 0 }, { 0, this->height }, { this->depth, this->height } } }, //left
		{ 4, 0, 3, 7, 1, 0, 0, { { this->depth, 0 }, { 0, 0 }, { 0, this->height }, { this->depth, this->height } } }, //right
		{ 7, 3, 2, 6, 0, 1, 0, { { this->depth, 0 }, { 0, 0 }, { 0, this->width }, { this->depth, this->width } } }, //up
		{ 4, 0, 1, 5, 0, -1, 0, { { this->depth, 0 }, { 0, 0 }, { 0, this->width }, { this->depth, this->width } } }, //down
	};

	for (int side = 0; side < 6; side++) {
		Mesh wall = Mesh();
		wall.addVertex(verHelper[wallHelper[side].i1].xpos,
			verHelper[wallHelper[side].i1].ypos,
			verHelper[wallHelper[side].i1].zpos,
			wallHelper[side].nx, wallHelper[side].ny, wallHelper[side].nz, wallHelper[side].texCor[0][0], wallHelper[side].texCor[0][1]);
		wall.addVertex(verHelper[wallHelper[side].i2].xpos,
			verHelper[wallHelper[side].i2].ypos,
			verHelper[wallHelper[side].i2].zpos,
			wallHelper[side].nx, wallHelper[side].ny, wallHelper[side].nz, wallHelper[side].texCor[1][0], wallHelper[side].texCor[1][1]);
		wall.addVertex(verHelper[wallHelper[side].i3].xpos,
			verHelper[wallHelper[side].i3].ypos,
			verHelper[wallHelper[side].i3].zpos,
			wallHelper[side].nx, wallHelper[side].ny, wallHelper[side].nz, wallHelper[side].texCor[2][0], wallHelper[side].texCor[2][1]);
		wall.addVertex(verHelper[wallHelper[side].i4].xpos,
			verHelper[wallHelper[side].i4].ypos,
			verHelper[wallHelper[side].i4].zpos,
			wallHelper[side].nx, wallHelper[side].ny, wallHelper[side].nz, wallHelper[side].texCor[3][0], wallHelper[side].texCor[3][1]);
		for (int i = 0; i < 7; i++) {
			if (i == 5)
				continue;
			wall.addIndex(i % 4);
		}

		wall.initMesh();

		this->addMesh(wall);
	}
}
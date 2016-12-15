#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "shader_prog.h"
#include "texture_loader.h"

using namespace std;

// Vertex structure, contains position in space normal vector and texture coordinates
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// Mesh class, containing all data needed when rendering it
class Mesh {
	// Mesh Data
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	//  Render data
	GLuint VAO, VBO, EBO;

public:
	// Constructors
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

	// Draw mesh using passed shader
	void draw(ShaderProgram shader);

	// Set up mesh data buffers - vertices, textures
	void initMesh();

	// Pushes new vertex into vertices vector
	void addVertex(GLfloat x, GLfloat y, GLfloat z,
		GLfloat nx, GLfloat ny, GLfloat nz,
		GLfloat tx, GLfloat ty);

	// Pushes new index into indices vector
	void addIndex(GLint index);

	// Pushes new texture into textures vector
	void addTexture(const Texture texture);
};


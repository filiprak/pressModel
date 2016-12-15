#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "shader_prog.h"
#include "texture_loader.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// Mesh class, containing all data needed when rendering it
class Mesh {
public:
	// Mesh Data
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	// Constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

	// Draw mesh using passed shader
	void draw(ShaderProgram shader);
private:
	//  Render data
	GLuint VAO, VBO, EBO;

	// Set up mesh data buffers - vertices, textures
	void initMesh();
};


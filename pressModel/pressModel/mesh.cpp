#include "mesh.h"
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(glm::vec3 color) {
	this->color = color;
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures,
	glm::vec3 color)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->color = color;
}

void Mesh::initMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram shader) const {
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding

		glUniform1i(glGetUniformLocation(shader.get_programID(), (this->textures[i].name).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
	
	GLint objectColorLoc = glGetUniformLocation(shader.get_programID(), "meshColor");
	glUniform3fv(objectColorLoc, 1, glm::value_ptr(this->color));

	shader.Use();
	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::addVertex(GLfloat x, GLfloat y, GLfloat z,
					 GLfloat nx, GLfloat ny, GLfloat nz,
					 GLfloat tx, GLfloat ty) {
	Vertex v = { glm::vec3(x, y, z), glm::normalize(glm::vec3(nx, ny, nz)), glm::vec2(tx, ty) };
	this->vertices.push_back(v);
}

void Mesh::addVertex(Vertex vertex) {
	glm::normalize(vertex.Normal);
	this->vertices.push_back(vertex);
}

void Mesh::addIndex(GLint index) {
	this->indices.push_back(index);
}

void Mesh::addTexture(const Texture texture) {
	this->textures.push_back(texture);
}

void Mesh::setColor(glm::vec3 color) {
	this->color = color;
}
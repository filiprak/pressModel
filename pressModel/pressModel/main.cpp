#define GLEW_STATIC
#include <GL/glew.h>
#include "shader_prog.h"
#include "camera.h"
#include "texture_loader.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat lastX = 400, lastY = 300; //mouse initial coords

Camera *camera = new Camera(0.0f, 0.0f, -5.0f, 0.0f, 0.0f);

GLfloat vrads = 0.0f, hrads = 0.0f, zoom = -3.0f, x = 0.0f, y = 0.0f;
float speed = 0.4f, speedscroll = 0.1f, movespeed = 0.003f;

glm::vec3 diffuseLightPos = glm::vec3(9.0f, 9.0f, 9.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP)
		camera->move(HORIZONTAL, 1.0f);
	if (key == GLFW_KEY_DOWN)
		camera->move(HORIZONTAL, -1.0f);
	if (key == GLFW_KEY_LEFT)
		camera->move(VERTICAL, 1.0f);
	if (key == GLFW_KEY_RIGHT)
		camera->move(VERTICAL, -1.0f);
	if (key == GLFW_KEY_W)
		camera->move(ZOOM, 1.0f);
	if (key == GLFW_KEY_S)
		camera->move(ZOOM, -1.0f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		camera->move(VERTICAL, xoffset);
		camera->move(HORIZONTAL, yoffset);
		return;
	}

	camera->rotate(VERTICAL, xoffset);
	camera->rotate(HORIZONTAL, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cout << yoffset << endl;
	camera->move(ZOOM, yoffset);
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

ostream& operator<<(ostream& os, const glm::mat4& mx)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			cout << mx[row][col] << ' ';
		cout << endl;
	}
	return os;
}

int main()
{
	{
		glm::mat4 trans;
		cout << trans << endl;
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		cout << trans << endl;
	}
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 05", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");

		glfwSetCursorPos(window, lastX, lastY);

		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);

		// Let's check what are maximum parameters counts
		GLint nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		cout << "Max vertex attributes allowed: " << nrAttributes << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &nrAttributes);
		cout << "Max texture coords allowed: " << nrAttributes << std::endl;

		// Build, compile and link shader program
		ShaderProgram theProgram("main.vert", "main.frag");

		// Set up vertex data 
		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,

			0, 4, 7,
			0, 7, 3,

			2, 3, 7,
			2, 7, 6,

			6, 7, 4,
			6, 4, 5,

			2, 6, 5,
			2, 5, 1,

			1, 0, 4,
			1, 4, 5,

			2, 4, 6, //diagonal
		};

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		TextureLoader textureLoader;
		textureLoader.loadTextures();
		const Texture weitiTex = textureLoader.getTexture("weitiTexture");
		const Texture iipwTex = textureLoader.getTexture("iipwTexture");

		vector<Texture> meshTextures;
		meshTextures.push_back(weitiTex);
		meshTextures.push_back(iipwTex);

		vector<Vertex> meshVertices;
		meshVertices.push_back({ glm::vec3(1,1,1), glm::vec3(1,0,0), glm::vec2(1,1) });
		meshVertices.push_back({ glm::vec3(1,1,0), glm::vec3(0,1,0), glm::vec2(0,1) });
		meshVertices.push_back({ glm::vec3(1,0,1), glm::vec3(0,0,1), glm::vec2(0,0) });
		meshVertices.push_back({ glm::vec3(1,0,0), glm::vec3(-1,0,0), glm::vec2(1,0) });
		meshVertices.push_back({ glm::vec3(0,1,1), glm::vec3(0,-1,0), glm::vec2(1,1) });
		meshVertices.push_back({ glm::vec3(0,1,0), glm::vec3(0,0,-1), glm::vec2(0,1) });
		meshVertices.push_back({ glm::vec3(0,0,1), glm::vec3(1,0,0), glm::vec2(0,0) });
		meshVertices.push_back({ glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(1,0) });

		vector<GLuint> meshIndices;
		for (int i = 0; i < 36; i++) {
			meshIndices.push_back(i % 8);
		}

		Mesh mesh = Mesh(meshVertices, meshIndices, meshTextures);

		// main event loop ///////////////////////////////////////////////////////////////////////////////////////////
		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 trans = glm::mat4();
			static GLfloat rot_angle = 0.0f;
			trans = glm::rotate(trans, -glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			rot_angle += 0.02f;
			if (rot_angle >= 360.0f)
				rot_angle -= 360.0f;

			glm::mat4 view = camera->getViewMatrix();

			glm::mat4 projection;
			projection = glm::perspective(20.0f, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);

			glm::mat4 model = glm::mat4();
			//model = glm::rotate(model, -glm::radians(0.0f), glm::vec3(1.0f, 1.0f, -1.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

			GLuint transformLoc = glGetUniformLocation(theProgram.get_programID(), "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			GLint modelLoc = glGetUniformLocation(theProgram.get_programID(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLint viewLoc = glGetUniformLocation(theProgram.get_programID(), "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLint projLoc = glGetUniformLocation(theProgram.get_programID(), "projection");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			// light
			glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 objectColor = glm::vec3(0.9f, 0.0f, 0.0f);
			GLint objectColorLoc = glGetUniformLocation(theProgram.get_programID(), "objectColor");
			glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));

			GLint lightLoc = glGetUniformLocation(theProgram.get_programID(), "lightColor");
			glUniform3fv(lightLoc, 1, glm::value_ptr(lightColor));

			GLint lightPosLoc = glGetUniformLocation(theProgram.get_programID(), "lightPos");
			glUniform3fv(lightPosLoc, 1, glm::value_ptr(diffuseLightPos));

			mesh.draw(theProgram);

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
		glfwTerminate();
		getchar();
		return 0;
	}
	glfwTerminate();
	
	return 0;
}

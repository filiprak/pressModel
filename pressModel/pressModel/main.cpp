#define GLEW_STATIC
#include <GL/glew.h>
#include "shader_prog.h"
#include "camera.h"
#include "light.h"
#include "texture_loader.h"
#include "scene.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera(24.0f, 0.0f, 0.0f);

glm::vec3 diffuseLightPos = glm::vec3(10.0f, 20.0f, 30.0f);

void RenderQuad();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_C) {
		static bool enable = true;
		if (enable) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			enable = false;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			enable = true;
		}
	}
	if (key == GLFW_KEY_UP)
		diffuseLightPos.y += 1.0f;
	if (key == GLFW_KEY_DOWN)
		diffuseLightPos.y -= 1.0f;
	if (key == GLFW_KEY_LEFT)
		diffuseLightPos.x -= 1.0f;
	if (key == GLFW_KEY_RIGHT)
		diffuseLightPos.x += 1.0f;
	if (key == GLFW_KEY_D)
		diffuseLightPos.z -= 1.0f;
	if (key == GLFW_KEY_E)
		diffuseLightPos.z += 1.0f;

	/*if (key == GLFW_KEY_W)
		light.changeIntensity(0.1f);
	if (key == GLFW_KEY_S)
		light.changeIntensity(-0.1f);*/
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		camera.move(VERTICAL, xoffset);
		camera.move(HORIZONTAL, yoffset);
		return;
	}*/

	camera.rotate(VERTICAL, xoffset);
	camera.rotate(HORIZONTAL, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cout << yoffset << endl;
	camera.move(yoffset);
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

		glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
		ShaderProgram theProgram("main.vert.shader", "main.frag.shader");
		ShaderProgram simpleDepthShader("depth.vert.shader", "depth.frag.shader");
		ShaderProgram debugQuadShader("debug.vert.shader", "debug.frag.shader");
		vector<ShaderProgram> shaders;
		shaders.push_back(theProgram);

		// light
		glm::vec3 objectColor = glm::vec3(0.9f, 0.0f, 0.0f);

		TextureLoader textureLoader;
		textureLoader.loadTextures();

		Scene scene(diffuseLightPos, textureLoader, WIDTH, HEIGHT, 2048, 2048);

		// main event loop ///////////////////////////////////////////////////////////////////////////////////////////
		while (!glfwWindowShouldClose(window))
		{
			glm::vec3 oldLightPos = diffuseLightPos;

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			static GLfloat rot_angle = 0.0f;
			rot_angle += 0.001f;
			if (rot_angle >= 360.0f)
				rot_angle -= 360.0f;

			camera.use(shaders, WIDTH, HEIGHT, 20.0f);

			glm::vec3 pos = camera.getPosition();
			cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")  " << endl;

			scene.setLightPosition(diffuseLightPos);

			// render scene
			scene.renderShadow(simpleDepthShader);
			scene.render(shaders, diffuseLightPos - oldLightPos);

			// Render Depth map to quad
			/*debugQuadShader.Use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, scene.getShadowDepthMapId());
			RenderQuad();*/

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

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
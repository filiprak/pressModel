#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "mesh.h"
#include "model.h"
#include "light.h"
#include "shadowmap.h"
#include "cuboid.h"
#include "shader_prog.h"


// Class of 3d scene with all objects, shadow and light to render
class Scene {

	// Scene elements
	Cuboid cubeLight, cube, testcube, cube2, cube3, cubeFloor;

	// common elements
	ShadowMap shadowMap;
	Light light;

	// Reference to texture loader
	TextureLoader textureLoader;

	GLfloat width, height;

public:
	// Constructors
	Scene(glm::vec3 lightPos, TextureLoader &textureLoader, GLfloat width, GLfloat height,
		GLfloat swidth = 1024, GLfloat sheight = 1024,
		GLfloat snearPlane = 0.01f, GLfloat sfarPlane = 100.0)
		: shadowMap(light.getPosition(), swidth, sheight, snearPlane, sfarPlane), light(lightPos) {
		this->width = width;
		this->height = height;
		this->textureLoader = textureLoader;

		this->createModels();

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// Sets new light position on the scene
	void setLightPosition(glm::vec3 position) {
		this->light.setPosition(position);
	}

	// draw scene on viewport
	void render(vector<ShaderProgram> shaders, glm::vec3 oldLightPos) {
		if (shaders.size() == 0)
			return;

		for (int i = 0; i < shaders.size(); ++i) {
			shaders[i].Use();
			GLuint lightSpaceMatrixLocation = glGetUniformLocation(shaders[i].get_programID(), "lightSpaceMatrix");
			glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->shadowMap.getLightSpaceMatrix()));
			glUniform1i(glGetUniformLocation(shaders[i].get_programID(), "shadowMap"), 10);
		}

		// Reset viewport
		glViewport(0, 0, this->width, this->height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0 + 10);
		glBindTexture(GL_TEXTURE_2D, this->shadowMap.getDepthMapId());

		// Turn on light and draw all models
		this->light.use(shaders);
		this->drawModels(shaders, oldLightPos);
	}
	
	// draw shadow depth map
	void renderShadow(ShaderProgram shadowMapShader) {
		// at first update light position
		shadowMap.updateLightPosition(this->light.getPosition());

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		shadowMapShader.Use();
		GLuint lightSpaceMatrixLocation = glGetUniformLocation(shadowMapShader.get_programID(), "lightSpaceMatrix");
		glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->shadowMap.getLightSpaceMatrix()));
		glUniform1i(glGetUniformLocation(shadowMapShader.get_programID(), "shadowMap"), 10);

		glViewport(0, 0, this->shadowMap.getWidth(), this->shadowMap.getHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, this->shadowMap.getDepthMapFBOId());
		glClear(GL_DEPTH_BUFFER_BIT);

		// draw model shadows
		this->drawShadows(shadowMapShader);

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint getShadowDepthMapId() {
		return this->shadowMap.getDepthMapId();
	}

private:
	void drawShadows(ShaderProgram shadowMapShader) {
		cube.draw(shadowMapShader);
		cube2.draw(shadowMapShader);
		cube3.draw(shadowMapShader);
		testcube.draw(shadowMapShader);
	}

	void drawModels(vector<ShaderProgram> shaders, glm::vec3 delta) {
		shaders[0].Use();
		cube.draw(shaders[0]);
		cube2.draw(shaders[0]);
		cube3.draw(shaders[0]);
		testcube.draw(shaders[0]);
		cubeFloor.draw(shaders[0]);

		cubeLight.move(delta);
		cubeLight.draw(shaders[0]);
	}

	// Construct all models and place it on the scene
	void createModels() {
		// prepare textures
		const Texture weitiTex = this->textureLoader.getTexture("weitiTexture");
		const Texture iipwTex = this->textureLoader.getTexture("iipwTexture");
		const Texture komanTex = this->textureLoader.getTexture("komanTexture");
		const Texture metalTex = this->textureLoader.getTexture("metalTexture");

		this->cubeLight = Cuboid(1, 1, 1, this->light.getPosition(), glm::vec3(0, 0, 0));
		this->cube = Cuboid(2, 8, 4, glm::vec3(6, 0, 0), glm::vec3(0, 0, 0));
		this->cube.addTexture(metalTex);
		this->testcube = Cuboid(7, 9, 5, glm::vec3(0, 0, 10), glm::vec3(0, 0, 0));
		this->testcube.addTexture(metalTex);
		this->cube2 = Cuboid(2, 8, 4, glm::vec3(-6, 0, 0), glm::vec3(0, 0, 0));
		this->cube2.addTexture(metalTex);
		this->cube3 = Cuboid(14, 2, 4, glm::vec3(0, 5, 0), glm::vec3(0, 0, 0));
		this->cube3.addTexture(metalTex);
		this->cubeFloor = Cuboid(100, 0.5, 100, glm::vec3(0, -4.25, 0), glm::vec3(0, 0, 0));
		this->cubeFloor.addTexture(metalTex);
		this->cubeFloor.setColor(glm::vec3(0.3, 0.7, 0.4));
	}
};
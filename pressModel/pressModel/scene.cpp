
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
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
#include "barrel.h"
#include "shader_prog.h"
#include "scene.h"


Scene::Scene(glm::vec3 lightPos, TextureLoader &textureLoader, GLfloat width, GLfloat height,
	GLfloat swidth, GLfloat sheight,
	GLfloat snearPlane, GLfloat sfarPlane)
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

void Scene::setLightPosition(glm::vec3 position) {
	this->light.setPosition(position);
}

void Scene::render(vector<ShaderProgram> shaders, glm::vec3 oldLightPos) {
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

void Scene::renderShadow(ShaderProgram shadowMapShader) {
	// at first update light position
	shadowMap.updateLightPosition(this->light.getPosition());

	shadowMapShader.Use();
	GLuint lightSpaceMatrixLocation = glGetUniformLocation(shadowMapShader.get_programID(), "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->shadowMap.getLightSpaceMatrix()));
	glUniform1i(glGetUniformLocation(shadowMapShader.get_programID(), "shadowMap"), 10);

	glViewport(0, 0, this->shadowMap.getWidth(), this->shadowMap.getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, this->shadowMap.getDepthMapFBOId());
	glClear(GL_DEPTH_BUFFER_BIT);

	// draw model shadows
	this->drawShadows(shadowMapShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Scene::getShadowDepthMapId() {
	return this->shadowMap.getDepthMapId();
}

void Scene::performMovement(GLfloat tapeSpeed, GLfloat pressSpeed) {
	static bool tapeStop = true;
	static int moveDir = -1;

	if (tapeStop) {
		// piston movement
		if (pistonHead.getPosition().y < -3.2) {
			moveDir = 1;
			for (int i = 0; i < 4; ++i) {
				if (elemHolders[i].getPosition().z > -0.1 &&
					elemHolders[i].getPosition().z < 0.1 &&
					!elements[i].deformed) {
					elements[i].changeSize(2.5, 0.2);
					elements[i].addTexture(this->textureLoader.getTexture("metalTexture"));
					elements[i].move(glm::vec3(0.0, -0.15, 0.0));
					elements[i].deformed = true;
				}
			}
		}
		if (pistonHead.getPosition().y > 2)
			moveDir = -1;

		piston.move(glm::vec3(0, 0.03f * moveDir * pressSpeed, 0));
		pistonHead.move(glm::vec3(0, 0.03f * moveDir * pressSpeed, 0));

		if (pistonHead.getPosition().y > 2)
			tapeStop = false;
		return;
	}

	// elements and its holders movement
	if (!tapeStop) {
		for (int i = 0; i < 4; ++i) {
			if (elemHolders[i].getPosition().z > 20.0f) {
				elemHolders[i].move(glm::vec3(0, 0, -40.0f));
				elements[i].move(glm::vec3(0, 0, -40.0f));

				if (elements[i].deformed) {
					elements[i].changeSize(2.0, 1.0);
					elements[i].addTexture(this->textureLoader.getTexture("metalTexture"));
					elements[i].move(glm::vec3(0.0, 0.15, 0.0));
					elements[i].deformed = false;
				}
				tapeStop = true;
			}
			elemHolders[i].move(glm::vec3(0, 0, 0.04f * tapeSpeed));
			elements[i].move(glm::vec3(0, 0, 0.04f * tapeSpeed));
		}

		// rollers movement
		GLfloat angle = 360.0 * 0.04 * tapeSpeed / (2 * rollers[0].getRadius() * glm::pi<GLfloat>());
		for (int i = 0; i < 10; ++i) {
			rollers[i].rotate(glm::vec3(0, -angle, 0));
		}
	}
}

void Scene::drawShadows(ShaderProgram shadowMapShader) {
	frameLeft.draw(shadowMapShader);
	frameRight.draw(shadowMapShader);
	frameRollersL.draw(shadowMapShader);
	frameRollersR.draw(shadowMapShader);
	frameTop.draw(shadowMapShader);
	piston.draw(shadowMapShader);
	pistonHead.draw(shadowMapShader);
	table.draw(shadowMapShader);

	for (int i = 0; i < 10; ++i) {
		rollers[i].draw(shadowMapShader);
	}

	for (int i = 0; i < 6; ++i) {
		supports[i].draw(shadowMapShader);
	}

	for (int i = 0; i < 4; ++i) {
		elemHolders[i].draw(shadowMapShader);
		elements[i].draw(shadowMapShader);
	}
}

void Scene::drawModels(vector<ShaderProgram> shaders, glm::vec3 delta) {
	shaders[0].Use();
	frameLeft.draw(shaders[0]);
	frameRight.draw(shaders[0]);
	frameRollersL.draw(shaders[0]);
	frameRollersR.draw(shaders[0]);
	frameTop.draw(shaders[0]);
	floor.draw(shaders[0]);
	piston.draw(shaders[0]);
	pistonHead.draw(shaders[0]);
	table.draw(shaders[0]);

	cubeLight.move(delta);
	cubeLight.draw(shaders[0]);

	for (int i = 0; i < 10; ++i) {
		rollers[i].draw(shaders[0]);
	}

	for (int i = 0; i < 6; ++i) {
		supports[i].draw(shaders[0]);
	}

	for (int i = 0; i < 4; ++i) {
		elemHolders[i].draw(shaders[0]);
		elements[i].draw(shaders[0]);
	}
}

void Scene::createModels() {
	// prepare textures
	const Texture weitiTex = this->textureLoader.getTexture("weitiTexture");
	const Texture iipwTex = this->textureLoader.getTexture("iipwTexture");
	const Texture stripestex = this->textureLoader.getTexture("stripes");
	const Texture metalTex = this->textureLoader.getTexture("metalTexture");
	const Texture metal1Tex = this->textureLoader.getTexture("metal1Texture");
	const Texture floor1 = this->textureLoader.getTexture("floor1");
	const Texture floor2 = this->textureLoader.getTexture("floor2");
	const Texture metalplate = this->textureLoader.getTexture("metalplate");
	const Texture bluesurf = this->textureLoader.getTexture("bluesurf");
	const Texture rollertex = this->textureLoader.getTexture("roller");

	cubeLight = Cuboid(1, 1, 1, this->light.getPosition());
	cubeLight.setColor(glm::vec3(1.0, 1.0, 1.0));

	frameRight = Cuboid(2, 8, 4, glm::vec3(6, 0, 0), glm::vec3(0.05));
	frameRight.addTexture(bluesurf);
	frameRollersL = Cuboid(3, 4, 30, glm::vec3(6, -6, 0));
	frameRollersL.addTexture(metalplate);
	frameRollersR = Cuboid(3, 4, 30, glm::vec3(-6, -6, 0));
	frameRollersR.addTexture(metalplate);
	frameLeft = Cuboid(2, 8, 4, glm::vec3(-6, 0, 0), glm::vec3(0.05));
	frameLeft.addTexture(bluesurf);
	frameTop = Cuboid(14, 4, 4, glm::vec3(0, 6, 0), glm::vec3(0.05));
	frameTop.addTexture(bluesurf);
	table = Cuboid(14, 2, 30, glm::vec3(0, -9, 0));
	table.addTexture(metalplate);
	floor = Cuboid(100, 0.5, 100, glm::vec3(0, -14.25, 0), glm::vec3(0.1));
	floor.addTexture(floor1);
	floor.setColor(glm::vec3(0.3, 0.7, 0.4));

	piston = Barrel(1, 10, 50, glm::vec3(0, 6, 0), glm::vec3(0.2));
	piston.addTexture(metal1Tex);
	pistonHead = Barrel(4, 1, 50, glm::vec3(0, 0.5, 0), glm::vec3(0.1));
	pistonHead.addTexture(metalTex);

	for (int i = 0; i < 10; ++i) {
		rollers[i] = Barrel(1, 9, 20, glm::vec3(0, -5.5, 3 * i - 13.5), glm::vec3(0.48, 0.11, 1.0), glm::vec3(0, 0, 90));
		rollers[i].addTexture(rollertex);
	}

	for (int i = 0; i < 6; i = i + 2) {
		supports[i] = Barrel(1, 4, 50, glm::vec3(6, -12, 5 * i - 10), glm::vec3(0.5));
		supports[i].addTexture(metalTex);
		supports[i + 1] = Barrel(1, 4, 50, glm::vec3(-6, -12, 5 * i - 10), glm::vec3(0.5));
		supports[i + 1].addTexture(metalTex);
	}

	for (int i = 0; i < 4; ++i) {
		elemHolders[i] = Cuboid(8, 0.5, 8, glm::vec3(0, -4.25, 10 * i - 20), glm::vec3(0.1));
		elemHolders[i].addTexture(stripestex);
		elements[i] = Barrel(2, 1, 50, glm::vec3(0, -3.75, 10 * i - 20));
		elements[i].addTexture(metalTex);
	}
}
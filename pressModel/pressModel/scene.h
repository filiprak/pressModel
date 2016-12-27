#pragma once

#include <glm/glm.hpp>
#include "light.h"
#include "shadowmap.h"
#include "cuboid.h"
#include "barrel.h"
#include "shader_prog.h"


// Class of 3d scene with all objects, shadow and light to render
class Scene {
	// Scene elements
	Cuboid frameLeft, frameRight, frameTop, frameRollersL, frameRollersR, floor, cubeLight;
	Cuboid table;
	Barrel piston, pistonHead;
	Barrel rollers[10];

	Barrel supports[6];

	Barrel elements[4];
	Cuboid elemHolders[4];

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
		GLfloat snearPlane = 0.01f, GLfloat sfarPlane = 100.0);

	// Sets new light position on the scene
	void setLightPosition(glm::vec3 position);

	// draw scene on viewport
	void render(vector<ShaderProgram> shaders, glm::vec3 oldLightPos);
	
	// draw shadow depth map
	void renderShadow(ShaderProgram shadowMapShader);

	GLuint getShadowDepthMapId();

	// Moves dynamic objects about small offset
	void Scene::performMovement(GLfloat tapeSpeed, GLfloat pressSpeed);

private:
	void drawShadows(ShaderProgram shadowMapShader);

	void drawModels(vector<ShaderProgram> shaders, glm::vec3 delta);

	// Construct all models and place it on the scene
	void createModels();
};
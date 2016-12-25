#pragma once

// GL Includes
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "shader_prog.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Shadow map class contains transformation matrices to light's point of view
// and prepares texture and frame buffer of depth map
class ShadowMap {

	glm::mat4 lightSpaceMatrix;
	glm::mat4 lightProjection;

	GLuint depthMapFBO;
	GLuint depthMap;

	// Sizes of texture in pixels
	GLfloat width, height;

public:

	// Constructor
	ShadowMap(glm::vec3 lightPosition,
		GLfloat width = 2048, GLfloat height = 2048,
		GLfloat near_plane = 0.01f, GLfloat far_plane = 100.0f) {
		this->lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		this->lightSpaceMatrix = lightProjection * lightView;
		this->width = width;
		this->height = height;

		// Create frame buffer
		glGenFramebuffers(1, &this->depthMapFBO);
		// Generate shaow map texture
		glGenTextures(1, &this->depthMap);
		glBindTexture(GL_TEXTURE_2D, this->depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
		// Color buffer is redundant
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Update matrices with new light position
	void updateLightPosition(glm::vec3 newLightPosition) {
		glm::mat4 lightView = glm::lookAt(newLightPosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		this->lightSpaceMatrix = this->lightProjection * lightView;
	}

	// Getters
	glm::mat4 getLightSpaceMatrix() const {
		return this->lightSpaceMatrix;
	}

	GLuint getDepthMapId() const {
		return this->depthMap;
	}

	GLuint getDepthMapFBOId() const {
		return this->depthMapFBO;
	}

	GLfloat getWidth() const {
		return this->width;
	}

	GLfloat getHeight() {
		return this->height;
	}
};
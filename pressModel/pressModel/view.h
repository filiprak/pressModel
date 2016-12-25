#pragma once

// GL Includes
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "shader_prog.h"
#include "light.h"

using namespace std;


class View
{
public:
	View(vector<ShaderProgram> shaders, Light light, glm::mat4 view);
	
};


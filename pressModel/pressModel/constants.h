#pragma once

#include <string>

using namespace std;

// number of textures in texturePaths
#define NUM_TEXTURES 2

// names of textures used in shaders and paths to load it from
string texturePaths[NUM_TEXTURES * 2] {
	"iipwTexture", "iipw.png",
	"weitiTexture", "weiti.png",
};

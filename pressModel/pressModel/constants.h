#pragma once

#include <string>

using namespace std;

// Size of window on screen
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

// number of textures in texturePaths
#define NUM_TEXTURES 4

// names of textures used in shaders and paths to load it from
string texturePaths[NUM_TEXTURES * 2] {
		"iipwTexture", "iipw.png",
		"weitiTexture", "weiti.png",
		"komanTexture", "koman.png",
		"metalTexture", "metal.png",
};

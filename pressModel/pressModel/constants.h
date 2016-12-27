#pragma once

#include <string>

using namespace std;

// Size of window on screen
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

// number of textures in texturePaths
#define NUM_TEXTURES 11

// names of textures used in shaders and paths to load it from
string texturePaths[NUM_TEXTURES * 2] {
		"iipwTexture", "iipw.png",
		"weitiTexture", "weiti.png",
		"brass", "brass.jpg",
		"metalTexture", "metal.png",
		"metal1Texture", "metal1.png",
		"metalplate", "metalplate.png",
		"floor1", "floor1.png",
		"floor2", "floor2.png",
		"bluesurf", "blue.png",
		"roller", "roller.png",
		"stripes", "stripes.jpg",
};

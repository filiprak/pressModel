#pragma once

#include <GL/glew.h>
#include <string>
#include <hash_map>

using namespace std;

struct Texture {
	GLuint id;
	string name; // Texture names must be unique because they are also uniform names
	string path;
};

// Is used to load all necessaary textures from files into memory
class TextureLoader {

	// Contains names of textures and their corresponding loaded Texture structures
	hash_map<string, Texture> textures;
public:
	// Constructor
	TextureLoader();

	// Loads all textures into hashmap, throws exception if cannot read file
	void loadTextures();

	const Texture getTexture(const string name);
};
#include "texture_loader.h"
#include "constants.h"

#include <SOIL.h>
#include <iostream>

TextureLoader::TextureLoader() {

}

void TextureLoader::loadTextures() {
	for (int i = 0; i < NUM_TEXTURES * 2; i = i + 2) {
		int width, height;
		unsigned char* image = SOIL_load_image((texturePaths[i + 1]).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == nullptr) {
			string message = "Failed to load texture file: " + texturePaths[i + 1];
			throw exception(message.c_str());
		}

		Texture texture;
		glGenTextures(1, &(texture.id));
		glActiveTexture(texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.name = texturePaths[i];
		texture.path = texturePaths[i + 1];
		this->textures[texture.name] = texture;
	}
}

const Texture TextureLoader::getTexture(const string name) {
	return this->textures[name];
}
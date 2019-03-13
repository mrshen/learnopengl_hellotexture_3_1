#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "stb_image.h"
#include <iostream>

GLuint loadTexture(const char *texturePath, int slot, int innerFormat, unsigned int format, bool needFlip)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + slot); 
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(needFlip);

	int width, height, nrChannels;
	unsigned char* texData = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (texData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error: FAILED TO LOAD TEXTURE [" << texturePath << "]." << std::endl;
	}
	stbi_image_free(texData);

	return texture;
}
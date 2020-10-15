#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//const unsigned int MAX_PATH = 255;


class Texture
{
public:
	unsigned int ID;
	std::string name;
	bool isFlipped;

	Texture(const char* path, bool flip = false, bool hasAlpha = false);
	~Texture();

private:
	bool loaded = false;
	int width, height, nrChannels;
	bool doLogging = true;
};

#endif

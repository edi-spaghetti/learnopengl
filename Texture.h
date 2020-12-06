#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Constants.h"


class Texture
{
public:
	unsigned int ID;
	std::string name;
	std::string path;
	bool isFlipped;

	Texture() {};
	Texture(std::string path, const char* varName = NULL,
		bool flip = true,
		int wrapping = GL_REPEAT, int filtering = GL_LINEAR,
		bool gammaCorrect = true
	);
	~Texture();

protected:
	int getFormat();
	int getInternalFormat(bool gammaCorrection);
	bool loaded = false;
	int width, height, nrChannels;
	bool doLogging = true;
};


class CubeMap : public Texture
{
public:
	unsigned int ID;
	std::vector<std::string> faces;

	CubeMap() {};
	CubeMap(std::vector<std::string> faces, std::string name);
	~CubeMap();
};

#endif

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Geometry.h"
#include "Texture.h"

const unsigned int MAX_TEXTURE_UNITS = 16;


class Shader
{
public:
	unsigned int ID;
	float currentZoom = 0.0f;
	float currentAlpha = 0.2f;

	// creates a new shader program
	Shader(
		const char* vertPath, const char* fragPath,
		Geometry geo, Texture* textures = NULL, unsigned int nTex = 0);
	~Shader();

	// update shaders
	void update();
	// draw loaded geometry 
	void draw();

	// set values of types
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void increaseTransparency();
	void decreaseTransparency();

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;	

	Geometry geometry;
	bool geometryLoaded = false;
	bool elementBuffer = false;

	unsigned int numTextures;
	Texture* texList;
	bool texLoaded = false;

	float maxAlpha = 1.0f;
	float minAlpha = 0.0f;
	float alphaIncrement = 0.05f;

	bool doLogging = true;

	static std::string readShaderFile(const char* shader_file);
	int createVertexShader(const char* path);
	int createFragmentShader(const char* path);
	void loadGeometry(Geometry geo);
	void loadTextures(Texture* textures = NULL, unsigned int nTex = 0);
};

#endif
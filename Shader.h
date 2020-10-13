#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Geometry.h"


class Shader
{
public:
	unsigned int ID;
	
	// creates a new shader program
	Shader(const char* vertPath, const char* fragPath, Geometry geo);
	~Shader();

	// update shaders
	void update();
	// draw loaded geometry 
	void draw();

	// set values of types
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;	

	Geometry geometry;
	bool geometryLoaded = false;
	bool elementBuffer = false;

	static std::string readShaderFile(const char* shader_file);
	int createVertexShader(const char* path);
	int createFragmentShader(const char* path);
	void loadGeometry(Geometry geo);
};

#endif
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Geometry.h"
#include "Texture.h"
#include "ScalarAttribute.h"

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
	void setMatrix(const std::string& name, glm::mat4 value) const;
	void setMatrix(const std::string& name, glm::mat3 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 value) const;

	void increaseTransparency();
	void decreaseTransparency();

	void translate(float x, float y);
	void rotate(float angle);
	void scale(float value);

	ScalarAttribute ambient = ScalarAttribute(0.1f, 0.0f, 1.0f, 0.05f);
	ScalarAttribute specular = ScalarAttribute(0.5f, 0.0f, 1.0f, 0.1f);
	ScalarAttribute shininess = ScalarAttribute(5.0f, 1.0f, 8.0f, 1.0f);

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

	// init transformation as id matrix
	glm::mat4 currentTransformation = glm::mat4(1.0f);

	bool doLogging = true;

	static std::string readShaderFile(const char* shader_file);
	int createVertexShader(const char* path);
	int createFragmentShader(const char* path);
	void loadGeometry(Geometry geo);
	void loadTextures(Texture* textures = NULL, unsigned int nTex = 0);
};

#endif
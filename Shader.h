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
#include "Materials.h"
#include "Attributes.h"
#include "Constants.h"


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
		Geometry geo, Material mat,
		Texture* textures = NULL, unsigned int nTex = 0);
	~Shader();

	// world space position
	bool initPositionSet = false;
	glm::vec3 initPosition = glm::vec3(0.0f);
	glm::vec3 position = glm::vec3(0.0f);
	void setPosition(glm::vec3 newPostion);

	// world space direction
	bool initDirectionSet = false;
	glm::vec3 initDirection = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	void setDirection(glm::vec3 newDir);

	// update shaders
	void update();
	// draw loaded geometry 
	void draw();

	void loadGeometry(Geometry geo);
	void loadTextures(Texture* textures = NULL, unsigned int nTex = 0);
	void loadMaterials(Material mat);

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

	Attribute<glm::vec3> ambient;
	Attribute<glm::vec3> diffuse;
	Attribute<glm::vec3> specular;
	Attribute<float> shininess;

	Geometry geometry;
	unsigned int numTextures;
	Texture* texList;
	Material material;

	bool geometryLoaded = false;
	bool elementBuffer = false;
	bool texLoaded = false;
	bool materialLoaded = false;

	bool invertSpec = false;
	bool addEmission = false;
	bool animateEmission = false;

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;	

	float maxAlpha = 1.0f;
	float minAlpha = 0.0f;
	float alphaIncrement = 0.05f;

	// init transformation as id matrix
	glm::mat4 currentTransformation = glm::mat4(1.0f);

	bool doLogging = true;

	static std::string readShaderFile(const char* shader_file);
	int createVertexShader(const char* path);
	int createFragmentShader(const char* path);
};

#endif
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
	Shader(const char* vertPath, const char* fragPath);
	Shader(
		const char* vertPath, const char* fragPath,
		Geometry geo, Material mat,
		Texture* textures = NULL, unsigned int nTex = 0);
	Shader(const char* vertPath, const char* fragPath, Model mod);
	~Shader();
	void tearDown();

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

	// TODO: rotation
	bool rotating = false;
	float rotationAngle = glm::radians(0.0f);
	glm::vec3 pointOfRotation = glm::vec3(0.0f, 1.0f, 0.0f);

	// for scaling geometry
	glm::vec3 size = glm::vec3(1.0f);

	// update shaders
	void update();
	// draw loaded geometry 
	void draw();
	void screenDraw(unsigned int tcb);
	void drawWithOutline();

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
	Model mod;

	bool geometryLoaded = false;
	bool elementBuffer = false;
	bool texLoaded = false;
	bool materialLoaded = false;
	bool modelLoaded = false;

	bool invertSpec = false;
	bool addEmission = false;
	bool animateEmission = false;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat3 getNormalMatrix();
	void move(const int direction, float deltaTime);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 objRight;
	glm::vec3 objUp;
	float baseSpeed = 50.0f;
	glm::vec3 outlineColour = glm::vec3(1.0f, 0.25f, 0.25f);

//protected:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;	

	float maxAlpha = 1.0f;
	float minAlpha = 0.0f;
	float alphaIncrement = 0.05f;

	// init transformation as id matrix
	glm::mat4 currentTransformation = glm::mat4(1.0f);

	bool doLogging = true;

	void createShaderProgram(const char* vertPath, const char* fragPath);
	static std::string readShaderFile(const char* shader_file);
	int createVertexShader(const char* path);
	int createFragmentShader(const char* path);
};


class LightSource : public Shader
{
public:
	LightSource(
		int lightType,
		Geometry geo, Material mat,
		const char* vertPath = "lightSource.vs", const char* fragPath = "lightSource.fs",
		Texture* textures = NULL, unsigned int nTex = 0
	) : Shader(vertPath, fragPath, geo, mat, textures, nTex) {
		this->type = lightType;
	};
	LightSource(
		int lightType,
		Model mod,
		const char* vertPath = "lightSource.vs", 
		const char* fragPath = "lightSource.fs"
	) : Shader(vertPath, fragPath, mod) {
		this->type = lightType;
	};

	int attenuationIndex = 3;
	static const int numAttenuationSettings = 12;
	glm::vec4 attenuation[numAttenuationSettings] = {
		glm::vec4(7.0, 1.0, 0.7, 1.8),
		glm::vec4(13.0, 1.0, 0.35, 0.44),
		glm::vec4(20.0, 1.0, 0.22, 0.2),
		glm::vec4(32.0, 1.0, 0.14, 0.07),
		glm::vec4(50.0, 1.0, 0.09, 0.032),
		glm::vec4(65.0, 1.0, 0.07, 0.017),
		glm::vec4(100.0, 1.0, 0.045, 0.0075),
		glm::vec4(160.0, 1.0, 0.027, 0.0028),
		glm::vec4(200.0, 1.0, 0.022, 0.0019),
		glm::vec4(325.0, 1.0, 0.014, 0.0007),
		glm::vec4(600.0, 1.0, 0.007, 0.0002),
		glm::vec4(3250.0, 1.0, 0.0014, 0.000007)
	};

	void setAttenuation(int index);
	float distance = attenuation[attenuationIndex].x;
	float constant = attenuation[attenuationIndex].y;
	float linear = attenuation[attenuationIndex].z;
	float quadratic = attenuation[attenuationIndex].w;

	float innerBeam = glm::cos(glm::radians(12.5f));
	float outerBeam = glm::cos(glm::radians(17.5f));
	int type;

	std::string uniform(int i, std::string name);

	// make lights smaller than other geo
	glm::vec3 size = glm::vec3(0.2f);
};


#endif
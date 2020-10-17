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

const unsigned int MAX_TEXTURE_UNITS = 16;


class Shader
{
public:
	unsigned int ID;
	float currentZoom = 0.0f;
	float currentAlpha = 0.2f;

	int screenWidth = 800;
	int screenHeight = 600;
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed = 0.05f;
	float pitch = 0.0f;
	float yaw = -90.0f;  // init as -90 to point in correction direction
	glm::vec3 cameraDirection;
	const float camSensitivity = 0.1f;
	float lastX = 400;
	float lastY = 300;
	bool firstMouse = true;

	// field of view, also used for zoom
	float FOV = 45.0f;
	const float maxFOV = 60.0f;
	const float minFOV = 1.0f;

	float deltaTime;
	float lastFrame;
	float currentFrame;

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

	void increaseTransparency();
	void decreaseTransparency();

	void translate(float x, float y);
	void rotate(float angle);
	void scale(float value);

	void moveCamera(int key);
	void updateCameraDirection(double xpos, double ypos);
	void updateFOV(float yoffset);

	void updateTime();

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
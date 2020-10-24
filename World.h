#ifndef WORLD_H
#define WORLD_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Utils.h"

class Camera;  // forward declaration

class World
{
public:
	World(GLFWwindow* win, Camera* cam, Shader* shad);
	~World();

	GLFWwindow* window;
	Shader* shader;
	Camera* camera;

	glm::mat4 view;
	glm::mat4 projection;

	int screenWidth;
	int screenHeight;

	float deltaTime;
	float lastFrame;
	float currentFrame;

	// actions
	bool staticLight = true;
	float currentLightRotationAngle = 0.0f;

	// main update function for everything contained inside world
	void update();

	// support functions for update
	void updateTime();
	void updateScreen();
	void updateAttributes();
	void updateView();
	void updateProjection();
	void updateModel();

	float getLightRotationAngle();
};

#endif // !WORLD_H

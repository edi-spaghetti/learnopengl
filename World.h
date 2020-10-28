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
	World(GLFWwindow* win, Camera* cam, Shader* obj, Shader* lgt, MaterialManager* mng);
	~World();

	GLFWwindow* window;
	Shader* object;
	Camera* camera;
	Shader* light;
	MaterialManager* matManager;

	glm::mat4 view;
	glm::mat4 projection;

	int screenWidth;
	int screenHeight;

	float deltaTime;
	float lastFrame;
	float currentFrame;

	// actions
	// ==========================================================================
	// 1 Controls if the light spins or not
	bool staticLight = true;
	float currentLightRotationAngle = 0.0f;
	// 2 Controls if the shading is calculated in view space or not
	bool shadeInViewSpace = false;
	// 3 Toggle shading in Gouraud or Phong
	bool toggleGouraudPhong = false;

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
	void cycleMaterial(bool forward);
};

#endif // !WORLD_H

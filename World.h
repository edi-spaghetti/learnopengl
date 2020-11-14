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
	World(GLFWwindow* win, Camera* cam, Shader* obj, MaterialManager* mng, std::vector<LightSource> lgts);
	~World();

	GLFWwindow* window;
	Shader* object;
	Camera* camera;
	std::vector<LightSource> lights;
	MaterialManager* matManager;

	glm::mat4 view;
	glm::mat4 projection;

	int screenWidth;
	int screenHeight;

	float deltaTime;
	float lastFrame;
	float currentFrame;

	bool doLogging = true;

	// actions
	// ==========================================================================
	// 1 Controls if the light spins or not
	// TODO: reimplement for multiple lights
	bool staticLight = true;
	float currentLightRotationAngle = 0.0f;
	// 3 Toggle shading in Gouraud or Phong
	bool toggleGouraudPhong = false;
	// 7 Start / Stop Screen Recording
	bool record = false;
	unsigned int currentOutputFrame = 0;
	const float outputFPS = 25.0f;
	float recordingStartedAt;
	void exportCurrentFrame();
	// 8 lighting types
	//unsigned int lightingType = 0;
	const unsigned int numLightingTypes = 3;

	// main update function for everything contained inside world
	void update();
	void draw();

	// support functions for update
	void updateTime();
	void updateScreen();
	void updateAttributes();
	void updateView();
	void updateProjection();
	void updateModel();

	float getLightRotationAngle();
	void cycleMaterial(bool forward);
	void moveSelected(const int direction);
};

#endif // !WORLD_H

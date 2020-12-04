#ifndef WORLD_H
#define WORLD_H

#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Utils.h"

class Camera;  // forward declaration

class World
{
public:
	World(GLFWwindow* win, Camera* cam, std::vector<Shader*> objs, MaterialManager* mng, 
		std::vector<LightSource*> lgts);
	~World();

	GLFWwindow* window;
	std::vector<Shader*> objects;
	Camera* camera;
	std::vector<LightSource*> lights;
	MaterialManager* matManager;
	Shader screen = Shader("postProcessing.vs", "postProcessing.fs");
	// get the mirror quad roughly the same aspect ratio as screen
	// TODO: make this dynamic
	Shader mirror = Shader("postProcessing.vs", "postProcessing.fs", 0.6f, 0.9f, 0.9f, 0.675f);
	Shader skybox;

	glm::mat4 view;
	glm::mat4 reverseView;
	glm::mat4 projection;

	int screenWidth;
	int screenHeight;
	// TODO: convert generation and storage of these variables into a function
	unsigned int fbo;  // framebuffer object
	unsigned int fboReverse;  // framebuffer object
	unsigned int tcb;  // texture colour buffer
	unsigned int tcbReverse;  // texture colour buffer
	unsigned int rbo;  // render buffer object
	unsigned int rboReverse;  // render buffer object
	// intermediary (if multi-sampling)
	unsigned int iFBO;
	unsigned int iTCB;
	unsigned int iRBO;


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

	void setupPostProcessing();

	// main update function for everything contained inside world
	void update();
	void draw();
	void drawObjects();

	// support functions for update
	void updateTime();
	void updateScreen();
	void updateAttributes();
	void updateView();
	void updateProjection();
	void updateModel();

	float getLightRotationAngle();
	void cycle(const int direction, const int attribute);
	int currentSelection = 0;
	void moveSelected(const int direction);
};

#endif // !WORLD_H

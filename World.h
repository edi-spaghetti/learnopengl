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
	void tearDown();

	GLFWwindow* window;
	std::vector<Shader*> objects;
	Camera* camera;
	std::vector<LightSource*> lights;
	MaterialManager* matManager;
	Shader* screen;
	Shader* mirror;
	Shader* skybox;

	glm::mat4 view;
	glm::mat4 reverseView;
	glm::mat4 projection;

	int screenWidth;
	int screenHeight;

	// framebuffers
	// -----------------------------------------------------------------------
	GLboolean doMultiSample;
	unsigned int fboIndex = 0;
	unsigned int tcbIndex = 1;
	unsigned int rboIndex = 2;
	std::map<const std::string, std::vector<unsigned int>> framebuffers;
	std::map<int, std::string> framebufferResult = {
		{GL_FRAMEBUFFER_COMPLETE, "complete"},
		{GL_FRAMEBUFFER_UNDEFINED, "undefined"},
		{GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "incomplete attachment"},
		{GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "missing image attachment"},
		{GL_FRAMEBUFFER_UNSUPPORTED, "unsupported"},
		{GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "incomplete multi-sample"},
		{GL_INVALID_ENUM, "invalid enum"}
	};
	void createFramebuffer(std::string name, GLenum target, 
		GLboolean doMultiSample);
	void copyBuffer(std::string src, std::string dst);
	void deleteBuffers();

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
	// blinn-phong shading
	bool doBlinnPhong = false;
	// 7 Start / Stop Screen Recording
	bool record = false;
	unsigned int currentOutputFrame = 0;
	const float outputFPS = 25.0f;
	float recordingStartedAt;
	void exportCurrentFrame();
	// 8 lighting types
	//unsigned int lightingType = 0;
	const unsigned int numLightingTypes = 3;

	void setupGlobalOpenGLSettings();
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

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class World;  // forward declaration
#include "World.h"


class Camera
{
public:
	Camera();
	~Camera();

	World* world;

	void move(int key);
	void updateDirection(double xpos, double ypos);
	void updateFOV(float yoffset);

	int screenWidth = 800;
	int screenHeight = 600;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float speed = 0.05f;
	float pitch = 0.0f;
	float yaw = -90.0f;  // init as -90 to point in correction direction
	glm::vec3 direction;
	const float sensitivity = 0.1f;
	float lastX = 400;
	float lastY = 300;
	bool firstMouse = true;

	// field of view, also used for zoom
	float FOV = 45.0f;
	const float maxFOV = 60.0f;
	const float minFOV = 1.0f;
};

#endif // !CAMERA_H

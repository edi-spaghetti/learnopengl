#include "Camera.h"


Camera::Camera()
{
	std::cout << "Init camera" << std::endl;
}

Camera::~Camera()
{
	std::cout << "Destroy camera" << std::endl;
}


void Camera::move(int key)
{
	speed = speedModifier * world->deltaTime;

	if (key == GLFW_KEY_W)
	{
		position += front * speed;
		if (!canFly) position.y = 0.0f;
	}
	else if (key == GLFW_KEY_S)
	{
		position -= front * speed;
		if (!canFly) position.y = 0.0f;
	}
	else if (key == GLFW_KEY_A)
	{
		position -= glm::normalize(
			glm::cross(front, up)
		) * speed;
	}
	else if (key == GLFW_KEY_D)
	{
		position += glm::normalize(
			glm::cross(front, up)
		) * speed;
	}
}


void Camera::updateDirection(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// TODO: convert to setMinMax util function
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
}


void Camera::updateFOV(float yoffset)
{
	FOV -= yoffset;
	if (FOV < minFOV) FOV = minFOV;
	if (FOV > maxFOV) FOV = maxFOV;
}
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "World.h"
#include "Camera.h"
#include "Constants.h"

class World;  // forward declaration

GLFWwindow* createWindow(int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setupUserControls(GLFWwindow* window, World* world);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, int* width, int* height);
float map_to_range(float X, float A, float B, float C, float D);

#endif

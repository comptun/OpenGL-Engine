#pragma once

#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Camera.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(Camera& camera, float deltaTime);


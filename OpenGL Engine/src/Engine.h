#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Keyboard.h"

#include "Camera.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "PerlinNoise.hpp"

class Engine
{
private:
	GLFWwindow* window;
	int WIDTH, HEIGHT;
	bool failed;
private:
	int setup();
public:
	bool hasFailed() const;
	GLFWwindow* getWindow();
	Engine(int w, int h);
};


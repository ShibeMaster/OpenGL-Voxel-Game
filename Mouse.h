#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <iostream>
class Mouse
{
private:
	bool firstMouseMove = true;
	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;
public:
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	glm::vec2 position;
	bool inputLocked = false;

	void MouseCallback(GLFWwindow* window, double xpos, double ypos);
};


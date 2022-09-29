#include "Mouse.h"



void Mouse::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouseMove) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouseMove = false;
	}
	if (inputLocked) {
		xOffset = 0.0f;
		yOffset = 0.0f;
	}
	else {

		xOffset = xpos - lastMouseX;
		yOffset = lastMouseY - ypos;
	}

	position.x = (float)xpos;
	position.y = (float)ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;
}
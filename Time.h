#pragma once
#include <GLFW/glfw3.h>


class Time
{
private:
	static float lastFrameTime;
public:
	static float deltaTime;
	static float fixedDeltaTime;

	static void Update() {
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

	}
};


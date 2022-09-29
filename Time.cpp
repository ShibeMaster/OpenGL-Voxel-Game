#include "Time.h"
float Time::fixedDeltaTime = 0.0f;
float Time::lastFrameTime = 0.0f;
float Time::deltaTime = 0.0f;


void Time::Update() {
	float currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

}
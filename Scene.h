#pragma once
#include "Instance.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
class Scene
{
public:
	int sceneId = 5;

	Scene(){
	}
	virtual void Start() {}
	virtual void FixedUpdate() {}
	virtual void Update() {}
	virtual void End() {}
	
	virtual void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mod) {}
	virtual void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {}
};


#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "InputManager.h"

enum MovementDirection {
	forward,
	backward,
	right,
	left,
	up,
	down
};

// defaults
class Camera
{
public:
	glm::vec3 position;
	glm::vec3 worldUp;

	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	float yaw;
	float pitch;

	float speed;
	float sensitivity;

	Camera();
	Camera(glm::vec3 pos, glm::vec3 wUp, float y, float p);
	void UpdateCameraVectors();
	void ProcessCameraMouse();
	glm::mat4 GetViewMatrix();
};
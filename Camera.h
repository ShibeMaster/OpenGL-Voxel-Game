#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum MovementDirection {
	forward,
	backward,
	right,
	left,
	up,
	down
};

// defaults
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.5f;
const float SENSITIVITY = 0.1f;

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

	float lastMouseX = 800.0f / 2.0f;
	float lastMouseY = 800.0f / 2.0f;
	bool firstMouseMove = true;

	Camera() {}
	Camera(glm::vec3 pos, glm::vec3 wUp = glm::vec3(0.0f, 1.0f, 0.0f), float y = YAW, float p = PITCH) : speed(SPEED), sensitivity(SENSITIVITY) {
		position = pos;
		worldUp = wUp;
		yaw = y;
		pitch = p;
		UpdateCameraVectors();
	}
	void UpdateCameraVectors() {
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		forward = direction;

		right = glm::normalize(glm::cross(forward, worldUp));
		up = glm::normalize(glm::cross(right, forward));
	}
	void ProcessCameraMouse(GLFWwindow* window, double xpos, double ypos) {
		if (firstMouseMove) {
			lastMouseX = xpos;
			lastMouseY = ypos;
			firstMouseMove = false;
		}
		float xoffset = xpos - lastMouseX;
		float yoffset = lastMouseY - ypos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		UpdateCameraVectors();
	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position, position + forward, up);
	}
};
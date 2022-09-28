#pragma once
#include "Mouse.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
	static Mouse mouse;
	static GLFWwindow* window;
	static bool inputLocked;

	static void Initialize() {
		SetMouseLocked();
	}
	static void SetMouseUnlocked() {
		mouse.inputLocked = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	static void SetMouseLocked() {
		mouse.inputLocked = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

#pragma region Input

	static bool GetKeyDown(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}
	static bool GetKeyRelease(int key) {
		return glfwGetKey(window, key) == GLFW_RELEASE;
	}
	static float MoveVert() {
		bool forward = GetKeyDown(GLFW_KEY_W);
		bool backward = GetKeyDown(GLFW_KEY_S);
		return forward && !backward ? 1.0f : backward && !forward ? -1.0f : 0.0f;
	}
	static float MoveHorz() {
		bool forward = GetKeyDown(GLFW_KEY_D);
		bool backward = GetKeyDown(GLFW_KEY_A);
		return forward && !backward ? 1.0f : backward && !forward ? -1.0f : 0.0f;
	}
	static float MoveUpDown() {

		bool forward = GetKeyDown(GLFW_KEY_E);
		bool backward = GetKeyDown(GLFW_KEY_Q);
		return forward && !backward ? 1.0f : backward && !forward ? -1.0f : 0.0f;
	}
	static bool IsMovingAny() {
		return MoveHorz() != 0.0f || MoveVert() != 0.0f || MoveUpDown() != 0.0f;
	}
#pragma endregion
};


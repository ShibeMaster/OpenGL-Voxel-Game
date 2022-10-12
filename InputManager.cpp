#include "InputManager.h"
GLFWwindow* InputManager::window;
Mouse InputManager::mouse;

void InputManager::SetMouseUnlocked() {
	mouse.inputLocked = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void InputManager::SetMouseLocked() {
	mouse.inputLocked = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

#pragma region Input

bool InputManager::GetKeyDown(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}
bool InputManager::GetKeyRelease(int key) {
	return glfwGetKey(window, key) == GLFW_RELEASE;
}
float InputManager::MoveVert() {
	bool forward = GetKeyDown(GLFW_KEY_W);
	bool backward = GetKeyDown(GLFW_KEY_S);
	return forward && !backward ? 1.0f : backward && !forward ? -1.0f : 0.0f;
}
float InputManager::MoveHorz() {
	bool forward = GetKeyDown(GLFW_KEY_D);
	bool backward = GetKeyDown(GLFW_KEY_A);
	return forward && !backward ? 1.0f : backward && !forward ? -1.0f : 0.0f;
}
float InputManager::MoveUpDown() {

	bool forward = GetKeyDown(GLFW_KEY_E);
	bool backward = GetKeyDown(GLFW_KEY_Q);
	return forward && !backward ? 1.0f : backward && !forward ? -1.0f : 0.0f;
}
bool InputManager::IsMovingAny() {
	return MoveHorz() != 0.0f || MoveVert() != 0.0f || MoveUpDown() != 0.0f;
}
#pragma endregion
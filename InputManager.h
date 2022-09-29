#pragma once
#include "Mouse.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
	static Mouse mouse;
	static GLFWwindow* window;
	static bool inputLocked;

	static void Initialize();
	static void SetMouseUnlocked();
	static void SetMouseLocked();

#pragma region Input

	static bool GetKeyDown(int key);
	static bool GetKeyRelease(int key);
	static float MoveVert();
	static float MoveHorz();
	static float MoveUpDown();
	static bool IsMovingAny();
#pragma endregion
};


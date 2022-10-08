#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Chunk.h"
#include "Hud.h"
#include "Player.h"
#include "Terrain.h"
#include <vector>
#include <thread>
#include "ItemDataManager.h"
#include "InputManager.h"
#include "Time.h"
#include "Scene.h"
#include "Mouse.h"
#include "World.h"
#include <WinSock2.h>
#include <NetworkManager.h>
#include "ClientManifestManager.h"

GLFWwindow* window;
std::thread gameTick;


// Scenes
World *world = new World();

// Starting Scenes
Scene *activeScene;

const float TICK_FREQUENCY = 50.0f;

const int WIDTH = 800;
const int HEIGHT = 800;

float lastActionTime = 0.0f;

Syncvar syncVar;
GLFWwindow* CreateDisplay(const char* title, int width, int height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	return window;
}
void ProcessInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (InputManager::GetKeyDown(GLFW_KEY_F4) && glfwGetTime() - lastActionTime > 0.25f) {
		lastActionTime = glfwGetTime();
		std::cout << "started connecting" << std::endl;
		ShibaNetLib::NetworkManager::StartClient();
		ShibaNetLib::Network::conn.showDebugMessages = true;
	}
	if (InputManager::GetKeyDown(GLFW_KEY_F5) && glfwGetTime() - lastActionTime > 0.25f) {
		lastActionTime = glfwGetTime();
		std::cout << "started hosting" << std::endl;
		ShibaNetLib::NetworkManager::StartHost();
		ShibaNetLib::Network::conn.showDebugMessages = true;
	}
	if (InputManager::GetKeyDown(GLFW_KEY_F3) && glfwGetTime() - lastActionTime > 0.25f) {
		Terrain::chunks.clear();
	}
}
void Tick() {
	float lastTickTime = 0.0f;
	
	// ticks per second
	while (true) {
		if (glfwGetTime() - lastTickTime >= 1.0f / TICK_FREQUENCY) {
			lastTickTime = glfwGetTime();

			activeScene->FixedUpdate();
		//	player.SetGrounded(terrain.GetPositionValue(player.GetPosition() + dirdown * player.playerHeight) != 0);

		//	glm::vec3 playerNextFramePos = player.GetFeetPosition() + PhysicsExtensions::RemoveY(player.velocity) * Time::fixedDeltaTime;


		//	if (terrain.GetPositionValue(playerNextFramePos) != 0)
			//	player.Collision();

		}
	}
}
void Update() {
	while (!glfwWindowShouldClose(window)) {
		Time::Update();
			
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput();
		activeScene->Update();

		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	gameTick.detach();
}
void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {
	InputManager::mouse.MouseCallback(window, xpos, ypos);
	activeScene->HandleMouseInput(window, xpos, ypos);
}
void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mod) {
	activeScene->HandleKeyPress(window, key, scancode, action, mod);
}
void ChangeScenes(Scene* newScene) {
	activeScene = newScene;
	activeScene->Start();
}

int	main() {
	glfwInit();
	window = CreateDisplay("opengl", WIDTH, HEIGHT);
	InputManager::window = window;
	glewInit();

	ShibaNetLib::NetworkManager::Initialize();

	activeScene = world;
	activeScene->Start();

	Time::fixedDeltaTime = 1.0f / TICK_FREQUENCY;
	srand(time(NULL));
	gameTick = std::thread(Tick);

	BlockDataManager::InitializeBlockDefs();
	ItemDataManager::InitializeItemDefs();
	InputManager::Initialize();

	syncVar = Syncvar(5, 0, 1, &syncVar);

	glfwSetCursorPosCallback(window, HandleMouseInput);
	glfwSetKeyCallback(window, HandleKeyPress);

	glEnable(GL_DEPTH_TEST);
	Update();
}
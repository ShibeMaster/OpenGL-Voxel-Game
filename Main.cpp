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
#include "CustomDataManager.h"

glm::vec3 positions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


const char* vertexSource = R"GLSL(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	Color = color;
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)GLSL";

const char* hudVertexSource = R"GLSL(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 Color;
uniform mat4 model;


void main(){
	Color = color;
	gl_Position = model * vec4(pos, 1.0f);
}
)GLSL";

const char* fragmentSource = R"GLSL(
#version 330 core

in vec4 Color;

out vec4 FragColor;

void main(){
	FragColor = Color;
}
)GLSL";



GLFWwindow* window;
Renderer renderer;
Hud hud;
Terrain terrain;
std::vector<GameObject> gameObjects;
Player player;
std::thread updateChunks;

float timeSinceLastPlaced = 0.0f;
float timeSinceLastSwappedMode = 0.0f;

float lastFrameTime, deltaTime = 0.0f;

const int WIDTH = 800;
const int HEIGHT = 800;

GLFWwindow* CreateWindow(const char* title, int width, int height) {
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

	if (glfwGetKey(window, GLFW_KEY_C) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
		player.placing = !player.placing;
		timeSinceLastPlaced = glfwGetTime();
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) && glfwGetTime() - timeSinceLastSwappedMode >= 0.25f) {
		if (player.placing) terrain.CreateBlock(player.GetPosition(), player.selectedPosition, BlockDataManager::GetWood());
		else terrain.DestroyBlock(player.GetPosition(), player.selectedPosition);
		timeSinceLastSwappedMode = glfwGetTime();
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		GameData data;
		CustomDataManager::CreateGameData(data);

	}
}
void Update() {
	while (!glfwWindowShouldClose(window)) {
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);

		glm::mat4 view = player.camera.GetViewMatrix();
		renderer.shader.SetMat4("view", view);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput();

		terrain.UpdateRenderedChunks(player.GetPosition());
		terrain.RenderChunks(renderer);

		hud.Render();

		player.HandleInput(window, deltaTime);

		// if (terrain.GetPositionValue(player.GetPosition() + player.velocity).data.type == 0) 
		player.Move(deltaTime);

		glm::vec3 playerSelectedPosition = terrain.GetSelectedBlock(player.GetPosition(), player.camera.forward, player.placing);
		if (playerSelectedPosition != player.GetPosition()) player.SetSelectedBlock(playerSelectedPosition);
		else player.UnselectBlock();

		player.Update(window, deltaTime, renderer);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}
void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {
	player.camera.ProcessCameraMouse(window, xpos, ypos);
}
void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mod) {
	if ((key >= GLFW_KEY_1 && key <= GLFW_KEY_9) || key == GLFW_KEY_0) {
		player.inventory.selectedIndex = key == GLFW_KEY_0 ? 9 : key - 49;
		hud.InventoryUpdated();
	}
}
int main() {
	glfwInit();
	window = CreateWindow("opengl", WIDTH, HEIGHT);
	glewInit();

	srand(time(NULL));

	renderer.Initialize(vertexSource, fragmentSource);

	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 800.0f, 0.1f, 100.0f);
	renderer.shader.SetMat4("projection", projection);
	
	CustomDataManager::InitializeFilesystem();
	hud = Hud(hudVertexSource, fragmentSource, &player.inventory);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, HandleMouseInput);
	glfwSetKeyCallback(window, HandleKeyPress);

	player.InitializePlayer();

	glEnable(GL_DEPTH_TEST);
	Update();
}
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
#include "Mouse.h"


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
Player player;
std::thread chunkGeneration;
std::thread gameTick;

float timeSinceLastPlaced = 0.0f;
float timeSinceLastSwappedMode = 0.0f;

const float TICK_FREQUENCY = 50.0f;

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

	if (InputManager::GetKeyDown(GLFW_KEY_TAB) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
		hud.ToggleInventory();
		timeSinceLastPlaced = glfwGetTime();
	}
	if (glfwGetKey(window, GLFW_KEY_C) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
		player.placing = !player.placing;
		timeSinceLastPlaced = glfwGetTime();
	}
	if (glfwGetKey(window, GLFW_KEY_F3) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
		glm::vec2 chunkPos = terrain.GetChunkPosition(player.GetPosition());
		std::cout << chunkPos.x << " | " << chunkPos.y << std::endl;
		timeSinceLastPlaced = glfwGetTime();
	}

	if (InputManager::GetKeyDown(GLFW_KEY_F5) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
		glm::vec3 playerPos = player.GetFeetPosition() + PhysicsExtensions::RemoveY(player.velocity) * Time::fixedDeltaTime;
		glm::vec3 playerLocalPos = terrain.GetLocalPosition(playerPos);
		std::cout << playerLocalPos.x << " | " << playerLocalPos.y << " | " << playerLocalPos.z << " || " << playerPos.x << " | " << playerPos.y << " | " << playerPos.z << std::endl;
	}

	if (InputManager::GetKeyDown(GLFW_KEY_F4) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
		player.inventory.TryAddItem(ItemType::item_testItem1);
		timeSinceLastPlaced = glfwGetTime();
	}
	if (InputManager::GetKeyDown(GLFW_KEY_SPACE) && glfwGetTime() - timeSinceLastSwappedMode >= 0.25f && !hud.menuOpen) {
		if (player.placing && player.inventory.inventory[player.inventory.selectedIndex].data.usage == ItemUsageType::usage_placeable) {
			terrain.CreateBlock(player.selectedPosition, player.inventory.inventory[player.inventory.selectedIndex].data.type);
			player.inventory.TryRemoveItem(player.inventory.selectedIndex);
		}
		else if (player.inventory.inventory[player.inventory.selectedIndex].data.usage == ItemUsageType::usage_consumable) {}
		else {
			player.inventory.TryAddItem(BlockDataManager::GetBlockData((BlockType)terrain.GetPositionValue(player.selectedPosition))->item);
			terrain.DestroyBlock(player.selectedPosition);
		}
		timeSinceLastSwappedMode = glfwGetTime();
	}
}
void Tick() {
	float lastTickTime = 0.0f;
	
	// ticks per second
	while (true) {
		if (glfwGetTime() - lastTickTime >= 1.0f / TICK_FREQUENCY) {
			lastTickTime = glfwGetTime();
	//		player.SetGrounded(terrain.GetPositionValue(player.GetPosition() + dirdown * player.playerHeight) != 0);

			player.Update();
//			glm::vec3 playerNextFramePos = player.GetFeetPosition() + PhysicsExtensions::RemoveY(player.velocity) * Time::fixedDeltaTime;


		//	if (terrain.GetPositionValue(playerNextFramePos) != 0)
			//	player.Collision();
			player.Move();

		}
	}
}
void Update() {
	while (!glfwWindowShouldClose(window)) {
		Time::Update();

		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);

		glm::mat4 view = player.camera.GetViewMatrix();
		renderer.shader.SetMat4("view", view);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput();

		terrain.MeshGeneration();
		terrain.UpdateRenderedChunks(player.GetPosition());
		terrain.RenderChunks(renderer);

		if (player.hasBlockSelected) player.RenderSelectedBlock(renderer);
		hud.Render();

		glm::vec3 playerSelectedPosition = terrain.GetSelectedBlock(player.GetPosition(), player.camera.forward, player.placing);
		if (playerSelectedPosition != player.GetPosition()) player.SetSelectedBlock(playerSelectedPosition);
		else player.UnselectBlock();



		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	chunkGeneration.detach();
	gameTick.detach();
}
void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {
	InputManager::mouse.MouseCallback(window, xpos, ypos);
	player.camera.ProcessCameraMouse();
}
void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mod) {
	if ((key >= GLFW_KEY_1 && key <= GLFW_KEY_9) || key == GLFW_KEY_0) {
		player.inventory.selectedIndex = key == GLFW_KEY_0 ? 9 : key - 49;
		hud.InventoryUpdated();
	}
}
void ChunkGenerationThread() {
	terrain.GenerationThread();
}
int main() {
	glfwInit();
	window = CreateWindow("opengl", WIDTH, HEIGHT);
	InputManager::window = window;
	glewInit();

	Time::fixedDeltaTime = 1.0f / TICK_FREQUENCY;
	srand(time(NULL));

	renderer.Initialize(vertexSource, fragmentSource);

	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 800.0f, 0.1f, 100.0f);
	renderer.shader.SetMat4("projection", projection);
	
	hud = Hud(hudVertexSource, fragmentSource, &player.inventory);

	BlockDataManager::InitializeBlockDefs();
	ItemDataManager::InitializeItemDefs();
	terrain.biomeManager.InitializeBiomeDefs();

	player.InitializePlayer();

	chunkGeneration = std::thread(ChunkGenerationThread);
	gameTick = std::thread(Tick);
	terrain.UpdateRenderedChunks(player.GetPosition());

	InputManager::Initialize();

	glfwSetCursorPosCallback(window, HandleMouseInput);
	glfwSetKeyCallback(window, HandleKeyPress);

	glEnable(GL_DEPTH_TEST);
	Update();
}
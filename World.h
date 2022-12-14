#pragma once
#include "Scene.h"
#include "Renderer.h"
#include "Hud.h"
#include "Terrain.h"
#include "Player.h"
#include <thread>
#include "Shaders.h"
#include <iostream>
#include "Model.h"
#include "GUIButton.h"
#include "GUIItem.h"
#include "GUIPercentageBar.h"
#include "GUICheckbox.h"
#include "GUISlider.h"
#include "NetworkedPlayersManager.h"

class World : public Scene
{
public:
	Renderer renderer;
	Hud hud;
	Terrain terrain;
	Player player;
	std::thread chunkGeneration;

	ManifestUpdateChannel updateChannel = ManifestUpdateChannel(3);
	ManifestRequestChannel requestChannel = ManifestRequestChannel(4);

	ChunkRequestChannel chunkRequestChannel = ChunkRequestChannel(5);
	ChunkRequestCallbackChannel chunkRequestCallbackChannel = ChunkRequestCallbackChannel(6);

	// need to move these to a playermodule or something
	float timeSinceLastPlaced = 0.0f;
	float timeSinceLastSwappedMode = 0.0f;




	GUIText coordinateDisplay;
	GUIText velocityDisplay;

	World() {
		this->sceneId = 0;
		std::cout << "started scene" << std::endl;

	}
	void Start() {
		renderer.Initialize(Shaders::vertexSource, Shaders::fragmentSource);

		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 800.0f, 0.1f, 100.0f);
		renderer.shader.SetMat4("projection", projection);

		hud = Hud(Shaders::hudVertexSource, Shaders::fragmentSource, &player.data.inventory);

		terrain.biomeManager.InitializeBiomeDefs();

		player.Initialize();
		NetworkedPlayersManager::Initialize();

		std::cout << "started" << std::endl;
		chunkGeneration = std::thread([this] { this->terrain.GenerationThread(); });
		
		terrain.UpdateRenderedChunks(player.data.camera.position);
		std::cout << player.GetPosition().x << " | " << player.GetPosition().z << std::endl;
		ShibaNetLib::NetworkChannelManager::AddNetworkChannel(&updateChannel);
		ShibaNetLib::NetworkChannelManager::AddNetworkChannel(&requestChannel);

		ShibaNetLib::NetworkChannelManager::AddNetworkChannel(&chunkRequestChannel);
		ShibaNetLib::NetworkChannelManager::AddNetworkChannel(&chunkRequestCallbackChannel);


		coordinateDisplay = GUIText("Position: " + std::to_string(player.data.camera.position.x) + " | " + std::to_string(player.data.camera.position.y) + " | " + std::to_string(player.data.camera.position.z), glm::vec2(50, 750), 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), GUIAlignment::alignment_right);
		velocityDisplay = GUIText("Velocity: " + std::to_string(player.data.physics.velocity.x) + " | " + std::to_string(player.data.physics.velocity.y) + " | " + std::to_string(player.data.physics.velocity.z), glm::vec2(50, 700), 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), GUIAlignment::alignment_right);

		
		started = true;
	}
	
	void ProcessInput() {
		if (InputManager::GetKeyDown(GLFW_KEY_TAB) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
			hud.ToggleInventory();
			timeSinceLastPlaced = glfwGetTime();
		}
		if (InputManager::GetKeyDown(GLFW_KEY_C) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
			player.state.placingState = player.state.placingState == PlacingState::placingstate_breaking ? PlacingState::placingstate_placing : PlacingState::placingstate_breaking;		timeSinceLastPlaced = glfwGetTime();
		}
		if (InputManager::GetKeyDown(GLFW_KEY_F3) && glfwGetTime() - timeSinceLastPlaced >= 0.25f) {
			player.data.camera.position.y = 100.0f;
			player.data.camera.position.x = 0.0f;
			player.data.camera.position.z = 0.0f;
			std::cout << player.data.camera.position.x << " | " << player.data.camera.position.y << " | " << player.data.camera.position.z << std::endl;
			timeSinceLastPlaced = glfwGetTime();
		}
		if (InputManager::GetKeyDown(GLFW_KEY_SPACE) && glfwGetTime() - timeSinceLastSwappedMode >= 0.25f && !hud.menuOpen) {
			if (player.data.inventory.inventory[player.data.inventory.selectedIndex].data.usage == ItemUsageType::usage_consumable) {}
			else if (player.modules.placing.hasBlockSelected) {

				if (player.state.placingState == placingstate_placing && player.data.inventory.inventory[player.data.inventory.selectedIndex].data.usage == ItemUsageType::usage_placeable) {
					terrain.CreateBlock(player.modules.placing.selectedPosition, player.data.inventory.inventory[player.data.inventory.selectedIndex].data.type);
					player.data.inventory.TryRemoveItem(player.data.inventory.selectedIndex);
				}
				else {
					player.data.inventory.TryAddItem(BlockDataManager::GetBlockData((BlockType)terrain.GetPositionValue(player.modules.placing.selectedPosition))->item);
					terrain.DestroyBlock(player.modules.placing.selectedPosition);
				}
			}
			timeSinceLastSwappedMode = glfwGetTime();
		}
	}
	void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {
		player.data.camera.ProcessCameraMouse();
	}
	void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mod) {
		if ((key >= GLFW_KEY_1 && key <= GLFW_KEY_9) || key == GLFW_KEY_0) {
			player.data.inventory.selectedIndex = key == GLFW_KEY_0 ? 9 : key - 49;
			hud.InventoryUpdated();
		}
	}

	void FixedUpdate() {
		player.FixedUpdate();
	}
	void Update() {
		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);

		glm::mat4 view = player.data.camera.GetViewMatrix();
		renderer.shader.SetMat4("view", view);

		player.Update();

		ProcessInput();
		
		terrain.MeshGeneration();
		terrain.UpdateRenderedChunks(player.GetPosition());
		terrain.RenderChunks(renderer);

		coordinateDisplay.text = "Position: " + std::to_string(player.data.camera.position.x) + " | " + std::to_string(player.data.camera.position.y) + " | " + std::to_string(player.data.camera.position.z);
		velocityDisplay.text = "Velocity: " + std::to_string(player.data.physics.velocity.x) + " | " + std::to_string(player.data.physics.velocity.y) + " | " + std::to_string(player.data.physics.velocity.z);

		coordinateDisplay.Render();
		velocityDisplay.Render();

		NetworkedPlayersManager::Render(renderer);

		if(player.modules.placing.hasBlockSelected) player.modules.placing.Render(renderer);
		hud.Render();

		glm::vec3 playerSelectedPosition = terrain.GetSelectedBlock(player.GetPosition(), player.data.camera.forward, player.state.placingState == PlacingState::placingstate_placing);
		if (playerSelectedPosition != player.GetPosition()) player.modules.placing.SetSelectedBlock(playerSelectedPosition);
		else player.modules.placing.UnselecteBlock();
	}
	void End() {
		chunkGeneration.detach();
	}
};


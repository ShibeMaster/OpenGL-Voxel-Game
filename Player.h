#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Chunk.h"
#include "Inventory.h"

class Player
{
public:
	Camera camera;
	glm::vec3 selectedPosition;
	bool hasBlockSelected = false;
	bool placing = false;
	bool grounded = false;
	const int SELECTION_DISTANCE = 8;
	Mesh selectedBlockMesh;
	Inventory inventory;
	float fallingSpeed;
	glm::vec3 velocity;
	Player();
	void Update(GLFWwindow* window, float deltaTime, Renderer& renderer);
	void InitializePlayer();
	void HandleInput(GLFWwindow* window, float deltaTime);
	void SetSelectedBlock(glm::vec3 position);
	void UnselectBlock();
	void RenderSelectedBlock(Renderer& renderer);
	void SetGrounded(bool value);
	void ApplyGravity(float deltaTime);
	void Jump();
	void Move(float deltaTime);
	glm::vec3 GetPosition();
};
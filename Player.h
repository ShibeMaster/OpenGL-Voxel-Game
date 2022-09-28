#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Chunk.h"
#include "Time.h"
#include "Inventory.h"
#include "PhysicsExtensions.h"

class Player
{
public:
	Camera camera;
	glm::vec3 selectedPosition;
	bool hasBlockSelected = false;
	bool placing = false;
	bool grounded = false;
	const int SELECTION_DISTANCE = 8;
	float speed = 5.0f;
	const float jumpForce = 5.0f;
	const float playerHeight = 1.75f;
	float lastJumpTime = 0.0f;
	const float jumpCooldown = 0.25f;

	Mesh selectedBlockMesh;
	Inventory inventory;
	float fallingSpeed;
	glm::vec3 velocity;
	glm::vec3 direction;
	Player();
	void Update();
	void InitializePlayer();
	void HandleInput();
	void SetSelectedBlock(glm::vec3 position);
	void UnselectBlock();
	void RenderSelectedBlock(Renderer& renderer);
	void SetGrounded(bool value);
	void Collision();
	void Jump();
	void Move();
	glm::vec3 GetPosition();
	glm::vec3 GetFeetPosition();
};
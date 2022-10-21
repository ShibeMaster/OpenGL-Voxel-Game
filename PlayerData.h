#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Inventory.h"
#include "PhysicsObject.h"
#include "PlayerState.h"

class PlayerData
{
public:
	PlayerState* state;
	PhysicsObject physics;

	Camera camera;
	glm::vec3 velocity;
	glm::vec3 direction;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;


	Inventory inventory;

#pragma region Stats
	int selectionDistance = 8;
	float speed = 2.0f;
	float jumpForce = 5.0f;
	float height = 1.75f;

#pragma endregion

	void Initialize(PlayerState* state);
	void Update();
	void FixedUpdate();
	void UpdateDirections();
};


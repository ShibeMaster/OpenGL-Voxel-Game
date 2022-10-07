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

#include "ClientManifestManager.h"

#include "PlayerState.h"
#include "PlayerData.h"
#include "PlayerModules.h"
#include "PhysicsCache.h"

class Player
{
public:
	PlayerState state;
	PlayerData data;
	PlayerModules modules;
	PhysicsCache physicsCache;


	float lastJumpTime = 0.0f;
	const float jumpCooldown = 0.25f;

	float fallingSpeed;

	Player();
	void Update();
	void FixedUpdate();
	void Initialize();
	void SetGrounded(bool value);
	void Collision();
	void Jump();
	glm::vec3 GetPosition();
	glm::vec3 GetFeetPosition();
};
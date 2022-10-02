#pragma once
#include <glm/glm.hpp>
#include "PlayerData.h"
class PhysicsCache
{
public:
	glm::vec3 lastFramePosition;
	glm::vec3 lastFrameVelocity;

	void CacheVariables(PlayerData* data) {
		lastFramePosition = data->camera.position;
		lastFrameVelocity = data->velocity;
	}
};


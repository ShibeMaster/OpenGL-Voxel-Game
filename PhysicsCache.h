#pragma once
#include <glm/glm.hpp>
class PhysicsCache
{
public:
	glm::vec3 lastFramePosition;
	glm::vec3 lastFrameVelocity;

	void CacheVariables(glm::vec3 position, glm::vec3 velocity) {
		lastFramePosition = position;
		lastFrameVelocity = velocity;
	}
};


#pragma once
#include <glm/glm.hpp>
#include "Time.h"

const float drag = 5.0f;
const glm::vec3 gravityDirection = glm::vec3(0.0f, -1.0f, 0.0f);
const float gravity = 9.98f;

class PhysicsExtensions
{
public:
	static glm::vec3 RemoveY(glm::vec3 value);
	static glm::vec3 AddImpulseForce(glm::vec3 vec, glm::vec3 force);
	static glm::vec3 ApplyDrag(glm::vec3 vec);
	static glm::vec3 ApplyGravity(glm::vec3 velocity);
	static float Magnitude(glm::vec3 vec);
};


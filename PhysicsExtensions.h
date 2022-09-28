#pragma once
#include <glm/glm.hpp>
#include "Time.h"

const float drag = 5.0f;
const glm::vec3 gravityDirection = glm::vec3(0.0f, -1.0f, 0.0f);
const float gravity = 9.98f;

class PhysicsExtensions
{
public:
	static glm::vec3 RemoveY(glm::vec3 value) {
		return glm::vec3(value.x, 0.0f, value.z);
	}
	static glm::vec3 AddImpulseForce(glm::vec3 vec, glm::vec3 force) {
		return vec + force;
	}
	static glm::vec3 ApplyDrag(glm::vec3 vec) {
		float multiplier = 1.0f - drag * Time::fixedDeltaTime;
		if (multiplier < 0.0f) multiplier = 0.0f;
		glm::vec3 newVec = vec * multiplier;
		newVec.y = vec.y;
		return newVec;
	}
	static glm::vec3 ApplyGravity(glm::vec3 velocity) {
		return velocity + gravityDirection * gravity * Time::fixedDeltaTime;
	}
	static float Magnitude(glm::vec3 vec) {
		return sqrt(pow(vec.x, 2) + pow(vec.y,2) + pow(vec.z,2));
	}
};


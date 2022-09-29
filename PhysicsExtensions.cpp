#include "PhysicsExtensions.h"

glm::vec3 PhysicsExtensions::RemoveY(glm::vec3 value) {
	return glm::vec3(value.x, 0.0f, value.z);
}
glm::vec3 PhysicsExtensions::AddImpulseForce(glm::vec3 vec, glm::vec3 force) {
	return vec + force;
}
glm::vec3 PhysicsExtensions::ApplyDrag(glm::vec3 vec) {
	float multiplier = 1.0f - drag * Time::fixedDeltaTime;
	if (multiplier < 0.0f) multiplier = 0.0f;
	glm::vec3 newVec = vec * multiplier;	
	return newVec;
}
glm::vec3 PhysicsExtensions::ApplyGravity(glm::vec3 velocity) {
	return velocity + gravityDirection * gravity * Time::fixedDeltaTime;
}
float PhysicsExtensions::Magnitude(glm::vec3 vec) {
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}
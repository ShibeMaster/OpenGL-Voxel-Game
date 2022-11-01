#include "PhysicsExtensions.h"

glm::vec3 PhysicsExtensions::RemoveY(glm::vec3 value) {
	return glm::vec3(value.x, 0.0f, value.z);
}
glm::vec3 PhysicsExtensions::AddImpulseForce(glm::vec3 vec, glm::vec3 force) {
	return vec + force;
}
glm::vec3 PhysicsExtensions::ApplyDrag(glm::vec3 vec) {
	// glm::vec3 vector = RemoveY(vec);
	glm::vec3 vector = vec;
	float multiplier = 1.0f - drag * Time::deltaTime;
	if (multiplier < 0.0f) multiplier = 0.0f;
	vector *= multiplier;
	// vector.y = vec.y;
	return vector;
}
glm::vec3 PhysicsExtensions::ApplyGravity(glm::vec3 velocity) {
	return velocity + (gravityDirection * gravity * Time::deltaTime);
}
float PhysicsExtensions::Magnitude(glm::vec3 vec) {
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}
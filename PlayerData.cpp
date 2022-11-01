#include "PlayerData.h"

void PlayerData::Initialize(PlayerState* state) {
	camera.position = glm::vec3(0.0f, 64.0f, 0.0f);
	this->state = state;
	this->physics = PhysicsObject(true, true, true, speed, glm::vec3(0.0f, 0.0f, 0.0f), height, 0.75f);
}
void PlayerData::Update() {
	UpdateDirections();
	if (physics.initialized)
		camera.position = physics.PhysicsUpdate(camera.position);
}
void PlayerData::FixedUpdate() {
}
void PlayerData::UpdateDirections() {
	forward = camera.forward;
	right = camera.right;
	up = camera.up;
}
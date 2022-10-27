#include "PlayerData.h"

void PlayerData::Initialize(PlayerState* state) {
	camera.position = glm::vec3(0.0f, 64.0f, 0.0f);
	this->state = state;
	this->physics = PhysicsObject(false, true, true, glm::vec3(0.0f, 0.0f, 0.0f), height, 0.5f);
}
void PlayerData::Update() {
	UpdateDirections();
}
void PlayerData::FixedUpdate() {
	if(physics.initialized)
		camera.position = physics.PhysicsFixedUpdate(camera.position);
}
void PlayerData::UpdateDirections() {
	forward = camera.forward;
	right = camera.right;
	up = camera.up;
}
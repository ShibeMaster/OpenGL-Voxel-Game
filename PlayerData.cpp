#include "PlayerData.h"

void PlayerData::Initialize(PlayerState* state) {
	this->state = state;
	this->physics = PhysicsObject(true, true, true, glm::vec3(0.0f, 0.0f, 0.0f), height, 0.5f);
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
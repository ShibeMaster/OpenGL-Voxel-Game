#include "PlayerData.h"

void PlayerData::Initialize(PlayerState* state) {
	this->state = state;
}
void PlayerData::Update() {
	UpdateDirections();
}
void PlayerData::FixedUpdate() {}
void PlayerData::UpdateDirections() {
	forward = camera.forward;
	right = camera.right;
	up = camera.up;
}
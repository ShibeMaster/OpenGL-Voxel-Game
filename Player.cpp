#include "Player.h"

Player::Player() {
	data.camera = Camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
}
void Player::Initialize() {
	modules.Initialize(&data);
}
void Player::Update() {
	data.Update();
	modules.Update();
}
void Player::FixedUpdate() {
	data.FixedUpdate();
	modules.FixedUpdate();
}

glm::vec3 Player::GetPosition() {
	return data.camera.position;
}
void Player::SetGrounded(bool value) {
	if (!state.grounded && value)
		data.velocity.y = 0.0f;

	state.grounded = value;
}
void Player::Jump() {
	data.velocity = PhysicsExtensions::AddImpulseForce(data.velocity, dirup * data.jumpForce);
	lastJumpTime = glfwGetTime();
	std::cout << "jumping" << std::endl;
}
void Player::Collision() {
	data.velocity = -data.velocity;
	std::cout << "collided" << std::endl;
}
glm::vec3 Player::GetFeetPosition() {
	glm::vec3 feetPos = GetPosition();
	feetPos += dirdown * (data.height - 0.15f);
	return feetPos;
}
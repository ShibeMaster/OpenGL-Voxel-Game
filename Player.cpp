#include "Player.h"

Player::Player() {
	data.camera = Camera(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
}
void Player::Initialize() {
	data.Initialize(&state);
	modules.Initialize(&data);
}
void Player::Update() {
	data.Update();
	modules.Update();
	if (((ShibaNetLib::Network::conn.isServer && ShibaNetLib::Network::clientCount > 0) || !ShibaNetLib::Network::conn.isServer) && ShibaNetLib::Network::state == ShibaNetLib::NetworkState::netstate_connected) {
		if (NetworkInfo::ClientExists(ShibaNetLib::Network::conn.netId)) {
			ClientObject* self = NetworkInfo::GetClient(ShibaNetLib::Network::conn.netId);
			if (glm::ivec3(self->playerData.position) != glm::ivec3(data.camera.position)) {
				ClientObject* client = NetworkInfo::GetClient(ShibaNetLib::Network::conn.netId);
				client->playerData.position = data.camera.position;
				client->playerData.rotation = data.camera.forward;
				client->playerData.color = data.inventory.inventory[data.inventory.selectedIndex].data.color;
				ClientManifestManager::PostManifestUpdateRequest(*client);
			};
		}
		else {
			ClientObject client;
			client.data.netid = ShibaNetLib::Network::conn.netId;
			client.playerData.position = data.camera.position;
			client.playerData.rotation = data.camera.forward;
			client.playerData.color = data.inventory.inventory[data.inventory.selectedIndex].data.color;
			NetworkInfo::clientManifest.push_back(client);
			ClientManifestManager::PostManifestUpdateRequest(client);
		}
	}

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
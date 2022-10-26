#include "MovementModule.h"

void Modules::MovementModule::Update() {
	ManageInput();
}

void Modules::MovementModule::ManageInput() {

	if (InputManager::IsMovingAny()) {
		glm::vec3 newDir = glm::vec3(0.0f);
		newDir += PhysicsExtensions::RemoveY(data->forward) * InputManager::MoveVert();
		newDir += PhysicsExtensions::RemoveY(data->right) * InputManager::MoveHorz();
		newDir += data->up * InputManager::MoveUpDown();
		data->direction = newDir;


		data->physics.velocity += (data->direction * data->speed);
	}

	if (data->physics.grounded && InputManager::GetKeyDown(GLFW_KEY_E)) {
		data->physics.velocity.y += data->jumpForce;
		data->physics.grounded = false;
	}

}
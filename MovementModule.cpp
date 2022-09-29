#include "MovementModule.h"

void Modules::MovementModule::Update() {
	ManageInput();
	ApplyVelocity();
}

void Modules::MovementModule::ManageInput() {

	if (InputManager::IsMovingAny()) {
		glm::vec3 newDir = glm::vec3(0.0f);
		newDir += data->forward * InputManager::MoveVert();
		newDir += data->right * InputManager::MoveHorz();
		newDir += data->camera.worldUp * InputManager::MoveUpDown();
		data->direction = newDir;

		data->velocity += (data->direction * data->speed);
	}
}

void Modules::MovementModule::ApplyVelocity() {
	data->velocity = PhysicsExtensions::ApplyDrag(data->velocity);
	data->camera.position += data->velocity * Time::fixedDeltaTime;
}
#pragma once
#include "Camera.h"
#include "PhysicsObject.h"
class SpectatorCamera {
public:
	const float speed = 5.0f;
	Camera camera = Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	PhysicsObject physics = PhysicsObject(false, false, true);

	void FixedUpdate() {
		glm::vec3 newDir = glm::vec3(0.0f);
		newDir += camera.forward * InputManager::MoveVert();
		newDir += camera.right * InputManager::MoveHorz();
		newDir += camera.up * InputManager::MoveUpDown();

		physics.velocity += (newDir * speed);

		camera.position = physics.PhysicsFixedUpdate(camera.position);
	}
};
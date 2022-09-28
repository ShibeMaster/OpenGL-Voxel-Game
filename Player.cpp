#include "Player.h"

Player::Player() {
	camera = Camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
}
void Player::InitializePlayer() {

	std::vector<Vertex> cubevertices;
	for (int i = 0; i < 36*3; i += 3) {
		Vertex vertex;
		vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
		vertex.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		cubevertices.push_back(vertex);
	}
	selectedBlockMesh = Mesh(cubevertices);
	camera.position.y = 15.0f;
}
void Player::Update() {
	HandleInput();

	// if (!grounded)
		// velocity = PhysicsExtensions::ApplyGravity(velocity);

	// velocity = PhysicsExtensions::ApplyDrag(velocity);
}

void Player::Move() {
	camera.position += velocity * Time::fixedDeltaTime;
}

void Player::HandleInput() {
	if (InputManager::GetKeyDown(GLFW_KEY_F1)) {
		camera.position.y = 32.0f;
	}


//	if (InputManager::GetKeyDown(GLFW_KEY_E) && grounded && glfwGetTime() - lastJumpTime > jumpCooldown)
	//	Jump();

	if (InputManager::IsMovingAny()) {
		glm::vec3 newDir = glm::vec3(0.0f);
		newDir += camera.forward * InputManager::MoveVert();
		newDir += camera.right * InputManager::MoveHorz();
		newDir += camera.worldUp * InputManager::MoveUpDown();
		direction = newDir;

		camera.position += (direction * speed * Time::fixedDeltaTime);
//		velocity += (direction * speed);

	}

}

glm::vec3 Player::GetPosition() {
	return camera.position;
}
void Player::RenderSelectedBlock(Renderer& renderer) {
	renderer.shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, selectedPosition);
	
	renderer.shader.SetMat4("model", model);

	selectedBlockMesh.DrawMesh();
}
void Player::SetSelectedBlock(glm::vec3 position) {
	selectedPosition = position;
	hasBlockSelected = true;
}
void Player::SetGrounded(bool value) {
	if (!grounded && value)
		velocity.y = 0.0f;

	grounded = value;
}
void Player::Jump() {
	velocity = PhysicsExtensions::AddImpulseForce(velocity, dirup * jumpForce);
	lastJumpTime = glfwGetTime();
	std::cout << "jumping" << std::endl;
}
void Player::UnselectBlock() {
	hasBlockSelected = false;
}
void Player::Collision() {
	velocity = -velocity;
	std::cout << "collided" << std::endl;
}
glm::vec3 Player::GetFeetPosition() {
	glm::vec3 feetPos = GetPosition();
	feetPos += dirdown * (playerHeight - 0.15f);
	return feetPos;
}
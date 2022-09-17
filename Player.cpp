#include "Player.h"
#include "Camera.h"

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
void Player::Update(GLFWwindow* window, float deltaTime, Renderer& renderer) {
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	if (hasBlockSelected) RenderSelectedBlock(renderer);
}

void Player::HandleInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		velocity += camera.forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		velocity -= camera.forward;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		velocity += camera.right;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		velocity -= camera.right;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		velocity -= camera.up;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		velocity += camera.up;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		camera.position.y = 15.0f;

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
	if (grounded && !value)
		fallingSpeed = 0.0f;

	grounded = value;
}
void Player::ApplyGravity(float deltaTime) {
	if (!grounded) {
		fallingSpeed += 9.98f * deltaTime;
		camera.position += dirdown * fallingSpeed * deltaTime;
	}
}
void Player::Jump() {
	if (grounded) {
		camera.position.y += 1.2f;
	}
}
void Player::UnselectBlock() {
	hasBlockSelected = false;
}
void Player::Move(float deltaTime) {
	camera.position += velocity * camera.speed * deltaTime;
}
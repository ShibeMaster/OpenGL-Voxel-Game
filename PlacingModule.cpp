#include "PlacingModule.h"
void Modules::PlacingModule::Initialize(PlayerData* data) {
	PlayerModule::Initialize(data);
}
void Modules::PlacingModule::Render(Renderer& renderer) {
	renderer.shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, selectedPosition);

	renderer.shader.SetMat4("model", model);

	selectedBlockMesh.DrawMesh();
}
void Modules::PlacingModule::UnselecteBlock() {
	hasBlockSelected = false;
}
void Modules::PlacingModule::SetSelectedBlock(glm::vec3 position) {
	selectedPosition = position;
	hasBlockSelected = true;
}
void Modules::PlacingModule::SetupMesh() {
	std::vector<Vertex> cubevertices;
	for (int i = 0; i < 36 * 3; i += 3) {
		Vertex vertex;
		vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
		vertex.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		cubevertices.push_back(vertex);
	}

	selectedBlockMesh = Mesh(cubevertices);
}
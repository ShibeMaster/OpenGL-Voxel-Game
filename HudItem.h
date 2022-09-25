#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Renderer.h"
#include "Mesh.h"

class HudItem
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	Mesh mesh;

	HudItem() {}
	HudItem(std::vector<Vertex> vertices, glm::vec3 position, glm::vec3 scale) {
		mesh = Mesh(vertices);
		this->position = position;
		this->scale = scale;
	}
	void Render(Renderer renderer) {
		renderer.shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		renderer.shader.SetMat4("model", model);
		mesh.DrawMesh();
	}
};
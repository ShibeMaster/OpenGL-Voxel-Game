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

	HudItem();
	HudItem(std::vector<Vertex> vertices, glm::vec3 position, glm::vec3 scale);
	void Render(Renderer renderer);
};
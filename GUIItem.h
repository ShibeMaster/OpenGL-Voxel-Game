#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include "GUIUtils.h"
#include "MathsExtensions.h"
#include "WindowInfo.h"
#include "Renderer.h"

class GUIItem {
public:
	glm::vec2 position;
	glm::vec2 dimensions;
	glm::vec4 color;
	GUIAlignment alignment;
	Mesh mesh;

	GUIItem(){}
	GUIItem(glm::vec2 position, glm::vec2 dimensions, glm::vec4 color, GUIAlignment alignment) {
		this->position = position;
		this->dimensions = dimensions;
		this->color = color;
		this->alignment = alignment;

		switch (alignment) {
		case GUIAlignment::alignment_left: this->position.x -= this->dimensions.x; break;
		case GUIAlignment::alignment_center: this->position.x -= (this->dimensions.x / 2); break;
		}
		mesh = GUIUtils::GenerateMesh(this->position, this->dimensions, this->color, this->alignment);
	}
	void Render(Renderer& renderer) {
		renderer.shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);
		mesh.DrawMesh();
	}

};
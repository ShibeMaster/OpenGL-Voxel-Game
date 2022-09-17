#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "Mesh.h"
#include "HudItem.h"
class HotbarSlot
{
public:
	int index;
	HudItem item;

	HotbarSlot() {}
	HotbarSlot(int index, std::vector<Vertex> vertices, glm::vec3 position, glm::vec3 scale) {
		this->index = index;
		item = HudItem(vertices, position, scale);
	}
	void Render(Renderer renderer) {
		item.Render(renderer);
	}
};


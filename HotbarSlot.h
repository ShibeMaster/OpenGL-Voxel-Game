#pragma once
#include <iostream>
#include "Inventory.h"
#include "Renderer.h"
#include "HudItem.h"
#include "Mesh.h"
#include <vector>

class HotbarSlot
{
public:
	int index;
	HudItem slot;
	ItemType type = ItemType::item_none;
	HudItem itemHUD;

	HotbarSlot() {}
	HotbarSlot(int index, std::vector<Vertex> vertices, glm::vec3 position, glm::vec3 scale) {
		this->index = index;
		slot = HudItem(vertices, position, scale);
		SetItem(ItemType::item_none, vertices);
	}
	void SetItem(ItemType item, std::vector<Vertex> vertices) {
		this->type = item;
		itemHUD = HudItem(vertices, glm::vec3(slot.position.x, slot.position.y, -0.2f), glm::vec3(0.05f, 0.05f, 0.05f));
	}
	void Render(Renderer renderer) {
		slot.Render(renderer);
		itemHUD.Render(renderer);
	}
};
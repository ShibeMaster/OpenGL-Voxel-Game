#pragma once
#include <iostream>
#include "Inventory.h"
#include "Renderer.h"
#include "HudItem.h"
#include "Mesh.h"
#include <vector>
#include "MathsExtensions.h"

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
	bool IsInBounds(glm::vec2 position) {
		return position.x > MathsExtensions::Lerp(400.0f, 800.0f, (slot.position.x - (0.5f * slot.scale.x))) && position.x < MathsExtensions::Lerp(400.0f, 800.0f, (slot.position.x + (0.5f * slot.scale.x))) && position.y > MathsExtensions::Lerp(400.0f, 800.0f, (slot.position.y - (0.5f * slot.scale.y))) && position.y > MathsExtensions::Lerp(400.0f, 800.0f, (slot.position.y + (0.5f * slot.scale.y)));
	}
};
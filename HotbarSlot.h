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

	HotbarSlot();
	HotbarSlot(int index, std::vector<Vertex> vertices, glm::vec3 position, glm::vec3 scale);
	void SetItem(ItemType item, std::vector<Vertex> vertices);
	void Render(Renderer renderer);
	bool IsInBounds(glm::vec2 position);
};
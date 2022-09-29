#pragma once
#include <vector>
#include "HotbarSlot.h"
#include "HudItem.h"
#include "InputManager.h"
#include "MathsExtensions.h"

class Hud
{
private:
	Renderer renderer;
	Inventory* inventory;
	std::vector<HudItem> hud;
	HudItem selectedItemHud;
	std::vector<HotbarSlot> hotbar;
	std::vector<HotbarSlot> inventoryHud;
	float hotbarSlots = 9.0f;
	int heldItemIndex = 0;
public:
	bool menuOpen = false;

	Hud();
	Hud(const char* vertexSource, const char* fragmentSource, Inventory* inventory);
	std::vector<Vertex> GetCubeVertices(glm::vec4 color);
	void ToggleInventory();
	void UpdateInventory();
	void InventoryUpdated();
	void PickupCheck();
	void Render();
};
#include "Hud.h"
Hud::Hud() {}
Hud::Hud(const char* vertexSource, const char* fragmentSource, Inventory* inventory) {
	renderer.Initialize(vertexSource, fragmentSource);
	this->inventory = inventory;
	HudItem crosshair = HudItem({ Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)} }, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f));

	selectedItemHud = HudItem(GetCubeVertices(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)), glm::vec3(-0.85f + (0.125f * 0), -0.85f, 0.1f), glm::vec3(0.15f, 0.15f, 0.15f));
	for (int i = 0; i < hotbarSlots; i++) {
		hotbar.push_back(HotbarSlot(i, GetCubeVertices(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), glm::vec3(-0.85f + (0.125f * i), -0.85f, -0.1f), glm::vec3(0.1f, 0.1f, 0.1f)));
	}

	for (int x = 0; x < inventory->INVENTORY_WIDTH; x++) {
		for (int y = 1; y < inventory->INVENTORY_HEIGHT; y++)
		{
			int index = (y * inventory->INVENTORY_WIDTH) + x;
			HotbarSlot inventorySlot = HotbarSlot(index, GetCubeVertices(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), glm::vec3(-0.8f + (0.20f * x), 0.5f - (0.20f * y), -0.1f), glm::vec3(0.1f, 0.1f, 0.1f));
			inventoryHud.push_back(inventorySlot);
		}
	}

	hud.push_back(crosshair);
}
std::vector<Vertex> Hud::GetCubeVertices(glm::vec4 color) {
	return {
			Vertex{glm::vec3(-0.5f, 0.5f, 0.0f), color}, Vertex{glm::vec3(0.5f, 0.5f, 0.0f), color}, Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), color},
			Vertex{glm::vec3(0.5f, -0.5f, 0.0f), color}, Vertex{glm::vec3(0.5f, 0.5f, 0.0f), color}, Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), color}
	};
}
void Hud::ToggleInventory() {
	if (menuOpen)
		InputManager::SetMouseLocked();
	else
		InputManager::SetMouseUnlocked();
	menuOpen = !menuOpen;
}
void Hud::UpdateInventory() {
	for (int x = 0; x < inventory->INVENTORY_WIDTH; x++) {
		for (int y = 1; y < inventory->INVENTORY_HEIGHT; y++)
		{
			int index = ((y * inventory->INVENTORY_WIDTH) - inventory->INVENTORY_WIDTH) + x;
			if (inventory->inventory[index + inventory->INVENTORY_WIDTH].data.type != inventoryHud[index].type)
				inventoryHud[index].SetItem(inventory->inventory[index + inventory->INVENTORY_WIDTH].data.type, GetCubeVertices(glm::vec4(inventory->inventory[index + inventory->INVENTORY_WIDTH].data.color, 1.0f)));
		}
	}
}
void Hud::InventoryUpdated() {
	selectedItemHud.position = glm::vec3(-0.85f + (0.125f * inventory->selectedIndex), -0.85f, 0.0f);
}
void Hud::PickupCheck() {
	if (InputManager::GetKeyDown(GLFW_KEY_SPACE)) {
		for (int x = 0; x < inventory->INVENTORY_WIDTH; x++) {
			for (int y = 1; y < inventory->INVENTORY_HEIGHT; y++) {
				int index = ((y * inventory->INVENTORY_WIDTH) - inventory->INVENTORY_WIDTH) + x;
				if (inventoryHud[index].IsInBounds(InputManager::mouse.position))
					std::cout << "in bounds" << std::endl;
				else
					std::cout << "X bounds: " << MathsExtensions::Lerp(400.0f, 800.0f, (inventoryHud[index].slot.position.x - (0.5f * inventoryHud[index].slot.scale.x))) << " | " << MathsExtensions::Lerp(400.0f, 800.0f, (inventoryHud[index].slot.position.x + (0.5f * inventoryHud[index].slot.scale.x))) << " | Y bounds: " << MathsExtensions::Lerp(400.0f, 800.0f, (inventoryHud[index].slot.position.y - (0.5f * inventoryHud[index].slot.scale.y))) << MathsExtensions::Lerp(400.0f, 800.0f, (inventoryHud[index].slot.position.y + (0.5f * inventoryHud[index].slot.scale.y))) << " | Mouse: " << InputManager::mouse.position.x << " | " << InputManager::mouse.position.y << std::endl;
			}
		}
	}
}
void Hud::Render() {
	selectedItemHud.Render(renderer);
	for (HudItem item : hud) {
		item.Render(renderer);
	}
	if (menuOpen) {
		PickupCheck();
		UpdateInventory();

		for (HotbarSlot slot : inventoryHud) {
			slot.Render(renderer);
		}
	}
	for (int i = 0; i < hotbarSlots; i++) {
		if (inventory->inventory[hotbar[i].index].data.type != hotbar[i].type) {
			std::cout << inventory->inventory[hotbar[i].index].data.type << " | " << hotbar[i].type << std::endl;
			hotbar[i].SetItem(inventory->inventory[hotbar[i].index].data.type, GetCubeVertices(glm::vec4(inventory->inventory[hotbar[i].index].data.color, 1.0f)));
		}
		hotbar[i].Render(renderer);
	}
}
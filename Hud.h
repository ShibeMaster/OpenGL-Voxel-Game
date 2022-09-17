#pragma once
#include "Renderer.h"
#include "HudItem.h"
#include "HotbarSlot.h"
#include "Inventory.h"
#include <vector>
class Hud
{
private:
	Renderer renderer;
	Inventory* inventory;
	std::vector<HudItem> hud;
	HudItem selectedItemHud;
	std::vector<HotbarSlot> hotbar;
public:
	Hud() {}
	Hud(const char* vertexSource, const char* fragmentSource, Inventory* inventory) {
		renderer.Initialize(vertexSource, fragmentSource);
		this->inventory = inventory;
		HudItem crosshair = HudItem({	Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}, Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)} }, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		
		selectedItemHud = HudItem(GetCubeVertices(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)), glm::vec3(-0.85f + (0.125f * 0), -0.85f, 0.1f), glm::vec3(0.15f, 0.15f, 0.15f));
		for (int i = 0; i < 10; i++) {
			hotbar.push_back(HotbarSlot(i, GetCubeVertices(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), glm::vec3(-0.85f + (0.125f * i), -0.85f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
		}

		hud.push_back(crosshair);
	}
	std::vector<Vertex> GetCubeVertices(glm::vec4 color) {
		return {
				Vertex{glm::vec3(-0.5f, 0.5f, 0.0f), color}, Vertex{glm::vec3(0.5f, 0.5f, 0.0f), color}, Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), color},
				Vertex{glm::vec3(0.5f, -0.5f, 0.0f), color}, Vertex{glm::vec3(0.5f, 0.5f, 0.0f), color}, Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), color}
		};
	}
	void InventoryUpdated() {
		selectedItemHud.position = glm::vec3(-0.85f + (0.125f * inventory->selectedIndex), -0.85f, 0.1f);
	}
	void Render() {
		selectedItemHud.Render(renderer);
		for (HudItem item : hud) {
			item.Render(renderer);
		}
		for (HotbarSlot slot : hotbar) {
			slot.Render(renderer);
		}
	}
};


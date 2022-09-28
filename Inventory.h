#pragma once
#include <iostream>
#include "ItemData.h"
#include "ItemDataManager.h"

struct InventoryItem
{
public:
	ItemData data;
	int count = 0;
};

class Inventory
{
public:
	const int INVENTORY_WIDTH = 9;
	const int INVENTORY_HEIGHT = 4;
	InventoryItem inventory[36];
	int selectedIndex = 0;
	bool menuOpen = false;

	bool TryAddItem(ItemType type) {
		for (int i = 0; i < INVENTORY_HEIGHT * INVENTORY_WIDTH; i++) {

			if (inventory[i].data.type == type && inventory[i].data.stackable) {
				inventory[i].count++;
				std::cout << "added Item" << std::endl;
				return true;
			}
			if (inventory[i].data.type == ItemType::item_none || inventory[i].count <= 0) {
				inventory[i].data = ItemDataManager::GetItemData(type);
				inventory[i].count = 1;

				std::cout << "added Item" << inventory[i].data.type << std::endl;
				return true;
			}
		}
		std::cout << "inventory full" << std::endl;
		return false;
	}

	void TryRemoveItem(int index) {
		if (inventory[index].data.type != ItemType::item_none) {
			if (inventory[index].count > 1)
				inventory[index].count--;
			else {
				inventory[index].data = ItemDataManager::GetItemData(ItemType::item_none);
				inventory[index].count = 0;
			}
		}
	}
};


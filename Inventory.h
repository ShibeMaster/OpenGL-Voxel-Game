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
	InventoryItem inventory[10];
	int selectedIndex = 0;

	bool TryAddItem(ItemType type) {
		for (int i = 0; i < 10; i++) {

			if (inventory[i].data.type == type) {
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
};


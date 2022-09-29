#include "Inventory.h"

bool Inventory::TryAddItem(ItemType type) {
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

void Inventory::TryRemoveItem(int index) {
	if (inventory[index].data.type != ItemType::item_none) {
		if (inventory[index].count > 1)
			inventory[index].count--;
		else {
			inventory[index].data = ItemDataManager::GetItemData(ItemType::item_none);
			inventory[index].count = 0;
		}
	}
}
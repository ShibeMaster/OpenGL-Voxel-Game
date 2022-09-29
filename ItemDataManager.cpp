#include "ItemDataManager.h"
std::unordered_map<int, ItemData> ItemDataManager::items;


void ItemDataManager::InitializeItemDefs() {
	ItemDefinitions itemdefs;
	for (ItemData item : itemdefs.availableItems) {
		items[(int)item.type] = item;
	}
}

ItemData ItemDataManager::GetItemData(ItemType item) {
	return items[(int)item];
}
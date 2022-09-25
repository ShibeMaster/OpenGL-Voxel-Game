#pragma once
#include <unordered_map>
#include "ItemDefinitions.h"
class ItemDataManager
{
public:
	static std::unordered_map<int, ItemData> items;

	static void InitializeItemDefs() {
		ItemDefinitions itemdefs;
		for (ItemData item : itemdefs.availableItems) {
			items[(int)item.type] = item;
		}
	}

	static ItemData GetItemData(ItemType item) {
		return items[(int)item];
	}
};
#pragma once
#include <unordered_map>
#include "ItemDefinitions.h"
class ItemDataManager
{
public:
	static std::unordered_map<int, ItemData> items;

	static void InitializeItemDefs();
	static ItemData GetItemData(ItemType item);
};
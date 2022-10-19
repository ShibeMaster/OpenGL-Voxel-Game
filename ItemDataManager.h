#pragma once
#include "ItemData.h"
#include <unordered_map>
class ItemDataManager
{
public:
	static std::unordered_map<int, ItemData> items;

	static ItemData GetItemData(ItemType item);
};
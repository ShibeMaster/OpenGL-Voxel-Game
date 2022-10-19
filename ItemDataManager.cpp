#include "ItemDataManager.h"
std::unordered_map<int, ItemData> ItemDataManager::items;

ItemData ItemDataManager::GetItemData(ItemType item) {
	return items[(int)item];
}
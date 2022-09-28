#pragma once
#include <vector>
#include "ItemData.h"

class ItemDefinitions {
public:


	// base item attributes
	const ItemData itemdef_none = ItemData{ ItemType::item_none, ItemUsageType::usage_none, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), false };

	const ItemData itemdef_stone = ItemData{ ItemType::item_stone, ItemUsageType::usage_placeable, glm::vec4(0.66f, 0.66f, 0.66f, 1.0f), true };

	const ItemData itemdef_dirt = ItemData{ ItemType::item_dirt, ItemUsageType::usage_placeable, glm::vec4(0.54, 0.27f, 0.07f, 1.0f), true };

	const ItemData itemdef_wood = ItemData{ ItemType::item_wood, ItemUsageType::usage_placeable, glm::vec4(0.87, 0.72f, 0.52f, 1.0f), true };

	const ItemData itemdef_sand = ItemData{ ItemType::item_sand, ItemUsageType::usage_placeable, glm::vec4(0.76, 0.69f, 0.50f, 1.0f), true };

	const ItemData itemdef_snow = ItemData{ ItemType::item_sand, ItemUsageType::usage_placeable, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), true };

	const ItemData itemdef_leaf = ItemData{ ItemType::item_leaf, ItemUsageType::usage_placeable, glm::vec4(0.17f, 0.35f, 0.15f, 0.5f), true };

	const ItemData itemdef_testItem1 = ItemData{ ItemType::item_testItem1, ItemUsageType::usage_consumable, glm::vec4(0.17f, 0.35f, 0.15f, 0.5f), true };

	const ItemData itemdef_testItem2 = ItemData{ ItemType::item_testItem2, ItemUsageType::usage_consumable, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), false };

	std::vector<ItemData> availableItems = {
		itemdef_none,
		itemdef_stone,
		itemdef_dirt,
		itemdef_wood,
		itemdef_sand,
		itemdef_snow,
		itemdef_leaf,
		itemdef_testItem1,
		itemdef_testItem2
	};
};
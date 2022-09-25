#pragma once
#include <glm/glm.hpp>

enum ItemUsageType {
	usage_none,
	usage_placeable,
	usage_tool,
	usage_consumable
};
enum ItemType {
	// Blocks
	item_none,
	item_stone,
	item_dirt,
	item_wood,
	item_sand,
	item_snow,
	item_leaf,

	// Items
	item_testItem1,
	item_testItem2
};


struct ItemData {
public:
	ItemType type;
	ItemUsageType usage;
	glm::vec3 color;
	bool stackable;

};

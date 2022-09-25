#pragma once
#include <vector>
#include "BlockData.h"

class BlockDefinitions {
public:
	const BlockData blockdef_none = BlockData{ BlockType::none, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), ItemType::item_none };

	const BlockData blockdef_stone = BlockData{ BlockType::stone, glm::vec4(0.66f, 0.66f, 0.66f, 1.0f), glm::vec4(0.66f, 0.66f, 0.66f, 1.0f), ItemType::item_stone };

	const BlockData blockdef_dirt = BlockData{ BlockType::dirt, glm::vec4(0.54, 0.27f, 0.07f, 1.0f), glm::vec4(0.33f, 0.49f, 0.27f, 1.0f), ItemType::item_dirt };

	const BlockData blockdef_wood = BlockData{ BlockType::wood, glm::vec4(0.87, 0.72f, 0.52f, 1.0f), glm::vec4(0.87, 0.72f, 0.52f, 1.0f), ItemType::item_wood };

	const BlockData blockdef_sand = BlockData{ BlockType::sand, glm::vec4(0.76, 0.69f, 0.50f, 1.0f), glm::vec4(0.80, 0.74f, 0.57f, 1.0f), ItemType::item_sand };

	const BlockData blockdef_snow = BlockData{ BlockType::snow, glm::vec4(0.54, 0.27f, 0.07f, 1.0f), glm::vec4(0.80, 0.74f, 0.57f, 1.0f), ItemType::item_snow };

	const BlockData blockdef_water = BlockData{ BlockType::water, glm::vec4(0.0f, 0.0f, 1.0f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 0.5f), ItemType::item_none };

	const BlockData blockdef_leaf = BlockData{ BlockType::leaf, glm::vec4(0.17f, 0.35f, 0.15f, 0.5f), glm::vec4(0.17f, 0.35f, 0.15f, 0.5f), ItemType::item_leaf };

	std::vector<BlockData> availableBlocks = {
		blockdef_none,
		blockdef_stone,
		blockdef_dirt,
		blockdef_wood,
		blockdef_sand,
		blockdef_snow,
		blockdef_water,
		blockdef_leaf
	};
};
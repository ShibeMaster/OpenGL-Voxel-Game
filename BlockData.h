#pragma once
#include <glm/glm.hpp>
#include "ItemData.h"

enum BlockType {
	none,
	stone,
	dirt,
	wood,
	sand,
	snow,
	water,
	leaf
};

struct BlockData
{
public:
	BlockType type = BlockType::none;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 topcolor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ItemType item = ItemType::item_none;
};
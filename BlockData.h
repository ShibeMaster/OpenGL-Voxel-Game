#pragma once
#include <glm/glm.hpp>

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
	BlockType type;
	glm::vec4 color;
	glm::vec4 topcolor;
};
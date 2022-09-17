#pragma once
#include "BlockData.h"
class BlockDataManager
{
public:

	static BlockData GetBlockData(BlockType type) {
		switch (type) {
		case BlockType::none: return GetNone();
		case BlockType::stone: return GetStone();
		case BlockType::dirt: return GetDirt();
		case BlockType::wood: return GetWood();
		case BlockType::sand: return GetSand();
		case BlockType::snow: return GetSnow();
		case BlockType::water: return GetWater();
		case BlockType::leaf: return GetLeaf();
		}
	}
	// Specific Block Data from this point onwards

	static BlockData GetNone() {
		BlockData data;
		data.type = BlockType::none;
		data.color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return data;
	}

	static BlockData GetStone() {
		BlockData data;
		data.type = BlockType::stone;
		data.color = glm::vec4(0.66f, 0.66f, 0.66f, 1.0f);
		data.topcolor = glm::vec4(0.66f, 0.66f, 0.66f, 1.0f);
		return data;
	}

	static BlockData GetDirt() {
		BlockData data;
		data.type = BlockType::dirt;
		data.color = glm::vec4(0.54, 0.27f, 0.07f, 1.0f);
		data.topcolor = glm::vec4(0.33f, 0.49f, 0.27f, 1.0f);
		return data;
	}

	static BlockData GetWood() {
		BlockData data;
		data.type = BlockType::wood;
		data.color = glm::vec4(0.87, 0.72f, 0.52f, 1.0f);
		data.topcolor = glm::vec4(0.87, 0.72f, 0.52f, 1.0f);
		return data;
	}

	static BlockData GetSand() {
		BlockData data;
		data.type = BlockType::sand;
		data.color = glm::vec4(0.76, 0.69f, 0.50f, 1.0f);
		data.topcolor = glm::vec4(0.80, 0.74f, 0.57f, 1.0f);
		return data;
	}

	static BlockData GetSnow() {
		BlockData data;
		data.type = BlockType::snow;
		data.color = glm::vec4(0.54, 0.27f, 0.07f, 1.0f);
		data.topcolor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		return data;
	}

	static BlockData GetWater() {
		BlockData data;
		data.type = BlockType::water;
		data.color = glm::vec4(0.0f, 0.0f, 1.0f, 0.5f);
		data.topcolor = glm::vec4(0.0f, 0.0f, 1.0f, 0.5f);
		return data;
	}

	static BlockData GetLeaf() {
		BlockData data;
		data.type = BlockType::leaf;
		data.color = glm::vec4(0.17f, 0.35f, 0.15f, 0.5f);
		data.topcolor = glm::vec4(0.17f, 0.35f, 0.15f, 0.5f);
		return data;
	}

};


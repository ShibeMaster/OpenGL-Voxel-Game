#pragma once
#include <unordered_map>
#include "BlockData.h"

class BlockDataManager
{
public:
	static std::unordered_map<int, BlockData> blocks;

	static BlockData* GetBlockData(BlockType type);
};
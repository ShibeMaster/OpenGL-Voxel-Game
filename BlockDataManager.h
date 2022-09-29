#pragma once
#include <unordered_map>
#include "BlockData.h"
#include "BlockDefinitions.h"

class BlockDataManager
{
	static std::unordered_map<int, BlockData> blocks;

public:
	static void InitializeBlockDefs();
	static BlockData* GetBlockData(BlockType type);
};
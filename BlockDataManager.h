#pragma once
#include <unordered_map>
#include "BlockData.h"
#include "BlockDefinitions.h"

class BlockDataManager
{
	static std::unordered_map<int, BlockData> blocks;

public:

	static void InitializeBlockDefs() {
		BlockDefinitions blockdefs;
		for (BlockData block : blockdefs.availableBlocks) {
			blocks[(int)block.type] = block;
		}
	}
	static BlockData* GetBlockData(BlockType type) {
		return &blocks[(int)type];
	}

};
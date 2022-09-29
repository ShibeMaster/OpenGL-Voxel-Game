#include "BlockDataManager.h"
std::unordered_map<int, BlockData> BlockDataManager::blocks;

void BlockDataManager::InitializeBlockDefs() {
	BlockDefinitions blockdefs;
	for (BlockData block : blockdefs.availableBlocks) {
		blocks[(int)block.type] = block;
	}
}
BlockData* BlockDataManager::GetBlockData(BlockType type) {
	return &blocks[(int)type];
}
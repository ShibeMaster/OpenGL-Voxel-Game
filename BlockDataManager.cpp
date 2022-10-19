#include "BlockDataManager.h"
std::unordered_map<int, BlockData> BlockDataManager::blocks;

BlockData* BlockDataManager::GetBlockData(BlockType type) {
	return &blocks[(int)type];
}
#pragma once
#include <glm/glm.hpp>
#include "BlockData.h"
#include "BlockDataManager.h"
#include <iostream>


class Block
{
public:
	BlockData data;
	

	Block() {
		data = BlockDataManager::GetNone();
	}
	Block(BlockData blockdata) {
		data = blockdata;
	}
};


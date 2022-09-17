#pragma once
#include "ItemDataManager.h"
enum Item {
	// Blocks
	none_item,
	stone_item,
	dirt_item,
	wood_item,
	sand_item,
	snow_item,
	water_item,
	leaf_item,



	// Items
	testItem1_item,
	testItem2_item
};

struct InventoryItem
{
public:
	int count = 0;
};

class Inventory
{
public:
	InventoryItem inventory[10];
	int selectedIndex = 0;	

	void AddItem(Item item) {

			
	}
};


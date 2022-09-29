#pragma once
#include <iostream>
#include "ItemData.h"
#include "ItemDataManager.h"

struct InventoryItem
{
public:
	ItemData data;
	int count = 0;
};

class Inventory
{
public:
	const int INVENTORY_WIDTH = 9;
	const int INVENTORY_HEIGHT = 4;
	InventoryItem inventory[36];
	int selectedIndex = 0;
	bool menuOpen = false;

	bool TryAddItem(ItemType type);
	void TryRemoveItem(int index);
};


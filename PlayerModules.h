#pragma once
#include "MovementModule.h"
#include "PlacingModule.h"

class PlayerModules
{
public:
	Modules::MovementModule movement;
	Modules::PlacingModule placing;

	void Initialize(PlayerData* data);
	void Update(); // for modules that need to be updated each frame
	void FixedUpdate(); // for modules that need to be frame indepentant
};


#pragma once

enum PlacingState {
	placingstate_none,
	placingstate_placing,
	placingstate_breaking
};
class PlayerState
{
public:
	PlacingState placingState;
	bool grounded;
};


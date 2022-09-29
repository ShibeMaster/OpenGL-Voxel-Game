#pragma once
#include "PlayerData.h"

namespace Modules {
	class PlayerModule
	{
	public:
		PlayerData* data;

		virtual void Initialize(PlayerData* data);
		virtual void Update();
	};
}


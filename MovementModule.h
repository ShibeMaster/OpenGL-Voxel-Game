#pragma once
#include "PlayerModule.h"
#include "Time.h"
#include "PhysicsExtensions.h"

namespace Modules {

	class MovementModule : public PlayerModule
	{
	public:
		void Update();
		void ManageInput();
		void ApplyVelocity();
	};

}

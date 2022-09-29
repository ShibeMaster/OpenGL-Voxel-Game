#pragma once
#include "PlayerModule.h"
#include "Renderer.h"

namespace Modules {
	class PlacingModule : public PlayerModule
	{
	public:
		bool hasBlockSelected;
		glm::vec3 selectedPosition;
		Mesh selectedBlockMesh;

		void Initialize(PlayerData* data);
		void Render(Renderer& renderer);
		void UnselecteBlock();
		void SetSelectedBlock(glm::vec3 position);
		void SetupMesh();
	};
}
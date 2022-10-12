#pragma once
#include "Mesh.h"
#include "WindowInfo.h"
#include "MathsExtensions.h"
class GUIUtils {
public:
	static Mesh GenerateMesh(glm::vec2 position, glm::vec2 dimensions, glm::vec4 color, GUIAlignment alignment) {
		float screenPosTopLeftX = MathsExtensions::InverseLerp(SCREEN_WIDTH / 2, SCREEN_WIDTH, position.x);
		float screenPosTopLeftY = MathsExtensions::InverseLerp(SCREEN_HEIGHT / 2, SCREEN_HEIGHT, position.y);
		float screenPosBottomRightX = MathsExtensions::InverseLerp(SCREEN_HEIGHT / 2, SCREEN_HEIGHT, position.x + dimensions.x);
		float screenPosBottomRightY = MathsExtensions::InverseLerp(SCREEN_HEIGHT / 2, SCREEN_HEIGHT, position.y + dimensions.y);

		return Mesh({
			Vertex{ glm::vec3(screenPosTopLeftX, screenPosTopLeftY, 0.0f), color }, Vertex{ glm::vec3(screenPosBottomRightX, screenPosTopLeftY, 0.0f), color }, Vertex{ glm::vec3(screenPosTopLeftX, screenPosBottomRightY, 0.0f), color },
				Vertex{ glm::vec3(screenPosBottomRightX, screenPosTopLeftY, 0.0f), color }, Vertex{ glm::vec3(screenPosBottomRightX, screenPosBottomRightY, 0.0f), color }, Vertex{ glm::vec3(screenPosTopLeftX, screenPosBottomRightY, 0.0f), color }
			});
	}
};
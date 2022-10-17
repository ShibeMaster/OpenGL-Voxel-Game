#pragma once
#include "GUIItem.h"
#include "GUIText.h"
#include <string>

enum class SliderDirection {
	SliderDirection_up,
	SliderDirection_left,
	SliderDirection_right,
	SliderDirection_down,
};
class GUIPercentageBar : public GUIItem {
public:
	glm::vec4 barColor;
	Mesh barMesh;
	float minValue;
	float maxValue;
	float setPercentage;
	SliderDirection direction;
	GUIText text;
	float* value; // between min and max

	float GetBarPercent() {
		return MathsExtensions::InverseLerp(minValue, maxValue, MathsExtensions::Clamp(minValue, maxValue, *value));
	}
	GUIPercentageBar() {}
	GUIPercentageBar(glm::vec2 position, glm::vec2 dimensions, glm::vec4 backgroundColor, glm::vec4 barColor, float minValue, float maxValue, float* value, SliderDirection direction, GUIAlignment alignment, std::string text) : GUIItem(position, dimensions, backgroundColor, alignment) {
		this->minValue = minValue;
		this->maxValue = maxValue;
		this->value = value;
		this->barColor = barColor;
		this->direction = direction;
		this->text = GUIText(text, glm::vec2(this->position.x - 20.0f, this->position.y + (this->dimensions.y / 2) - 5.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), GUIAlignment::alignment_left);
		UpdateBar(GetBarPercent());
	}

	void UpdateBar(float percentage) {
		if (setPercentage != percentage) {
			switch (direction) {
			case SliderDirection::SliderDirection_up: barMesh = GUIUtils::GenerateMesh(position, glm::vec2(dimensions.x, dimensions.y * percentage), barColor, alignment); break;
			case SliderDirection::SliderDirection_down: barMesh = GUIUtils::GenerateMesh(glm::vec2(position.x, position.y + dimensions.y), glm::vec2(dimensions.x, -dimensions.y * percentage), barColor, alignment); break;
			case SliderDirection::SliderDirection_right: barMesh = GUIUtils::GenerateMesh(position, glm::vec2(dimensions.x * percentage, dimensions.y), barColor, alignment); break;
			case SliderDirection::SliderDirection_left: barMesh = GUIUtils::GenerateMesh(glm::vec2(position.x + dimensions.x, position.y), glm::vec2(-dimensions.x * percentage, dimensions.y), barColor, alignment); break;
			}
			setPercentage = percentage;
		}
	}

	void Render(Renderer& renderer) {
		renderer.shader.Use();
		barMesh.DrawMesh();
		GUIItem::Render(renderer);
		text.Render();
	};
};
#pragma once
#include "GUIPercentageBar.h"
#include "InputManager.h"

class GUISlider : public GUIPercentageBar {
public:
	float sliderValue;
	bool onlyFullNumbers;
	bool interactable = true;

	float GetBarPercent() {
		return MathsExtensions::InverseLerp(minValue, maxValue, MathsExtensions::Clamp(minValue, maxValue, sliderValue));
	}
	GUISlider(){}
	GUISlider(glm::vec2 position, glm::vec2 dimensions, glm::vec4 backgroundColor, glm::vec4 barColor, float minValue, float maxValue, float defaultValue, SliderDirection direction, GUIAlignment alignment, std::string text, bool fullNumbers) : GUIPercentageBar(position, dimensions, backgroundColor, barColor, minValue, maxValue, &sliderValue, direction, alignment, text) {
		sliderValue = defaultValue;
		this->value = &sliderValue;
		this->onlyFullNumbers = fullNumbers;
	}
	void UpdateSliderPercent(GLFWwindow* window, int button, int action) {

		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS && interactable ) {
			if (InputManager::mouse.position.x > position.x && InputManager::mouse.position.x < position.x + dimensions.x && InputManager::mouse.position.y > position.y && InputManager::mouse.position.y < position.y + dimensions.y) {
				switch (direction)
				{
				case SliderDirection::SliderDirection_up: sliderValue = MathsExtensions::Lerp(minValue, maxValue, MathsExtensions::InverseLerp(position.y, position.y + dimensions.y, InputManager::mouse.position.y)); break;
				case SliderDirection::SliderDirection_left: sliderValue = MathsExtensions::Lerp(minValue, maxValue, 1.0f - MathsExtensions::InverseLerp(position.x, position.x + dimensions.x, InputManager::mouse.position.x)); break;
				case SliderDirection::SliderDirection_right: sliderValue = MathsExtensions::Lerp(minValue, maxValue, MathsExtensions::InverseLerp(position.x, position.x + dimensions.x, InputManager::mouse.position.x)); break;
				case SliderDirection::SliderDirection_down: sliderValue = MathsExtensions::Lerp(minValue, maxValue,  1.0f - MathsExtensions::InverseLerp(position.y, position.y + dimensions.y, InputManager::mouse.position.y)); break;
				default:
					break;
				}

				if (onlyFullNumbers)
					sliderValue = round(sliderValue);
				setPercentage = minValue - 1.0f;
				UpdateBar(GetBarPercent());
			}
		}
	}
};
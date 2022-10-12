#pragma once
#include "Scene.h"
#include "GUIButton.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Shaders.h"
#include "GUIText.h"

class StartButton : public GUIButton {
public:

	StartButton(){}
	StartButton(glm::vec2 position, glm::vec2 dimensions, glm::vec4 colour, GUIAlignment alignment, std::string text, glm::vec3 textColor) : GUIButton(position, dimensions, colour, alignment, text, textColor) {}
	void OnClicked() {
		InputManager::SetMouseLocked();
		SceneManager::SetActiveScene(&SceneManager::world);
	}
};
class MainMenu : public Scene {
public:
	Renderer renderer;
	GUIText title;
	StartButton startButton;
	GUICheckbox buttonTest2;
	GUISlider slider;


	MainMenu(){}
	void Start() {
		InputManager::SetMouseUnlocked();
		startButton = StartButton(glm::vec2(400, 150), glm::vec2(250, 80), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), GUIAlignment::alignment_center, "Start", glm::vec3(0.0f, 0.0f, 0.0f));
		buttonTest2 = GUICheckbox(glm::vec2(400, 400), glm::vec2(80, 80), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), GUIAlignment::alignment_center, "test button text test test", glm::vec3(1.0f, 1.0f, 1.0f));
		title = GUIText("Quarry Craft", glm::vec2(400, 700), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), GUIAlignment::alignment_center);
		slider = GUISlider(glm::vec2(400, 550), glm::vec2(350, 80), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 10.0f, 5.0f, SliderDirection::SliderDirection_right, GUIAlignment::alignment_center, "test slider", true);
		renderer.Initialize(Shaders::hudVertexSource, Shaders::fragmentSource);
		started = true;
	}

	void Update() {
		
		startButton.CheckForPress();
		buttonTest2.CheckForPress();
		startButton.Render(renderer);
		buttonTest2.Render(renderer);
		slider.UpdateSliderPercent();
		slider.Render(renderer);
		title.Render();
	}
};
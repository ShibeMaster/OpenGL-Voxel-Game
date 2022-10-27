#pragma once
#include "Scene.h"
#include "Resources.h"
#include "GUIButton.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Shaders.h"
#include "GUIManager.h"
#include "GUITextbox.h"
#include "GUIText.h"

class LoadModelButton : public GUIButton {
public:
	GUITextbox textbox;
	LoadModelButton() {}
	LoadModelButton(glm::vec2 position, glm::vec2 dimensions, glm::vec4 colour, GUIAlignment alignment, std::string text, glm::vec3 textColor) : GUIButton(position, dimensions, colour, alignment, text, textColor) {
		textbox = GUITextbox(position + glm::vec2(0.0f, 125.0f), glm::vec2(350, 80), glm::vec4(1.0f, 1.0f, 1.0f, 1.0), glm::vec4(0.0f), 20, "models\\", "Model Resource", GUIAlignment::alignment_center);
	}
	void Render(Renderer& renderer) {
		renderer.shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);
		mesh.DrawMesh();
		textbox.Render(renderer);
		text.Render();
	}
	void OnClicked() {
		SceneManager::editor.path = textbox.text;
		SceneManager::SetActiveScene(&SceneManager::editor);
	}
};
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
	GUIText title;
	LoadModelButton loadModel;
	StartButton startButton;


	MainMenu(){}
	void Start() {
		InputManager::SetMouseUnlocked();
		startButton = StartButton(glm::vec2(400, 150), glm::vec2(250, 80), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), GUIAlignment::alignment_center, "Start", glm::vec3(0.0f, 0.0f, 0.0f));
		title = GUIText("OpenGL Voxel Game", glm::vec2(400, 700), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), GUIAlignment::alignment_center);
		loadModel = LoadModelButton(glm::vec2(400, 300), glm::vec2(250, 80), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), GUIAlignment::alignment_center, "Load Model", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		GUIManager::RegisterButton(&startButton);
		GUIManager::RegisterButton(&loadModel);
		GUIManager::RegisterTextbox(&loadModel.textbox);
		started = true;
	}

	void Update() {
		title.Render();
		loadModel.Render(GUIManager::renderer);
		startButton.Render(GUIManager::renderer);
	}
};
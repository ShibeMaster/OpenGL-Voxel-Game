#pragma once
#include "Scene.h"
#include "Shaders.h"
#include <vector>
#include "GUIText.h"
#include "Model.h"
#include "GUICheckbox.h"
#include "GUIPercentageBar.h"
#include <nlohmann/json.hpp>
#include "SpectatorCamera.h"
#include "Resources.h"

class ModelEditor : public Scene {
public:
	std::string path;
	Model model;
	std::string jsonString;
	GUIText modelTitle;
	GUICheckbox animationToggle;
	GUICheckbox animationPause;
	GUIPercentageBar animationProgress;
	float animationPercentage = 0.0f;
	SpectatorCamera camera;

	void Start() {
		InputManager::SetMouseLocked();

		std::ifstream file(Resources::ResourcesPath + path + ".json");
		jsonString = nlohmann::json::parse(file).dump(4);
		file.close();
		model = Resources::LoadModel(path);
		model.Generate();

		camera.camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

		modelTitle = GUIText(model.name, glm::vec2(100, 750), 1.0f, glm::vec3(1.0f), GUIAlignment::alignment_center);
		animationToggle = GUICheckbox(glm::vec2(750, 750), glm::vec2(40, 40), glm::vec4(1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), GUIAlignment::alignment_center, "Playing", glm::vec3(1.0f));
		animationPause = GUICheckbox(glm::vec2(750, 680), glm::vec2(40, 40), glm::vec4(1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), GUIAlignment::alignment_center, "Pause", glm::vec3(1.0f));
		animationProgress = GUIPercentageBar(glm::vec2(780, 600), glm::vec2(100, 40), glm::vec4(1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0, 1, SliderDirection::SliderDirection_right, GUIAlignment::alignment_left, "Progress");
		started = true;

		GUIManager::RegisterButton(&animationToggle);
		GUIManager::RegisterButton(&animationPause);

	}
	void HandleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mod) {
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			model.StartAnimation();
		}
		if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
			if(!InputManager::mouse.inputLocked)
				InputManager::SetMouseUnlocked();
			else 
				InputManager::SetMouseLocked();
		}
		if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			if (model.isPaused) {
				model.Unpause();
			}
			else {
				model.Pause();
			}
		}
	}
	void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {
		camera.camera.ProcessCameraMouse();
	}
	void Update() {
		camera.Update();

		glm::mat4 modelMat = glm::mat4(1.0f);
		model.renderer.shader.SetMat4("model", modelMat);

		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 800.0f, 0.1f, 100.0f);
		model.renderer.shader.SetMat4("projection", projection);

		glm::mat4 view = camera.camera.GetViewMatrix();
		model.renderer.shader.SetMat4("view", view);

		if(model.inAnimation && !model.isPaused)
			model.AnimationUpdate();

		if (animationToggle.checked && (!model.inAnimation && !model.isPaused)) {
			model.StartAnimation();
		}
		else if (!animationToggle.checked && (model.inAnimation && !model.isPaused)) {
			model.EndAnimation();
		}

		if(animationPause.checked && !model.isPaused){
			model.Pause();

		}
		else if (!animationPause.checked && model.isPaused) {
			model.Unpause();
		}

		if (Resources::ResourcesUpdated(path, jsonString)) {
			std::ifstream file(Resources::ResourcesPath + path + ".json");
			if (file) {
				std::ostringstream ss;
				ss << file.rdbuf(); // reading data
				jsonString = ss.str();
				std::cout << jsonString << std::endl;
			}
			file.close();
			model = Resources::LoadModel(path);
		}

		if (model.inAnimation && !model.isPaused) {
			animationPercentage = MathsExtensions::InverseLerp(0, model.animation.frames[model.animation.frames.size() - 1].time, glfwGetTime() - model.animationStartTime);
			animationProgress.UpdateBar(animationPercentage);
		}
		model.Render(glm::vec3(0.0f));
		modelTitle.Render();
		animationPause.Render(GUIManager::renderer);
		animationToggle.Render(GUIManager::renderer);
		animationProgress.Render(GUIManager::renderer);
	}
};
#pragma once
#include "Scene.h"
#include "Shaders.h"
#include <vector>
#include "GUIText.h"
#include "Model.h"
#include <nlohmann/json.hpp>
#include "SpectatorCamera.h"
#include "Resources.h"

class ModelEditor : public Scene {
public:
	std::string path;
	Model model;
	std::string jsonString;
	GUIText modelTitle;
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
		started = true;
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

		model.Render(glm::vec3(0.0f));
		modelTitle.Render();
	}
};
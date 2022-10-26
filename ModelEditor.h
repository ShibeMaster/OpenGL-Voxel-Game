#pragma once
#include "Scene.h"
#include "Shaders.h"
#include <vector>
#include "Model.h"
#include <nlohmann/json.hpp>
#include "SpectatorCamera.h"
#include "Resources.h"

class ModelEditor : public Scene {
public:
	Renderer renderer;
	std::string path;
	Model model;
	std::string jsonString;
	SpectatorCamera camera;

	void Start() {
		InputManager::SetMouseLocked();
		renderer.Initialize(Shaders::vertexSource, Shaders::fragmentSource);
		renderer.shader.Use();
		std::cout << "Enter the resource path of the model you want to load: " << std::endl;
		std::cin >> path;

		std::ifstream file(Resources::ResourcesPath + path + ".json");
		jsonString = nlohmann::json::parse(file).dump(4);
		file.close();
		model = Resources::LoadModel(path);
		model.Generate();

		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 800.0f, 0.1f, 100.0f);
		renderer.shader.SetMat4("projection", projection);

		camera.camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

		started = true;
	}
	void HandleMouseInput(GLFWwindow* window, double xpos, double ypos) {
		camera.camera.ProcessCameraMouse();
	}
	void FixedUpdate() {
		camera.FixedUpdate();
	}
	void Update() {


		glm::mat4 modelMat = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", modelMat);

		glm::mat4 view = camera.camera.GetViewMatrix();
		renderer.shader.SetMat4("view", view);

		if (Resources::ResourcesUpdated(path, jsonString)) {
			std::cout << "updated" << std::endl;
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

		model.Render(renderer);
	}
};
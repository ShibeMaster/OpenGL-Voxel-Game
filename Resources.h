#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "BlockData.h"
#include <iostream>
#include "Model.h"
#include "ItemData.h"
#include "BlockDataManager.h"
#include "Shader.h"
#include "ItemDataManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
enum class ResourceType {
	resourcetype_block,
	resourcetype_biome,
	resourcetype_item
};

class Resources {
public:
	static std::string ResourcesPath;

	static void InitializeDirectories() {
		std::filesystem::create_directory(ResourcesPath);
		std::filesystem::create_directory(ResourcesPath + "blocks");
		std::filesystem::create_directory(ResourcesPath + "biomes");
		std::filesystem::create_directory(ResourcesPath + "items");
		std::filesystem::create_directory(ResourcesPath + "models");
		std::filesystem::create_directory(ResourcesPath + "shaders");
		LoadResources();
	}
	#pragma region Loading Data
	static void LoadResources() {
		for (auto entry : std::filesystem::recursive_directory_iterator(ResourcesPath + "blocks")) {
			BlockData data = GetBlockResource("blocks\\" + entry.path().stem().string());
			BlockDataManager::blocks[data.type] = data;
		}

		for (auto entry : std::filesystem::recursive_directory_iterator(ResourcesPath + "items")) {
			ItemData data = GetItemResource("items\\" + entry.path().stem().string());
			ItemDataManager::items[data.type] = data;
		}
	}
	static void LoadResource(std::string resource) {
		std::ifstream file(ResourcesPath + resource + ".json");
		json data = json::parse(file);
		std::cout << data.dump(4) << std::endl;
	}
	#pragma endregion
	static BlockData GetBlockResource(std::string resource) {
		std::cout << resource << std::endl;
		std::ifstream file(ResourcesPath + resource + ".json");
		json data = json::parse(file);
		file.close();
		BlockData block;
		block.topcolor = glm::vec4(data["topcolor"]["r"], data["topcolor"]["g"], data["topcolor"]["b"], data["topcolor"]["a"]);
		block.color = glm::vec4(data["color"]["r"], data["color"]["g"], data["color"]["b"], data["color"]["a"]);
		block.item = data["item"];
		block.type = data["type"];
		return block;
	}
	static ItemData GetItemResource(std::string resource) {
		std::cout << resource << std::endl;
		std::ifstream file(ResourcesPath + resource + ".json");
		json data = json::parse(file);
		file.close();
		ItemData item;
		item.type = data["type"];
		item.usage = data["usage"];
		item.color = glm::vec3(data["color"]["r"], data["color"]["g"], data["color"]["b"]);
		item.stackable = data["stackable"];
		return item;
	}
	static std::string ReadResource(std::string path) {

		std::ifstream file(ResourcesPath + path);
		std::string data;
		if (file) {
			std::ostringstream ss;
			ss << file.rdbuf(); // reading data
			data = ss.str();
		}
		file.close();
		return data;
	}
	static bool ResourcesUpdated(std::string resourcePath, std::string oldJson) {

		try {
			std::string data = ReadResource(resourcePath + ".json");
			return oldJson != data;
		}
		catch (int e) {
			std::cout << "error " << e << std::endl;
			return false;
		}
	}
	static Shader LoadShader(std::string vertexResource, std::string fragmentResource) {
		std::cout << vertexResource << " | " << fragmentResource << std::endl;

		std::string vertex = ReadResource(vertexResource + ".txt");
		std::string fragment = ReadResource(fragmentResource + ".txt");

		Shader shader = Shader(vertex.c_str(), fragment.c_str());
		return shader;
	}
	static Model LoadModel(std::string resource) {
		std::cout << resource << std::endl;
		try {
			std::ifstream file(ResourcesPath + resource + ".json");
			json data = json::parse(file);
			file.close();
			Model model;
			model.name = data["name"];
			std::map<std::string, Model> otherRequiredModels;
			std::string vertexPath = std::string(data["shader"]["vertex"]) + ".txt";
			std::string fragmentPath = std::string(data["shader"]["fragment"]) + ".txt";
			model.renderer.Initialize(ReadResource(vertexPath).c_str(), ReadResource(fragmentPath).c_str());
			for (auto shaderUniform : data["shader"]["uniforms"]) {
				model.renderer.shader.Use();
				if (shaderUniform["type"] == "vec3")
					model.renderer.shader.SetVec3(shaderUniform["title"], glm::vec3(shaderUniform["value"][0], shaderUniform["value"][1], shaderUniform["value"][2]));
				else if (shaderUniform["type"] == "vec4")
					model.renderer.shader.SetVec4(shaderUniform["title"], glm::vec4(shaderUniform["value"][0], shaderUniform["value"][1], shaderUniform["value"][2], shaderUniform["value"][3]));
			}
			for (auto jsonObject : data["objects"]) {
				Object object;
				json transformPosition = jsonObject["transform"]["position"];
				json transformRotation = jsonObject["transform"]["rotation"];
				json transformPivot = jsonObject["transform"]["pivot"];
				json transformScale = jsonObject["transform"]["scale"];

				object.transform = Transform{ glm::vec3(transformPosition[0], transformPosition[1], transformPosition[2]), glm::vec3(transformRotation[0], transformRotation[1], transformRotation[2]), glm::vec3(transformPivot[0], transformPivot[1], transformPivot[2]), glm::vec3(transformScale[0], transformScale[1], transformScale[2])};
				object.name = jsonObject["name"];
				if (jsonObject["type"] == "custom") {
					for (auto jsonVertex : jsonObject["vertices"]) {
						json jsonVertPosition = jsonVertex["position"];
						json jsonColor = jsonVertex["color"];
						Vertex vertex = Vertex{ glm::vec3(jsonVertPosition[0], jsonVertPosition[1], jsonVertPosition[2]), glm::vec4(jsonColor[0], jsonColor[1], jsonColor[2], jsonColor[3]) };
						object.vertices.push_back(vertex);
					}
				}
				else {
					std::string objectResourcePath = jsonObject["type"];
					if (otherRequiredModels.find(objectResourcePath) == otherRequiredModels.end())
						otherRequiredModels[objectResourcePath] = LoadModel(objectResourcePath);

					for (auto otherModel : otherRequiredModels[objectResourcePath].objects) {
						for (Vertex vertex : otherModel.second.vertices) {
							vertex.color = glm::vec4(jsonObject["color"][0], jsonObject["color"][1], jsonObject["color"][2], jsonObject["color"][3]);
							object.vertices.push_back(vertex);
						}
					}
				}
				model.objects[object.name] = object;
			}

			model.hasAnimation = data["hasAnimation"];
			if (model.hasAnimation) {
				Animation animation;
				animation.looping = data["animation"]["looping"];
				std::vector<Frame> keyframes;
				for (auto jsonAnimation : data["animation"]["keyframes"]) {
					Frame frame;
					frame.time = jsonAnimation["time"];
					for (auto jsonAnimChange : jsonAnimation["objectChanges"]) {
						ObjectChange change;
						change.name = jsonAnimChange["name"];
						json changePosition = jsonAnimChange["transform"]["position"];
						json changeRotation = jsonAnimChange["transform"]["rotation"];
						json changeScale = jsonAnimChange["transform"]["scale"];
						change.transform = Transform{ glm::vec3(changePosition[0], changePosition[1], changePosition[2]), glm::vec3(changeRotation[0], changeRotation[1], changeRotation[2]), model.objects[change.name].transform.pivot, glm::vec3(changeScale[0], changeScale[1], changeScale[2])};
						frame.changes.push_back(change);
					}
					keyframes.push_back(frame);
				}
				animation.frames = keyframes;
				model.animation = animation;
			}

			model.Generate();
			return model;
		}
		catch (int e) {
			std::cout << "error " << e << std::endl;
		}
		return Model();
	}
	static void SetBlockData(BlockData block, std::string path) {

		std::ofstream file(ResourcesPath + path + ".json");
		json data;
		data["type"] = block.type;
		data["item"] = block.item;
		data["color"]["r"] = block.color.x;
		data["color"]["g"] = block.color.y;
		data["color"]["b"] = block.color.z;
		data["color"]["a"] = block.color.w;
		data["topcolor"]["r"] = block.topcolor.x;
		data["topcolor"]["g"] = block.topcolor.y;
		data["topcolor"]["b"] = block.topcolor.z;
		data["topcolor"]["a"] = block.topcolor.w;
		file << data.dump(4);
		file.close();
	}
	static void SetItemData(ItemData item, std::string path) {

		std::ofstream file(ResourcesPath + path + ".json");
		json data;
		data["type"] = item.type;
		data["usage"] = item.usage;
		data["color"]["r"] = item.color.x;
		data["color"]["g"] = item.color.y;
		data["color"]["b"] = item.color.z;
		data["stackable"] = item.stackable;
		file << data.dump(4);
		file.close();
	}
};
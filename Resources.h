#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "BlockData.h"
#include <iostream>
#include "Model.h"
#include "ItemData.h"
#include "BlockDataManager.h"
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
	static bool ResourcesUpdated(std::string resourcePath, std::string oldJson) {

		try {
			std::ifstream file(ResourcesPath + resourcePath + ".json");
			std::string data;
			if (file) {
				std::ostringstream ss;
				ss << file.rdbuf(); // reading data
				data = ss.str();
			}
			file.close();
			return oldJson != data;
		}
		catch (int e) {
			std::cout << "error " << e << std::endl;
			return false;
		}
	}
	static Model LoadModel(std::string resource) {
		std::cout << resource << std::endl;
		try {
			std::ifstream file(ResourcesPath + resource + ".json");
			json data = json::parse(file);
			file.close();
			Model model;
			model.name = data["name"];
			for (auto jsonObject : data["objects"]) {
				Object object;
				json transformPosition = jsonObject["transform"]["position"];
				json transformRotation = jsonObject["transform"]["rotation"];
				json transformScale = jsonObject["transform"]["scale"];

				object.transform = Transform{ glm::vec3(transformPosition[0], transformPosition[1], transformPosition[2]), glm::vec3(transformRotation[0], transformRotation[1], transformRotation[2]), glm::vec3(transformScale[0], transformScale[1], transformScale[2]) };
				
				if (jsonObject["type"] == "custom") {
					for (auto jsonVertex : jsonObject["vertices"]) {
						json jsonVertPosition = jsonVertex["position"];
						json jsonColor = jsonVertex["color"];
						Vertex vertex = Vertex{ glm::vec3(jsonVertPosition[0], jsonVertPosition[1], jsonVertPosition[2]), glm::vec4(jsonColor[0], jsonColor[1], jsonColor[2], jsonColor[3]) };
						object.vertices.push_back(vertex);
					}
				}
				else if (jsonObject["type"] == "cube") {
					for (int i = 0; i < 36 * 3; i += 3) {
						Vertex vertex;
						vertex.color = glm::vec4(jsonObject["color"][0], jsonObject["color"][1], jsonObject["color"][2], jsonObject["color"][3]);
						vertex.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
						object.vertices.push_back(vertex);
					}
				}
				model.objects.push_back(object);
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
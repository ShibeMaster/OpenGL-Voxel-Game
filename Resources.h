#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "BlockData.h"
#include <iostream>
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
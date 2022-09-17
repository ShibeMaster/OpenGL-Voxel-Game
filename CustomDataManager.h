#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <direct.h>
#include <filesystem>
#include "BlockData.h"


enum DataType {
	block,
	biome,
	item
};
static const std::string FILE_EXTENSION = ".cum";
class CustomData {
public:
	friend std::istream& operator>>(std::istream& stream, CustomData& data);
	friend std::ostream& operator<<(std::ostream& stream, CustomData& data);
	friend std::ofstream& operator<<(std::ofstream& stream, CustomData& data);
	friend std::ifstream& operator>>(std::ifstream& stream, CustomData& data);
};

struct BlockCustomData : public CustomData {
public:
	BlockType type;
	glm::vec4 color;
	glm::vec4 topColor;

	friend std::istream& operator>>(std::istream& stream, CustomData& data) {
		BlockCustomData* blockData = (BlockCustomData*)&data;
		
		
		// Type
		std::cout << "Input Block Type (int):" << std::endl;
		int blockType;
		stream >> blockType;
		blockData->type = (BlockType)blockType;

		// Color	
		std::cout << "Input Block Color (vec4):" << std::endl;

		std::cout << "r: ";
		float x;
		stream >> x;

		std::cout << "g: ";
		float y;
		stream >> y;

		std::cout << "b: ";
		float z;
		stream >> z;

		std::cout << "a: ";
		float w;
		stream >> w;

		blockData->color = glm::vec4(x, y, z, w);

		// Top Color
		std::cout << "Input Block Top Color (vec4):" << std::endl;

		std::cout << "r: ";
		float xi;
		stream >> xi;

		std::cout << "g: ";
		float yi;
		stream >> yi;

		std::cout << "b: ";
		float zi;
		stream >> zi;

		std::cout << "a: ";
		float wi;
		stream >> wi;

		blockData->topColor = glm::vec4(x, y, z, w);

		data = *blockData;
		return stream;
	}
	friend std::ostream& operator<<(std::ostream& stream, CustomData& data) {
		BlockCustomData* blockData = (BlockCustomData*)&data;
		stream << "Type: " << blockData->type << std::endl;
		stream << "Color: " << blockData->color.x << " | " << blockData->color.y << " | " << blockData->color.z << " | " << blockData->color.w << std::endl;
		stream << "Top Color: " << blockData->topColor.x << " | " << blockData->topColor.y << " | " << blockData->topColor.z << " | "  << blockData->topColor.w << std::endl;
		return stream;
	}
	friend std::ofstream& operator<<(std::ofstream& stream, CustomData& data) {
		BlockCustomData* blockData = (BlockCustomData*)&data;
		stream << blockData->type << std::endl;
		stream << blockData->color.x << std::endl << blockData->color.y << std::endl << blockData->color.z << std::endl << blockData->color.w << std::endl;
		stream << blockData->topColor.x << std::endl << blockData->topColor.y << std::endl << blockData->topColor.z << std::endl << blockData->topColor.w << std::endl;
		return stream;
	}
	friend std::ifstream& operator>>(std::ifstream& stream, CustomData& data) {
		BlockCustomData* blockData = (BlockCustomData*)&data;
		int type;
		stream >> type;
		blockData->type = (BlockType)type;
		stream >> blockData->color.x;
		stream >> blockData->color.y;
		stream >> blockData->color.z;
		stream >> blockData->color.w;

		stream >> blockData->topColor.x;
		stream >> blockData->topColor.y;
		stream >> blockData->topColor.z;
		stream >> blockData->topColor.w;
		data = *blockData;
		return stream;
	}

};

struct GameData {
public:
	std::string name;
	DataType type;
	CustomData data;

	friend std::istream& operator>>(std::istream& stream, GameData& data) {
		std::cout << "Data Type: " << std::endl;
		int index;
		stream >> index;
		data.type = (DataType)index;

		std::cout << std::endl << "Name: " << std::endl;
		stream >> data.name;

		switch (data.type)
		{
		case DataType::block: data.data = BlockCustomData();
		default: data.data = CustomData(); break;
		}

		std::cout << std::endl << "Data: " << std::endl;
		stream >> data.data;
		return stream;
	}
	friend std::ostream& operator<<(std::ostream& stream, GameData& data) {
		stream << std::endl << "Data Type: " << data.type << std::endl;
		stream << "Name: " << data.name << std::endl;
		stream << data.data;
		return stream;
	}
	friend std::ofstream& operator<<(std::ofstream& stream, GameData& data) {
		stream << data.name << std::endl;
		stream << data.type << std::endl;
		stream << data.data;
		return stream;
	}
	friend std::ifstream& operator>>(std::ifstream& stream, GameData& data){
		stream >> data.name;
		int type;
		stream >> type;
		data.type = (DataType)type;
		stream >> data.data;
		return stream;
	}

};




class CustomDataManager
{
public:

	static void InitializeFilesystem() {
		int datastatus = _mkdir(GetAppDataPath().c_str());
		std::cout << datastatus << std::endl;
		
		
		std::string inputPath = GetInputPath();
		std::string outputPath = GetOutputPath();

		int inputStatus = _mkdir(inputPath.c_str());
		int outputStatus = _mkdir(outputPath.c_str());

		int blockInputStatus = _mkdir((GetInputPath() + "block\\").c_str());
		int biomeInputStatus = _mkdir((GetInputPath() + "biome\\").c_str());
		int itemInputStatus = _mkdir((GetInputPath() + "item\\").c_str());

		std::cout << inputStatus << " | " << outputStatus << std::endl;
		std::cout << blockInputStatus << " | " << biomeInputStatus << " | " << itemInputStatus << std::endl;

		std::vector<GameData> data = GetTypeData(DataType::block);
		std::cout << data[0];
	}

	static const GameData GetFileData(const char* path) {
		GameData data;
		std::ifstream stream;
		stream.open(path);
		stream >> data;
		stream.close();
		std::cout << data;
		return data;
	}

	static std::vector<GameData> GetTypeData(DataType type) {
		std::vector<const char*> paths;
		for (auto file : std::filesystem::directory_iterator(GetInputPath() + GetDataPath(type))) {
			paths.push_back(file.path().string().c_str());
		}

		std::vector<GameData> data;
		for (const char* path : paths) {
			data.push_back(GetFileData(path));
		}
		return data;
	}
	static std::string GetDataPath(DataType type) {
		switch (type) {
		case DataType::block: return "block\\";
		}
	}
	static std::string GetOutputPath() {
		return GetAppDataPath() + "\\output\\";
	}
	static std::string GetInputPath() {
		return GetAppDataPath() + "\\input\\";
	}
	static std::string GetAppDataPath() {
		char* appdataPath;
		size_t len;
		_dupenv_s(&appdataPath, &len, "appdata");
		std::cout << appdataPath << std::endl;
		std::string path = std::string(appdataPath);
		return path + "\\gaming";
	}
	static void CreateGameData(GameData data) {
		std::cin >> data;
		std::cout << std::endl << std::endl;
		std::cout << data;

		const std::string path = GetOutputPath() + data.name + FILE_EXTENSION;
		std::ofstream file = std::ofstream(path);
		file << data;
		file.close();
	}
};


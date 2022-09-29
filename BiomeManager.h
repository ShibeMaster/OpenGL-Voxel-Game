#pragma once
#include <vector>
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <map>
#include "BlockDataManager.h"
#include "MathsExtensions.h"

const int MAX_GENERATION_HEIGHT = 32;
const int MID_GENERATION_HEIGHT = 24;
const int MIN_GENERATION_HEIGHT = 12;

enum BiomeType {
	plains,
	desert,
	snowField,
	river
};

struct Layer {
	BlockType block;
	float maxGenerationHeight;
	float minGenerationHeight;
	float depth;
};
struct Biome {
public:
	BiomeType type;
	std::vector<Layer> layers;
	BlockType defaultBlock;

	float maxTemp; // from -1 to 1;
	float minTemp; // same as above;

	float maxPrec;
	float minPrec;
};



class BiomeManager
{
public:
	std::vector<Biome> biomedefs = {
		Biome{ BiomeType::plains,  { Layer{BlockType::dirt, 32, 12, 2}, Layer{BlockType::sand, 12, 7, 2} }, BlockType::stone, 0.5f, -0.5f, 1.0f, -0.25f },
		Biome{ BiomeType::desert, { Layer{ BlockType::sand, 32, 6, 5 } }, BlockType::stone, 1.0f, 0.5f, 0.0f, -1.0f },
		Biome{ BiomeType::snowField, { Layer{BlockType::snow, 32, 12, 2}, Layer{BlockType::dirt, 12, 7, 2} }, BlockType::stone, -0.25f, -1.0f, 1.0f, 0.25f },
		Biome{ BiomeType::river, { Layer{BlockType::water, 20, 10, 5} }, BlockType::sand, 0.0f, 0.0f, 1.0f, 0.25f}
	};

	std::map<int, Biome> biomes;

	FastNoiseLite temperature;
	FastNoiseLite precipitation;
	FastNoiseLite height;

	int tempSeed;
	int precSeed;
	int heightSeed;

	BiomeType defaultBiome = plains;

	std::vector<BiomeType> availableBiomes = {
		desert,
		snowField
	};

	BiomeManager();
	Biome* GetBiomeInformation(glm::vec2 position);
	void InitializeBiomeDefs();
	Biome* GetBiomeData(BiomeType type);
	int GetElevation(glm::vec2 position);
};
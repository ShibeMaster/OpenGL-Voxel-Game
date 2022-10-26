#pragma once
#include <vector>
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <map>
#include "BlockDataManager.h"
#include "MathsExtensions.h"

#define MAX_GENERATION_HEIGHT 64
#define MID_GENERATION_HEIGHT 54
#define MIN_GENERATION_HEIGHT 0

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
		Biome{ BiomeType::plains,  { Layer{BlockType::dirt, 64, 48, 2}, Layer{BlockType::sand, 48, 36, 2} }, BlockType::stone, 0.5f, -0.5f, 1.0f, -0.25f },
		Biome{ BiomeType::desert, { Layer{ BlockType::sand, 64, 48, 5 } }, BlockType::stone, 1.0f, 0.5f, 0.0f, -1.0f },
		Biome{ BiomeType::snowField, { Layer{BlockType::snow, 64, 48, 2}, Layer{BlockType::dirt, 48, 36, 2} }, BlockType::stone, -0.25f, -1.0f, 1.0f, 0.25f },
		Biome{ BiomeType::river, { Layer{BlockType::water, 49, 36, 5} }, BlockType::sand, 0.0f, 0.0f, 1.0f, 0.25f}
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
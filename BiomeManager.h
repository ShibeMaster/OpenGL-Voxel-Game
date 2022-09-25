#pragma once
#include <vector>
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <map>
#include "BlockDataManager.h"

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

	BiomeManager() {
		tempSeed = rand();
		precSeed = rand();
		heightSeed = rand();

		temperature = FastNoiseLite(tempSeed);
		temperature.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
		temperature.SetFrequency(0.005);
		temperature.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);

		precipitation = FastNoiseLite(precSeed);
		precipitation.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
		precipitation.SetFrequency(0.005);
		precipitation.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);

		height = FastNoiseLite(heightSeed);
		height.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		height.SetFractalType(FastNoiseLite::FractalType_FBm);
		height.SetFrequency(0.02);
	}


	// Biome Data


	BiomeType defaultBiome = plains;

	std::vector<BiomeType> availableBiomes = {
		desert,
		snowField
	};

	// Methods
	static float Lerp(float a, float b, float t) {
		return a + (t * (b - a));
	}

	Biome* GetBiomeInformation(glm::vec2 position) {
		Biome biome;
		float temp = Lerp(-1.0f, 1.0f, abs(temperature.GetNoise(position.x, position.y)));
		float prec = Lerp(-1.0f, 1.0f, abs(precipitation.GetNoise(position.x, position.y)));
		std::vector<Biome*> possibleBiomes;
		for (BiomeType type : availableBiomes) {
			Biome* biome = GetBiomeData(type);
			if (temp <= biome->maxTemp && temp >= biome->minTemp && prec <= biome->maxPrec && prec >= biome->minPrec) {
				possibleBiomes.push_back(biome);
			}
		}
		return possibleBiomes.size() == 0 ? (GetElevation(position) < MID_GENERATION_HEIGHT - 2) ? GetBiomeData(BiomeType::river) : GetBiomeData(defaultBiome) : possibleBiomes[rand() % possibleBiomes.size()];
	}

	void InitializeBiomeDefs() {

		for (Biome biome : biomedefs) {
			biomes[(int)biome.type] = biome;
		}
	}

	Biome* GetBiomeData(BiomeType type) {
		return &biomes[(int)type];
	}

	int GetElevation(glm::vec2 position) {
		float noise = height.GetNoise(position.x, position.y);
		int elevation = (int)Lerp(MID_GENERATION_HEIGHT, MAX_GENERATION_HEIGHT, noise);
		return elevation;
	}
};
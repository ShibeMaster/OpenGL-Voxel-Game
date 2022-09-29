#include "BiomeManager.h"
BiomeManager::BiomeManager() {
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
Biome* BiomeManager::GetBiomeInformation(glm::vec2 position) {
	Biome biome;
	float temp = MathsExtensions::Lerp(-1.0f, 1.0f, abs(temperature.GetNoise(position.x, position.y)));
	float prec = MathsExtensions::Lerp(-1.0f, 1.0f, abs(precipitation.GetNoise(position.x, position.y)));
	std::vector<Biome*> possibleBiomes;
	for (BiomeType type : availableBiomes) {
		Biome* biome = GetBiomeData(type);
		if (temp <= biome->maxTemp && temp >= biome->minTemp && prec <= biome->maxPrec && prec >= biome->minPrec) {
			possibleBiomes.push_back(biome);
		}
	}
	return possibleBiomes.size() == 0 ? (GetElevation(position) < MID_GENERATION_HEIGHT - 2) ? GetBiomeData(BiomeType::river) : GetBiomeData(defaultBiome) : possibleBiomes[rand() % possibleBiomes.size()];
}

void BiomeManager::InitializeBiomeDefs() {

	for (Biome biome : biomedefs) {
		biomes[(int)biome.type] = biome;
	}
}

Biome* BiomeManager::GetBiomeData(BiomeType type) {
	return &biomes[(int)type];
}

int BiomeManager::GetElevation(glm::vec2 position) {
	float noise = height.GetNoise(position.x, position.y);
	int elevation = (int)MathsExtensions::Lerp(MID_GENERATION_HEIGHT, MAX_GENERATION_HEIGHT, noise);
	return elevation;
}
#include "ChunkGenerationManager.h"
std::unordered_map<glm::vec3, std::thread> ChunkGenerationManager::generationThreads;
#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <thread>
#include <glm/gtx/hash.hpp>
class ChunkGenerationManager
{
public:
	static std::unordered_map<glm::vec3, std::thread> generationThreads;
};


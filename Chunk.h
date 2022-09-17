#pragma once
#include "Renderer.h"
#include "Block.h"
#include "BlockData.h"
#include <GL/glew.h>
#include <glm/gtx/hash.hpp>
#include <FastNoiseLite.h>
#include <unordered_map>
#include "Mesh.h"
#include <vector>
#include "BiomeManager.h"

// Layers
// (height - y >= layerdepth &&) set layer block


const glm::vec3 dirup = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 dirdown = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 dirright = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 dirleft = glm::vec3(-1.0f, 0.0f, 0.0f);
const glm::vec3 dirfront = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 dirback = glm::vec3(0.0f, 0.0f, 1.0f);

class Chunk
{
public:
	Block chunk[16][32][16] = {Block()};
	glm::vec2 chunkPos;

	Mesh mesh;
	Block emptyBlock;

	Chunk(glm::vec2 pos);
	Chunk(){}
	void Generate(BiomeManager biomes);
	glm::vec3 GetChunkGlobalOrigin();
	void SetChunkMesh(std::vector<Vertex> vertices);
	void RenderChunk(Renderer& renderer);
	glm::vec3 GetGlobalPosition(glm::vec3 position);
	bool IsPositionInside(glm::vec3 localPos);
	Block* GetPositionValue(glm::vec3 position);
	glm::vec3 ClampChunkPosition(glm::vec3 position);
};


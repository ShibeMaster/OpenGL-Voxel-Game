#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>
#include "Chunk.h"
#include "Mesh.h"
#include <unordered_map>
#include "FastNoiseLite.h"
#include "BiomeManager.h"
#include <iostream>
#include <stack>

class Terrain
{
public:
	std::unordered_map<glm::vec2, Chunk> chunks;
	FastNoiseLite noise;
	static const int LOADED_RADIUS = 9;
	Chunk* loadedChunks[9][9];
	const int CHUNK_LENGTH = 16;
	const int CHUNK_WIDTH = 16;
	const int SEED = rand();
	std::stack<Chunk*> chunkGenerationQueue;
	std::stack<std::pair<Chunk*, std::vector<Vertex>>> chunkMeshGenerationQueue;
	BiomeManager biomeManager;
	int emptyBlock = 0;

	bool CheckForChunkUpdates(glm::vec3 position);
	glm::vec3 GetSelectedBlock(glm::vec3 position, glm::vec3 forward, bool placing);
	bool Raycast(glm::vec3 startingPosition, glm::vec3 direction, int distance);
	glm::vec3 ClampChunkY(glm::vec3 position);
	glm::vec2 GetChunkPosition(glm::vec3 position);
	Chunk* GetPositionChunk(glm::vec3 position);
	Chunk* GetPositionChunkPointer(glm::vec3 position);
	int GetPositionValueChunk(Chunk* chunk, glm::vec3 position);
	bool ChunkExists(glm::vec2 chunkpos);
	glm::vec3 GetLocalPosition(glm::vec3 position);
	int GetPositionValue(glm::vec3 globalPos);
	bool IsChunkLoaded(Chunk* chunk);
	void GenerationThread();
	void MeshGeneration();
	void SetPosition(glm::vec3 position, int value);
	void CreateBlock(glm::vec3 position, int data);
	void DestroyBlock(glm::vec3 position);
	int GetLoadedPositionValue(glm::vec3 globalPos);
	bool CollidesWithBlock(glm::vec3 position);
	std::vector<CubeFace> GetVisibleFaces(Chunk* chunk, glm::vec3 position);
	std::vector<Vertex> CreateChunkVertexArray(Chunk* chunk);
	void RenderChunks(Renderer& renderer);
	void UpdateChunkMesh(Chunk* chunk);
	void UpdateRenderedChunks(glm::vec3 position);
};
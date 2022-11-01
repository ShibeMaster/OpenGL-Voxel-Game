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
#include <deque>
#include <NetworkChannelManager.h>
#include <Network.h>

#define VISIBLE_RADIUS 5
#define LOADED_RADIUS 6

struct ChunkRequestMessage : public ShibaNetLib::NetworkMessage {
	glm::vec3 position;
};

struct ChunkRequestCallbackMessage : public ShibaNetLib::NetworkMessage {
	glm::vec3 position;
	short segment;
	std::byte chunkSegment[16][16][16];
};


class Terrain
{
public:
	static std::unordered_map<glm::vec3, Chunk> chunks;
	static std::vector<Chunk*> loadedChunks;
	static glm::vec3 centeredChunkPosition;
	const int SEED = rand();
	static std::deque<glm::vec3> chunkGenerationQueue;
	static std::deque<std::pair<Chunk*, std::vector<Vertex>>> chunkMeshGenerationQueue;
	static BiomeManager biomeManager;
	static std::unordered_map<glm::vec3, std::thread> generationThreads;
	static int emptyBlock;

	static bool CheckForChunkUpdates(glm::vec3 position);
	glm::vec3 GetSelectedBlock(glm::vec3 position, glm::vec3 forward, bool placing);
	bool Raycast(glm::vec3 startingPosition, glm::vec3 direction, int distance);
	glm::vec3 ClampChunkY(glm::vec3 position);
	static glm::vec3 GetChunkPosition(glm::vec3 position);
	static Chunk* GetPositionChunk(glm::vec3 position);
	Chunk* GetPositionChunkPointer(glm::vec3 position);
	int GetPositionValueChunk(Chunk* chunk, glm::vec3 position);
	static bool ChunkExists(glm::vec3 chunkpos);
	static glm::vec3 GetLocalPosition(glm::vec3 position);
	static int GetPositionValue(glm::vec3 globalPos);
	bool IsChunkLoaded(Chunk* chunk);
	void GenerationThread();
	void MeshGeneration();
	void SetPosition(glm::vec3 position, int value);
	void CreateBlock(glm::vec3 position, int data);
	void DestroyBlock(glm::vec3 position);
	int GetLoadedPositionValue(glm::vec3 globalPos);
	bool CollidesWithBlock(glm::vec3 position);
	static std::vector<CubeFace> GetVisibleFaces(Chunk* chunk, glm::vec3 position);
	static std::vector<Vertex> CreateChunkVertexArray(Chunk* chunk);
	void RenderChunks(Renderer& renderer);
	void UpdateChunkMesh(Chunk* chunk);
	static void UpdateRenderedChunks(glm::vec3 position);
};


// server handling chunk requests
class ChunkRequestChannel : public ShibaNetLib::NetworkChannel {
	using NetworkChannel::NetworkChannel;

	std::deque<ChunkRequestMessage> chunkQueue;
	std::thread chunkQueueThread = std::thread([this] { this->ChunkQueueThread(); });

	void ChunkQueueThread() {
		while (true) {
			while (!chunkQueue.empty()) {
				std::cout << "received chunk request" << std::endl;
				ChunkRequestMessage incoming = chunkQueue.front();
				glm::vec3 chunkPos = incoming.position;
				if (!Terrain::ChunkExists(chunkPos) || Terrain::chunks[chunkPos].state == ChunkState::chunkstate_empty) {
					Chunk chunk = Chunk(chunkPos);
					chunk.Generate(Terrain::biomeManager);
					chunk.state = ChunkState::chunkstate_generated;
					Terrain::chunks[chunkPos] = chunk;
					Terrain::chunkGenerationQueue.push_back(chunkPos);
					chunkQueue.pop_front();
					chunkQueue.push_back(incoming);
					continue;
				}
				for (int segment = 0; segment < 2; segment++) {
					ChunkRequestCallbackMessage message{};
					message.channelid = 6;
					message.senderid = conn->netId;
					message.position = chunkPos;
					message.segment = segment;
					for (int x = 0; x < 16; x++) {
						for (int y = 0; y < 16; y++) {
							for (int z = 0; z < 16; z++) {
								message.chunkSegment[x][y][z] = Terrain::chunks[chunkPos].chunk[x][y + (16 * segment)][z];
							}
						}
					}

					conn->Send(&message, sizeof(ChunkRequestCallbackMessage));
				}
				chunkQueue.pop_front();
				std::cout << "finished loading chunk" << std::endl;
			}
		}
	}

	void Incoming(char* buffer) {
		chunkQueue.push_back(*((ChunkRequestMessage*)buffer));
	}
};

// client handling received chunk
class ChunkRequestCallbackChannel : public ShibaNetLib::NetworkChannel {
	using NetworkChannel::NetworkChannel;


	std::deque<ChunkRequestCallbackMessage> chunkQueue;
	std::thread chunkQueueThread = std::thread([this] { this->ChunkQueueThread(); });


	void ChunkQueueThread() {
		while (true) {
			while (!chunkQueue.empty()) {
				ChunkRequestCallbackMessage message = chunkQueue.front();
				for (int x = 0; x < 16; x++) {
					for (int y = 0; y < 16; y++) {
						for (int z = 0; z < 16; z++) {
							Terrain::chunks[message.position].chunk[x][y + (message.segment * 16)][z] = message.chunkSegment[x][y][z];
						}
					}
				}
				//Terrain::chunks[message.position].segmentsLoaded++;
				chunkQueue.pop_front();
				/*if (Terrain::chunks[message.position].segmentsLoaded == 2) {
					Terrain::chunks[message.position].state = ChunkState::chunkstate_generated;
					Terrain::chunkMeshGenerationQueue.push_back(std::pair(&Terrain::chunks[message.position], Terrain::CreateChunkVertexArray(&Terrain::chunks[message.position])));
					std::cout << "finished unloading chunk" << std::endl;
				}*/
			}
		}
	}
	void Incoming(char* buffer) {
		chunkQueue.push_back(*((ChunkRequestCallbackMessage*)buffer));
		std::cout << "received chunk request callback" << std::endl;

	}
};
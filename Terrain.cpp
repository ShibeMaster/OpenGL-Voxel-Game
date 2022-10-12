#include "Terrain.h"
std::unordered_map<glm::vec2, Chunk> Terrain::chunks;
std::deque<Chunk*> Terrain::chunkGenerationQueue;
std::deque<std::pair<Chunk*, std::vector<Vertex>>> Terrain::chunkMeshGenerationQueue;
BiomeManager Terrain::biomeManager;
Chunk* Terrain::loadedChunks[9][9];
bool Terrain::CheckForChunkUpdates(glm::vec3 position) {
	if (ChunkExists(GetChunkPosition(position))) {
		if (loadedChunks[5][5]->chunkPos != GetChunkPosition(position)) {
			return true;
		}
	}
	else return true;
	return false;
}
glm::vec3 Terrain::GetSelectedBlock(glm::vec3 position, glm::vec3 forward, bool placing) {
	for (int i = 0; i < 8; i++) {
		glm::vec3 rayPos = position + forward * glm::vec3(i, i, i);
		if (CollidesWithBlock(rayPos)) {
			return placing ? glm::vec3((int)(rayPos.x - forward.x), (int)(rayPos.y - forward.y), (int)(rayPos.z - forward.z)) : glm::vec3((int)(rayPos.x), (int)(rayPos.y), (int)(rayPos.z));
		}
	}
	return position;
}
bool Terrain::Raycast(glm::vec3 startingPosition, glm::vec3 direction, int distance) {
	for (int i = 0; i < distance; i++) {
		if (CollidesWithBlock(glm::vec3(startingPosition + direction * glm::vec3(i)))) return true;
	}
	return false;
}
glm::vec3 Terrain::ClampChunkY(glm::vec3 position) {
	return glm::clamp(position, glm::vec3(position.x, 0.0f, position.z), glm::vec3(position.x, 32.0f, position.z));
}
glm::vec2 Terrain::GetChunkPosition(glm::vec3 position) {
	return glm::round(glm::vec2(position.x / 16, position.z / 16));
}
Chunk* Terrain::GetPositionChunk(glm::vec3 position) {
	glm::vec2 chunkPosition = GetChunkPosition(position);

	return &chunks[chunkPosition];
}
Chunk* Terrain::GetPositionChunkPointer(glm::vec3 position) {
	glm::vec2 chunkPosition = GetChunkPosition(position);
	return &chunks[chunkPosition];
}
int Terrain::GetPositionValueChunk(Chunk* chunk, glm::vec3 position) {
	if (chunk->IsPositionInside(position)) return chunk->GetPositionValue(position);
	else return GetLoadedPositionValue(position);
}
bool Terrain::ChunkExists(glm::vec2 chunkpos) {
	return chunks.find(chunkpos) != chunks.end();
}
glm::vec3 Terrain::GetLocalPosition(glm::vec3 position) {
	int posx = (int)position.x % 16;
	int posz = (int)position.z % 16;
	return glm::vec3(posx < 0 ? 16 + posx : posx, position.y, posz < 0 ? 16 + posz : posz);
}
int Terrain::GetPositionValue(glm::vec3 globalPos) {
	if (!ChunkExists(GetChunkPosition(globalPos))) return emptyBlock;
	return GetPositionChunk(globalPos)->GetPositionValue(GetLocalPosition(globalPos));
}
bool Terrain::IsChunkLoaded(Chunk* chunk) {
	for (int x = 0; x < LOADED_RADIUS; x++) {
		for (int y = 0; y < LOADED_RADIUS; y++) {
			if (loadedChunks[x][y] == chunk) return true;
		}
	}
	return false;
}

void Terrain::GenerationThread() {
	while (true) {
		if (!chunkGenerationQueue.empty()) {
			Chunk* chunk = chunkGenerationQueue.front();
			if (chunk->state == ChunkState::chunkstate_empty) {
				chunk->Generate(biomeManager);
				chunk->state = ChunkState::chunkstate_generated;
			}
			chunkGenerationQueue.pop_front();
			std::vector<Vertex> vertices = CreateChunkVertexArray(chunk);
			chunkMeshGenerationQueue.push_back(std::pair(chunk, vertices));
		}
	}
}
void Terrain::MeshGeneration() {
	while (!chunkMeshGenerationQueue.empty()) {
		std::pair<Chunk*, std::vector<Vertex>> value = chunkMeshGenerationQueue.front();
		value.first->SetChunkMesh(value.second);
		value.first->state = ChunkState::chunkstate_ready;
		chunkMeshGenerationQueue.pop_front();
	}
}
void Terrain::SetPosition(glm::vec3 position, int value) {
	if (!ChunkExists(GetChunkPosition(position))) return;
	glm::vec3 localPos = GetLocalPosition(position);
	Chunk* chunk = GetPositionChunkPointer(position);

	chunk->chunk[(int)localPos.x][(int)localPos.y][(int)localPos.z] = std::byte(value);
	UpdateChunkMesh(chunk);
}

void Terrain::CreateBlock(glm::vec3 position, int data) {
	SetPosition(position, data);
}
void Terrain::DestroyBlock(glm::vec3 position) {
	SetPosition(position, 0);
}
int Terrain::GetLoadedPositionValue(glm::vec3 globalPos) {
	if (!ChunkExists(GetChunkPosition(globalPos))) return emptyBlock;
	if (!IsChunkLoaded(GetPositionChunk(globalPos)) || globalPos.y != ClampChunkY(globalPos).y) return emptyBlock;
	return GetPositionChunkPointer(globalPos)->GetPositionValue(GetLocalPosition(ClampChunkY(globalPos)));
}
bool Terrain::CollidesWithBlock(glm::vec3 position) {
	return GetPositionValue(position) != 0;
}
std::vector<CubeFace> Terrain::GetVisibleFaces(Chunk* chunk, glm::vec3 position) {
	std::vector<CubeFace> visibleFaces;
	if (chunk->GetPositionValue(position) != 0) {
		if (chunk->GetPositionValue(position + dirup) == 0)
			visibleFaces.push_back(CubeFace::topSide);
		if (chunk->GetPositionValue(position + dirdown) == 0)
			visibleFaces.push_back(CubeFace::bottomSide);
		if (chunk->GetPositionValue(position + dirleft) == 0)
			visibleFaces.push_back(CubeFace::leftSide);
		if (chunk->GetPositionValue(position + dirright) == 0)
			visibleFaces.push_back(CubeFace::rightSide);
		if (chunk->GetPositionValue(position + dirfront) == 0)
			visibleFaces.push_back(CubeFace::frontSide);
		if (chunk->GetPositionValue(position + dirback) == 0)
			visibleFaces.push_back(CubeFace::backSide);
	}
	return visibleFaces;
}

std::vector<Vertex> Terrain::CreateChunkVertexArray(Chunk* chunk) {
	std::vector<Vertex> chunkvertices;
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 32; y++) {
			for (int z = 0; z < 16; z++) {
				glm::vec3 localPos = glm::vec3(x, y, z);
				BlockData* block = BlockDataManager::GetBlockData((BlockType)chunk->GetPositionValue(localPos));
				glm::vec3 chunkGlobalOrigin = chunk->GetChunkGlobalOrigin();
				if (block->type != 0) {
					for (CubeFace face : GetVisibleFaces(chunk, localPos)) {
						for (int i = 0; i < 6; i++) {
							Vertex vertex;
							vertex.position = chunkGlobalOrigin + localPos + glm::vec3(vertices[((int)face * (6 * 3)) + (i * 3)], vertices[((int)face * (6 * 3)) + (i * 3) + 1], vertices[((int)face * (6 * 3)) + (i * 3) + 2]);
							vertex.color = face == CubeFace::topSide ? block->topcolor : block->color;
							chunkvertices.push_back(vertex);
						}
					}
				}
			}
		}
	}
	return chunkvertices;
}

void Terrain::RenderChunks(Renderer& renderer) {
	renderer.shader.Use();
	for (int x = 0; x < LOADED_RADIUS; x++) {
		for (int y = 0; y < LOADED_RADIUS; y++) {
			if (loadedChunks[x][y]->state == ChunkState::chunkstate_ready)
				loadedChunks[x][y]->RenderChunk(renderer);
		}
	}
}
void Terrain::UpdateChunkMesh(Chunk* chunk) {
	chunk->SetChunkMesh(CreateChunkVertexArray(chunk));
}
void Terrain::UpdateRenderedChunks(glm::vec3 position) {
	if (CheckForChunkUpdates(position)) {
		std::vector<glm::vec2> networkUpdateRequiredChunks;
		for (int x = 0; x < LOADED_RADIUS; x++) {
			for (int y = 0; y < LOADED_RADIUS; y++) {
				glm::vec2 chunkPos = GetChunkPosition(position + glm::vec3((x - floor(LOADED_RADIUS / 2)) * CHUNK_WIDTH, 0, (y - floor(LOADED_RADIUS / 2)) * CHUNK_LENGTH));
				if (!ChunkExists(chunkPos)) {
					Chunk chunk = Chunk(chunkPos);
					chunks[chunkPos] = chunk;
					loadedChunks[x][y] = &chunks[chunkPos];

					if (!ShibaNetLib::Network::conn.isServer && ShibaNetLib::Network::state == ShibaNetLib::NetworkState::netstate_connected) {
						networkUpdateRequiredChunks.push_back(chunkPos);
					}
					else
						chunkGenerationQueue.push_back(loadedChunks[x][y]);
				}
				else loadedChunks[x][y] = &chunks[chunkPos];
			}
		}
		if (!networkUpdateRequiredChunks.empty()) {
			for (glm::vec2 pos : networkUpdateRequiredChunks) {

				ChunkRequestMessage message;
				message.channelid = 5;
				message.senderid = ShibaNetLib::Network::conn.netId;
				message.position = pos;
				ShibaNetLib::Network::conn.Send(&message, sizeof(ChunkRequestMessage));
				std::cout << "sent chunk request" << std::endl;
			}
		}
	}
}
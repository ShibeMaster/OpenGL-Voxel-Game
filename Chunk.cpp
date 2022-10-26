#include "Chunk.h"
#include <vector>

Chunk::Chunk(glm::vec3 pos) {
	chunkPos = pos;
}
static float Lerp(float a, float b, float t) {
	return a + (t * (b - a));
}
void Chunk::Generate(BiomeManager biomes) {

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_LENGTH; z++) {
			glm::vec3 chunkGlobalOrigin = GetChunkGlobalOrigin();
			Biome* biome = biomes.GetBiomeInformation(glm::vec2(chunkGlobalOrigin.x + x, chunkGlobalOrigin.z + z));
			int height = biomes.GetElevation(glm::vec2(chunkGlobalOrigin.x + x, chunkGlobalOrigin.z + z));
			if (chunkGlobalOrigin.y > height) {
				state = ChunkState::chunkstate_empty;
				return;
			}

			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				bool found = false;
				int globalY = y + (CHUNK_HEIGHT * chunkPos.y);
				if (globalY > height)
					continue;

				for (Layer layer : biome->layers)
				{
					if (height <= layer.maxGenerationHeight && height >= layer.minGenerationHeight && height - globalY <= layer.depth) {
						chunk[x][y][z] = std::byte((int)(BlockDataManager::GetBlockData(layer.block)->type));
						found = true;
					}
				}
				if (!found) chunk[x][y][z] = std::byte((int)(BlockDataManager::GetBlockData(biome->defaultBlock)->type));
			}

		}
	}
	state = ChunkState::chunkstate_generated;
}
glm::vec3 Chunk::GetChunkGlobalOrigin() {
	return glm::vec3(chunkPos.x * CHUNK_WIDTH, chunkPos.y * CHUNK_HEIGHT, chunkPos.z * CHUNK_LENGTH);
}
void Chunk::SetChunkMesh(std::vector<Vertex> vertices) {
	this->mesh = Mesh(vertices);
}
void Chunk::RenderChunk(Renderer& renderer) {
	mesh.DrawMesh();
}
glm::vec3 Chunk::ClampChunkPosition(glm::vec3 position) {
	return glm::clamp(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(16.0f, 16.0f, 16.0F));
}

bool Chunk::IsPositionInside(glm::vec3 position) {
	return position.x < 16.0f && position.x >= 0.0f && position.z < 16.0f && position.z >= 0.0f && position.y >= 0.0f && position.y < 16.0f;
}

glm::vec3 Chunk::GetGlobalPosition(glm::vec3 position) {
	return glm::vec3(position.x + chunkPos.x * CHUNK_WIDTH, position.y + chunkPos.y * CHUNK_HEIGHT, position.z + chunkPos.y * CHUNK_LENGTH);
}
int Chunk::GetPositionValue(glm::vec3 position) {
	if (!IsPositionInside(position)) return std::to_integer<int>(emptyBlock);


	return std::to_integer<int>(chunk[(int)position.x][(int)position.y][(int)position.z]);
}
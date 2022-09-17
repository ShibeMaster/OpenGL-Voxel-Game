#include "Chunk.h"
#include <vector>

Chunk::Chunk(glm::vec2 pos) {
	chunkPos = pos;
}
static float Lerp(float a, float b, float t) {
	return a + (t * (b - a));
}
void Chunk::Generate(BiomeManager biomes) {

	// Gather Noise Data
	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			glm::vec3 chunkGlobalOrigin = GetChunkGlobalOrigin();
			Biome biome = biomes.GetBiomeInformation(glm::vec2(chunkGlobalOrigin.x + x, chunkGlobalOrigin.z + z));
			int height = biomes.GetElevation(glm::vec2(chunkGlobalOrigin.x + x, chunkGlobalOrigin.z + z));
			for (int y = 0; y < height; y++) {
				bool found = false;
				for (Layer layer : biome.layers)
				{
					if (height <= layer.maxGenerationHeight && height >= layer.minGenerationHeight && height - y <= layer.depth) {
						chunk[x][y][z] = Block(BlockDataManager::GetBlockData(layer.block));
						found = true;
					}
				}
				if (!found) chunk[x][y][z] = Block(BlockDataManager::GetBlockData(biome.defaultBlock));
			}

		}
	}
}
glm::vec3 Chunk::GetChunkGlobalOrigin() {
	return glm::vec3(chunkPos.x * 16, 0, chunkPos.y * 16);
}
void Chunk::SetChunkMesh(std::vector<Vertex> vertices) {
	this->mesh = Mesh(vertices);
}
void Chunk::RenderChunk(Renderer& renderer) {
	mesh.DrawMesh();
}
glm::vec3 Chunk::ClampChunkPosition(glm::vec3 position) {
	return glm::clamp(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(16.0f, 32.0f, 16.0F));
}

bool Chunk::IsPositionInside(glm::vec3 position) {
	return position.x < 16.0f && position.x >= 0.0f && position.z < 16.0f && position.z >= 0.0f && position.y >= 0.0f && position.y < 32.0f;
}

glm::vec3 Chunk::GetGlobalPosition(glm::vec3 position) {
	return glm::vec3(position.x + chunkPos.x * 16, position.y, position.z + chunkPos.y * 16);
}
Block* Chunk::GetPositionValue(glm::vec3 position) {
	if (!IsPositionInside(position)) return &emptyBlock;
	return &chunk[(int)position.x][(int)position.y][(int)position.z];
}
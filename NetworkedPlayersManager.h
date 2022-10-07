#pragma once
#include "NetworkInfo.h"
#include "Renderer.h"
#include "Mesh.h"
class NetworkedPlayersManager
{
public:
	static Mesh playerMesh;
	static void Initialize() {
		SetupMesh();
	}
	static void Render(Renderer& renderer) {
		renderer.shader.Use();
		if ((ShibaNetLib::Network::conn.isServer && ShibaNetLib::Network::clientCount > 0) || !ShibaNetLib::Network::conn.isServer) {
			for (ClientObject client : NetworkInfo::clientManifest) {
				if (client.data.netid != ShibaNetLib::Network::conn.netId) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, client.playerData.position);
					float max = max(client.playerData.rotation.x , client.playerData.rotation.y, client.playerData.rotation.z);
					model = glm::rotate(model, glm::radians(max), client.playerData.rotation / max);
					renderer.shader.SetMat4("model", model);
					playerMesh.DrawMesh();
				}
			}
		}
	}

	static void SetupMesh() {
		std::vector<Vertex> cubevertices;
		for (int i = 0; i < 36 * 3; i += 3) {
			Vertex vertex;
			vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
			vertex.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			cubevertices.push_back(vertex);
		}

		playerMesh = Mesh(cubevertices);
	}
};


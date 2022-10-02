#pragma once
#include "NetworkInfo.h"
#include "Renderer.h"
#include "Mesh.h"
#include "NetworkManager.h"
class NetworkedPlayersManager
{
public:
	static Mesh playerMesh;
	static void Initialize() {
		SetupMesh();
	}
	static void Render(Renderer& renderer) {
		renderer.shader.Use();
		if ((NetworkInfo::status == NetworkStatus::netstatus_server && NetworkManager::host.clientCount > 0) || NetworkInfo::status == NetworkStatus::netstatus_client) {
			for (ClientObject client : NetworkInfo::clientManifest) {
				if (client.data.netid != NetworkInfo::netid) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3((float)client.playerData.x, (float)client.playerData.y, (float)client.playerData.z));
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


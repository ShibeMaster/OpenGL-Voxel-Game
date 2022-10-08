#pragma once
#include <NetworkChannelManager.h>
#include "NetworkInfo.h"

struct ManifestUpdateMessage : public ShibaNetLib::NetworkMessage {
	int clientId;
	glm::vec3 position;
};

struct ManifestRequestMessage : public ShibaNetLib::NetworkMessage{};

class ClientManifestManager
{
public:
	static void PostManifestUpdateRequest(ClientObject client) {
		ManifestUpdateMessage message;
		message.channelid = 3;
		message.senderid = ShibaNetLib::Network::conn.netId;
		message.clientId = client.data.netid;
		message.position = client.playerData.position;

		ShibaNetLib::Network::conn.Send(&message, sizeof(ManifestUpdateMessage));
	}
	static void PostManifestSyncRequest() {
		if (!ShibaNetLib::Network::conn.isServer) {
			ManifestRequestMessage message;
			message.channelid = 4;
			message.senderid = ShibaNetLib::Network::conn.netId;
			ShibaNetLib::Network::conn.Send(&message, sizeof(ManifestRequestMessage));
		}
		else
			SyncManifest();
	}
	static void SyncManifest() {
		for (ClientObject client : NetworkInfo::clientManifest) {
			PostManifestUpdateRequest(client);
		}
	}
};

class ManifestRequestChannel : public ShibaNetLib::NetworkChannel {
	using ShibaNetLib::NetworkChannel::NetworkChannel;


	void Incoming(char* buffer) {
		if (conn->isServer)
			ClientManifestManager::SyncManifest();
	}
};


class ManifestUpdateChannel : public ShibaNetLib::NetworkChannel {
	using ShibaNetLib::NetworkChannel::NetworkChannel;

	void Incoming(char* buffer) {
		ManifestUpdateMessage* message = (ManifestUpdateMessage*)buffer;
		if (NetworkInfo::ClientExists(message->clientId)) {
			ClientObject* client = NetworkInfo::GetClient(message->clientId);
			client->playerData.position = message->position;
		}
		else {
			NetworkInfo::clientManifest.push_back(ClientObject(message->clientId, message->position));
		}

		if (conn->isServer)
			ClientManifestManager::SyncManifest();
	}
};
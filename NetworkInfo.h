#pragma once
#include "ClientObject.h"
#include <vector>

enum NetworkStatus {
	netstatus_offline,
	netstatus_client,
	netstatus_server
};
enum NetworkState {
	netstate_offline,
	netstate_failed,
	netstatus_ready,
	netstate_connected
};
class NetworkInfo
{
public:
	static NetworkStatus status;
	static NetworkState state;
	static int netid;
	static std::vector<ClientObject> clientManifest;

	static bool ClientExists(int id) {
		for (ClientObject client : clientManifest) {
			if (client.data.netid == id) {
				return true;
			}
		}
		return false;
	}
	static ClientObject* GetClient(int id) {
		for (int i = 0; i < clientManifest.size(); i++)
		{
			if (clientManifest[i].data.netid == id)
				return &clientManifest[i];
		}
	}
};


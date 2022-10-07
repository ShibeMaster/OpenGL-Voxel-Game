#pragma once
#include "ClientData.h"
#include "NetworkedPlayerContainer.h"
#include <NetworkBehaviour.h>

class ClientObject
{
public:
	ClientData data;
	NetworkedPlayerContainer playerData;

	ClientObject(){}
	ClientObject(int id, glm::vec3 position) {
		data = ClientData { id };
		playerData = { position };
	}
};


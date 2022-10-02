#pragma once
#include "ClientData.h"
#include "NetworkedPlayerContainer.h"
class ClientObject
{
public:
	ClientData data;
	NetworkedPlayerContainer playerData;

	ClientObject(){}
	ClientObject(int id, int x, int y, int z) {
		data = ClientData { id };
		playerData = { x, y, z };
	}
};


#pragma once
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer.h"
#include "Mesh.h"
#include "Syncvar.h"
#include "NetworkInfo.h"

#define DEFAULT_PORT "15142"
#define DEFAULT_BUFFER_LENGTH 1028

#define ENTIRE_MANIFEST_SYNC -1

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class NetworkBase {
public:
	SOCKET networkSocket = INVALID_SOCKET;
	virtual void Receive() {
		int recresult = 1;
		int buffer[DEFAULT_BUFFER_LENGTH];
		while (recresult > 0) {
			recresult = recv(networkSocket, (char*)&buffer, DEFAULT_BUFFER_LENGTH, 0);
			if (recresult > 0) {
				Process(buffer);
			}
		}
		std::cout << "connection ended" << std::endl;
	}
	virtual void Process(int data[]) {}
	virtual void Send(int data[], int length) {
		int sendRes = send(networkSocket, (char*)data, sizeof(int) * length, 0);
		if (sendRes == SOCKET_ERROR) {
			std::cout << "failed to send: " << WSAGetLastError() << std::endl;
			NetworkInfo::state = NetworkState::netstate_failed;
			closesocket(networkSocket);
			WSACleanup();
			return;
		}
	}

};


class ClientNetwork : public NetworkBase
{
public:
	void Connect(char** argv) {
		struct addrinfo* result = NULL, * ptr = NULL, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int addrresult = getaddrinfo(argv[1], "15142", &hints, &result);
		if (addrresult != 0) {
			std::cout << "getting address failed: " << addrresult << std::endl;
			WSACleanup();
			return;
		}

		for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

			networkSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (networkSocket == INVALID_SOCKET) {
				std::cout << "creating a socket failed: " << WSAGetLastError() << std::endl;
				NetworkInfo::state = NetworkState::netstate_failed;
				WSACleanup();
				NetworkInfo::state = NetworkState::netstate_failed;
				return;
			}
			int connectResult = connect(networkSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (connectResult == SOCKET_ERROR) {
				closesocket(networkSocket);
				networkSocket = INVALID_SOCKET;
			}
		}
		freeaddrinfo(result);
		if (networkSocket == INVALID_SOCKET) {
			std::cout << "failed to connect to server" << std::endl;
			NetworkInfo::state = NetworkState::netstate_failed;
			WSACleanup();
			return;
		}

		SendConnectMessage();
		Receive();
	}
	void Receive() {
		int result = 1;
		int buffer[DEFAULT_BUFFER_LENGTH];
		while (result > 0) {
			result = recv(networkSocket, (char*) &buffer, DEFAULT_BUFFER_LENGTH, 0);
			if (result > 0) {
				Process(buffer);
			}
		}
		NetworkInfo::state = NetworkState::netstate_failed;
		std::cout << "connection ended" << std::endl;
	}
	void Process(int data[]) {
		switch (data[0]) {
		case 0: // initializing
			NetworkInfo::netid = data[1];
			std::cout << "successfully joined server - client id: " << NetworkInfo::netid << std::endl;
			NetworkInfo::state = NetworkState::netstate_connected;
			NetworkInfo::clientManifest.push_back(ClientObject(NetworkInfo::netid, 0, 0, 0));
			PostManifestUpdate();
			break;
		case 1: // syncvars
			if (SyncvarManager::syncVars[data[2]].count(data[3]) > 0) {
				std::cout << "Set SyncInt " << data[3] << " from " << SyncvarManager::syncVars[data[2]][data[3]]->value << " to " << data[4] << std::endl;
				SyncvarManager::syncVars[data[2]][data[3]]->value = data[4];
			}
			break;
		case 3:
		{
			if (NetworkInfo::ClientExists(data[2])) {
				ClientObject* client = NetworkInfo::GetClient(data[2]);
				client->data.netid = data[2];
				client->playerData.x = data[3];
				client->playerData.y = data[4];
				client->playerData.z = data[5];
			}
			else {
				ClientObject client = ClientObject(data[2], data[3], data[4], data[5]);
				NetworkInfo::clientManifest.push_back(client);
			}
		}
			break;
		}
	}
	void PostManifestUpdate() {
		ClientObject* myClientObject = NetworkInfo::GetClient(NetworkInfo::netid);
		int data[] = { 3, NetworkInfo::netid, true, myClientObject->data.netid, myClientObject->playerData.x, myClientObject->playerData.y, myClientObject->playerData.z };
		Send(data, 8);
	}
	void RequestManifestUpdate() {
		int data[] = { 3, NetworkInfo::netid, false };
		Send(data, 4);
	}
	void SendConnectMessage() {
		int data[] = { 0 };
		Send(data, 1);
	}
};


class HostNetwork : public NetworkBase {
public:

	SOCKET listenSocket = INVALID_SOCKET;
	int clientCount = 0;

	void Host() {
		struct addrinfo* result = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int addrresult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (addrresult != 0) {
			std::cout << "failed to get address: " << addrresult << std::endl;
			NetworkInfo::state = NetworkState::netstate_failed;
			WSACleanup();
			return;
		}

		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET) {
			std::cout << "failed to create socket: " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			NetworkInfo::state = NetworkState::netstate_failed;
			WSACleanup();
			return;
		}

		int bindResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (bindResult == SOCKET_ERROR) {
			std::cout << "bind failed: " << bindResult << std::endl;
			freeaddrinfo(result);
			closesocket(listenSocket);
			NetworkInfo::state = NetworkState::netstate_failed;
			WSACleanup();
			return;
		}

		freeaddrinfo(result);

		if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
			std::cout << "failed to listen: " << WSAGetLastError() << std::endl;
			closesocket(listenSocket);
			NetworkInfo::state = NetworkState::netstate_failed;
			WSACleanup();
			return;
		}

		networkSocket = accept(listenSocket, NULL, NULL);
		if (networkSocket == INVALID_SOCKET) {
			std::cout << "accepting failed: " << WSAGetLastError() << std::endl;
			closesocket(listenSocket);
			NetworkInfo::state = NetworkState::netstate_failed;
			WSACleanup();
			return;
		}
		
		closesocket(listenSocket);
		
		NetworkInfo::netid = 0;
		NetworkInfo::state = NetworkState::netstate_connected;
		NetworkInfo::clientManifest.push_back(ClientObject(NetworkInfo::netid, 0, 0, 0));
		Receive();
	}
	void Process(int data[]) {
		switch (data[0]) {
		case 0: // assigning an id
		{
			int sendData[] = { 0, ++clientCount };
			std::cout << "new client connected - id: " << clientCount << std::endl;
			Send(sendData, 2);
		}
			break;
		case 1: // syncvars
			if (SyncvarManager::syncVars[data[2]].count(data[3]) > 0) {
				std::cout << "Set SyncInt " << data[3] << " from " << SyncvarManager::syncVars[data[2]][data[3]]->value << " to " << data[4] << std::endl;
				SyncvarManager::syncVars[data[2]][data[3]]->value = data[4];
			}
			Send(data, 6);
			break;
		case 3: // client manifest
		{
			if (data[2]) {
				if (NetworkInfo::ClientExists(data[3])) {
					ClientObject* client = NetworkInfo::GetClient(data[3]);
					client->data.netid = data[3];
					client->playerData.x = data[4];
					client->playerData.y = data[5];
					client->playerData.z = data[6];
				}
				else {
					ClientObject client = ClientObject(data[3], data[4], data[5], data[6]);
					NetworkInfo::clientManifest.push_back(client);
				}
				ManifestSync();
			}
			else {
				ManifestSync();
			}
		}
			break;
		}
	}
	void ManifestSync() {
		for (ClientObject client : NetworkInfo::clientManifest) {
			int data[6];
			data[0] = 3;
			data[1] = NetworkInfo::netid;
			data[2] = client.data.netid;
			data[3] = client.playerData.x;
			data[4] = client.playerData.y;
			data[5] = client.playerData.z;
			Send(data, 7);
		}
	}
};

class NetworkManager
{
public:
	static WSAData wsaData;
	static ClientNetwork client;
	static HostNetwork host;
	static std::thread networkThread;
	// Documentation Time!
	// right, so we're gonna want to keep a structure of how each buffer of information is formatted and what everything means
	// this is probably gonna change over time so just remember to change it here as well;
	// 
	// first piece of information - the type of information we're sending, not the data type but rather what we're going to be doing with it
	// 0 = a new client sending a connection message and the server replying with their id
	// 1 = a syncvar, more on this later
	// 2 = a network callback, also more on this later (probably wont be done at the start lol)
	// 3 (being sent from a client) = requesting an updated for that client's data in the server manifest
	// 3 (being sent from the serer) = sending manifest data to the client, this will probably be done in multiple messages for each client
	//
	// second piece of information - the network id of the client sending the packet (0 for the host), this won't need to appear on any this with the "0" type of information
	// as the host needs to register the new client;
	//
	// information from this point on will probably be different for each of the information types;
	//
	// Synvars!
	// syncvars are gonna be variables that get synced throughout the network with anyone who registers one on a specifc "id", this id will be put into a dictionary with the pointer to a variable
	// each syncvar will have a certain "owner client id" which will only allow it to be changed by that client.
	// syncvars can't be changed by just changing the variable, you'll need to call a "Hook(newValue)" method from the class
	// this is for the client to know that there's a change and send the updated syncvar info in this order:
	// - normal 2 first packets
	// - the type of the syncvar being change (0 = int, 1 = float, 2 = string)
	// - id of the syncvar being changed
	// - new value of the syncvar being changed
	// the server is then gonna send some information to all the clients telling them that the syncvar on that id has been changed in the same formatte as above, if the client has a syncvar registered on that id, it'll be changed
	// syncvars are probably going to be completely necssary to making this.
	//
	// Network Callbacks!
	// network callbacks are gonna be methods that clients can register to an "id", which will be put in a dictionary.
	// whenever the server sends out a packet containing the "2" information type, each client will call the method that is in the dictionary with the id that the server sends
	// I'll probably get arround to adding this when i actually need it.
	//
	// Client Manifest
	// the client manifest is going to be the main way of storing the information for each client.
	// each client can send their information to be synced with the server's manifest how often they please, this will also cause the host to do a server wide sync of the manifest by sending everyone the new data
	// if the client gets sent a manifest update for a netid that isn't registered on the client side, that client will need to add it and everything

	static void Initialize() {
		int initializeResult;
		initializeResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (initializeResult != 0) {
			std::cout << "startup failed: " << initializeResult;
			NetworkInfo::state = NetworkState::netstate_failed;
			return;
		}
		NetworkInfo::state = NetworkState::netstatus_ready;
	}
	static void Send(int data[], int length) {
		if (NetworkInfo::status == NetworkStatus::netstatus_server) host.Send(data, length);
		else if (NetworkInfo::status == NetworkStatus::netstatus_client) client.Send(data, length);
	}
	static void Connect(char **argv) {
		NetworkInfo::status = NetworkStatus::netstatus_client;
		networkThread = std::thread([argv] { client.Connect(argv); });
	}
	static void Host() {
		NetworkInfo::status = NetworkStatus::netstatus_server;
		networkThread = std::thread([] { host.Host(); });
	}

	static void HookSyncvar(int newValue, Syncvar* syncvar) {
		int data[5]{};
		data[0] = 1;
		data[1] = NetworkInfo::netid;
		data[2] = syncvar->ownerId;

		
		data[4] = newValue;
		data[3] = syncvar->id;
		
		if (NetworkInfo::status == NetworkStatus::netstatus_server)
			host.Process(data);
		else 
			Send(data, 6);
	}
};
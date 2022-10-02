#include "NetworkManager.h"

ClientNetwork NetworkManager::client;
HostNetwork NetworkManager::host;
WSAData NetworkManager::wsaData;
std::thread NetworkManager::networkThread;


#include "NetworkInfo.h"

NetworkStatus NetworkInfo::status = NetworkStatus::netstatus_offline;
NetworkState NetworkInfo::state = NetworkState::netstate_offline;

std::vector<ClientObject> NetworkInfo::clientManifest;
int NetworkInfo::netid = 0;
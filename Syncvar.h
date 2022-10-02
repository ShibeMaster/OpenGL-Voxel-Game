#pragma once
#include <map>
class Syncvar;
class SyncvarManager;


class SyncvarManager {
public:
	static std::map<int, std::map<int, Syncvar*>> syncVars;
	static void AddSyncvar(Syncvar* syncvar, int netid, int id) {
		syncVars[netid][id] = (Syncvar*)syncvar;
	}
};


class Syncvar
{
public:
	int value;
	int ownerId;
	int id;

	Syncvar() {}
	Syncvar(int value, int ownernetid, int id, Syncvar* self) {
		this->value = value;
		this->id = id;
		this->ownerId = ownernetid;
		SyncvarManager::AddSyncvar(self, ownernetid, id);
	}
};

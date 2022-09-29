#include "PlayerModules.h"
void PlayerModules::Initialize(PlayerData* data) {
	movement.Initialize(data);
	placing.Initialize(data);
}
void PlayerModules::Update() {
	placing.Update();
}
void PlayerModules::FixedUpdate() {
	movement.Update();
}
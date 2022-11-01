#include "PlayerModules.h"
void PlayerModules::Initialize(PlayerData* data) {
	movement.Initialize(data);
	placing.Initialize(data);
}
void PlayerModules::Update() {
	movement.Update();
	placing.Update();
}
void PlayerModules::FixedUpdate() {
}
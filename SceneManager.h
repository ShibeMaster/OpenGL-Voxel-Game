#pragma once
#include "Scene.h"
#include "World.h"

class SceneManager {
public:
	static Scene* activeScene;
	static World world;

	static void SetActiveScene(Scene* scene) {
		activeScene = scene;
		activeScene->Start();
	}
};
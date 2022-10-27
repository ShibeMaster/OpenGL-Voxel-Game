#pragma once
#include "Scene.h"
#include "GUIManager.h"
#include "ModelEditor.h"
#include "World.h"

class SceneManager {
public:
	static Scene* activeScene;
	static World world;
	static ModelEditor editor;

	static void SetActiveScene(Scene* scene) {
		GUIManager::OnSceneStart();
		activeScene = scene;
		activeScene->Start();
	}
};
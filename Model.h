#pragma once
#include <vector>
#include "Mesh.h"
#include <map>
#include <string>
#include "MathsExtensions.h"
#include "Transform.h"
#include "Renderer.h"

struct Object {
	std::string name;
	Transform transform;
	std::vector<Vertex> vertices;
	Mesh mesh;
};

struct ObjectChange {
	std::string name;
	Transform transform;
};
struct Frame {
	float time;
	std::vector<ObjectChange> changes;
};
struct Animation {
	bool looping = false;
	std::vector<Frame> frames;
};
class Model {
public:
	std::string name;
	Renderer renderer;
	std::map<std::string, Object> objects;
	std::map<std::string, Transform> startingObjectPositions;
	std::map<std::string, Transform> lastFrameObjectPositions;
	bool hasAnimation;
	bool inAnimation;
	Animation animation;
	int currentFrame;
	float lastFrameTime;
	float animationStartTime;

	float pauseStartTime;
	bool isPaused = false;

	void StartAnimation() {
		currentFrame = 0;
		inAnimation = true;
		std::cout << "started animation" << std::endl;
		lastFrameTime = 0;
		animationStartTime = glfwGetTime();
		for (auto object : objects) {
			startingObjectPositions[object.second.name] = object.second.transform;
			lastFrameObjectPositions[object.second.name] = object.second.transform;
		}
		AnimationUpdate();
	}
	void Pause() {
		if (!isPaused) {

			isPaused = true;
			pauseStartTime = glfwGetTime();
			inAnimation = false;
		}
	}
	void Unpause() {
		if (isPaused) {
			animationStartTime = glfwGetTime() - (pauseStartTime - animationStartTime);
			isPaused = false;
			inAnimation = true;
		}

	}
	void EndAnimation() {

		for (auto startingTransform : startingObjectPositions)
			objects[startingTransform.first].transform = startingTransform.second;
		inAnimation = false;
	}
	void NextAnimationFrame() {
		std::cout << "new animation keyframe" << std::endl;
		for (auto object : objects) {
			lastFrameObjectPositions[object.second.name] = object.second.transform;
		}
		if (currentFrame < animation.frames.size()-1) {
			lastFrameTime = animation.frames[currentFrame].time;
			currentFrame++;
		}
		else {
			if (animation.looping) {
				lastFrameTime = 0.0f;
				currentFrame = 0;
				animationStartTime = glfwGetTime();
			}
			else
				EndAnimation();
		}
	}
	void AnimationUpdate() {
		if (glfwGetTime() - animationStartTime < animation.frames[currentFrame].time) {
			float framePercentage = MathsExtensions::InverseLerp(lastFrameTime, animation.frames[currentFrame].time, glfwGetTime() - animationStartTime);

			for (ObjectChange change : animation.frames[currentFrame].changes) {
				glm::vec3 objectPosition = glm::vec3(std::lerp(lastFrameObjectPositions[change.name].position.x, change.transform.position.x, framePercentage), std::lerp(lastFrameObjectPositions[change.name].position.y, change.transform.position.y, framePercentage), std::lerp(lastFrameObjectPositions[change.name].position.z, change.transform.position.z, framePercentage));
				glm::vec3 objectRotation = glm::vec3(std::lerp(lastFrameObjectPositions[change.name].rotation.x, change.transform.rotation.x, framePercentage), std::lerp(lastFrameObjectPositions[change.name].rotation.y, change.transform.rotation.y, framePercentage), std::lerp(lastFrameObjectPositions[change.name].rotation.z, change.transform.rotation.z, framePercentage));
				objects[change.name].transform.position = objectPosition;
				objects[change.name].transform.rotation = objectRotation;
			}
			return;
		}
		else {
			NextAnimationFrame();
		}
	}

	void Generate() {
		for (auto object : objects) {
			objects[object.first].mesh = Mesh(objects[object.first].vertices);
		}
	}

	void Render(glm::vec3 position) {
		renderer.shader.Use();
		for (auto object : objects) {
			renderer.shader.SetMat4("model", object.second.transform.GetMatrix(position));
			object.second.mesh.DrawMesh();
		}
	}
};
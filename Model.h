#pragma once
#include <vector>
#include "Mesh.h"
#include <string>
#include "Transform.h"
#include "Renderer.h"

struct Object {
	Transform transform;
	std::vector<Vertex> vertices;
	Mesh mesh;
};

class Model {
public:
	std::string name;
	Renderer renderer;
	std::vector<Object> objects;

	void Generate() {
		for (int i = 0; i < objects.size(); i++) {
			objects[i].mesh = Mesh(objects[i].vertices);
		}
	}

	void Render(glm::vec3 position) {
		renderer.shader.Use();
		for (Object object : objects) {
			renderer.shader.SetMat4("model", object.transform.GetMatrix(position));
			object.mesh.DrawMesh();
		}
	}
};
#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include <GL/glew.h>

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
};
class Mesh
{
public:
	std::vector<Vertex> vertices;
	GLuint vbo;
	GLuint vao;

	void DrawMesh() {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}

	Mesh() {}
	Mesh(std::vector<Vertex> vertices) {
		this->vertices = vertices;
		InitializeMesh();
	}

	void InitializeMesh() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
};

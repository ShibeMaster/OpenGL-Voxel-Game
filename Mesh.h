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

	void DrawMesh();
	Mesh();
	Mesh(std::vector<Vertex> vertices);
	void InitializeMesh();
};

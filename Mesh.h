#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

enum GUIAlignment {
	alignment_left,
	alignment_center,
	alignment_right
};

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

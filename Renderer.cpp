#include "Renderer.h"
#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

void Renderer::Initialize(const char* vertexSource, const char* fragmentSource) {

	GLuint vbo;
	
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	shader = Shader(vertexSource, fragmentSource);

	shader.Use();
}
void Renderer::Render(GameObject& gameObject) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, gameObject.position);
	model = glm::rotate(model, glm::radians(1.0f), gameObject.rotation);
	model = glm::scale(model, gameObject.scale);

	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void Renderer::RenderCubeFace(CubeFace face, glm::vec3 position, glm::vec4 color) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 6*face, 6);
}
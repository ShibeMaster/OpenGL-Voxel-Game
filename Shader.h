#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	GLuint id;

	Shader();
	Shader(const char* vertexSource, const char* fragmentSource);
	void Use();
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetBool(const std::string& name, const bool value) const;
};
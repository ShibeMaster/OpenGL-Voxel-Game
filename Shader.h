#pragma once
#ifndef SHADER_H
#define SHADER_H

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

	Shader(){}
	Shader(const char* vertexSource, const char* fragmentSource) {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		int vertexSuccess;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
		std::cout << vertexSuccess << std::endl;

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		int fragmentSuccess;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
		std::cout << fragmentSuccess << std::endl;

		id = glCreateProgram();
		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		glLinkProgram(id);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	void Use() {
		glUseProgram(id);
	}
	void SetMat4(const std::string &name, const glm::mat4 &value) const {
		glProgramUniformMatrix4fv(id, glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}
	void SetVec4(const std::string& name, const glm::vec4 &value) const {
		glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}
	void SetFloat(const std::string& name, const float value) const {
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}
	void SetBool(const std::string& name, const bool value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
};
#endif
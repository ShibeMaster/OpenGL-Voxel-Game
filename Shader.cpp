#include "Shader.h"
Shader::Shader() {}

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	int vertexSuccess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
	std::cout << "vertex status: " << vertexSuccess << std::endl;

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	int fragmentSuccess;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
	std::cout << "fragment status: " << fragmentShader << std::endl;

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shader::Use() {
	glUseProgram(id);
}
void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
	glProgramUniformMatrix4fv(id, glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::SetFloat(const std::string& name, const float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::SetBool(const std::string& name, const bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
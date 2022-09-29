#include "Shaders.h"
const char* Shaders::vertexSource = R"GLSL(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	Color = color;
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)GLSL";

const char* Shaders::hudVertexSource = R"GLSL(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 Color;
uniform mat4 model;


void main(){
	Color = color;
	gl_Position = model * vec4(pos, 1.0f);
}
)GLSL";

const char* Shaders::fragmentSource = R"GLSL(
#version 330 core

in vec4 Color;

out vec4 FragColor;

void main(){
	FragColor = Color;
}
)GLSL";
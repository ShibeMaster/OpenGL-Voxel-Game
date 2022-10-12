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

const char* Shaders::textVertexSource = R"GLSL(
#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoord;

uniform mat4 projection;

void main(){
	gl_Position = projection * vec4(vertex.xy, 0.1, 1.0);
	TexCoord = vertex.zw;
}
)GLSL";
const char* Shaders::textFragmentSource = R"GLSL(
#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main(){
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoord).r);
	color = vec4(textColor, 1.0) * sampled;
}
)GLSL";

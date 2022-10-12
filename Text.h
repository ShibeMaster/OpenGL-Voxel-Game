#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <ft2build.h>
#include <iostream>
#include "Renderer.h"
#include "Shader.h"
#include "WindowInfo.h"
#include "Shaders.h"
#include FT_FREETYPE_H



struct Character {
	unsigned int textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;

};

class Font {
public:
	static std::map<char, Character> characters;

	static float GetStringLength(std::string text, float scale) {
		float length = 0;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			length += ((characters[*c].advance >> 6) * scale);
		}
		return length;
	}
	static void Initialize() {

		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "Failed to initialize this freetype library" << std::endl;
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
			std::cout << "Failed to load font" << std::endl;
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "Failed to load glyph: " << c << std::endl;
				continue;
			}

			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			characters[c] = character;
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};

class TextRenderer {
public:
	Shader shader;
	GLuint vao, vbo;

	void Initialize(const char* vertexSource, const char* fragmentSource) {

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);



		shader = Shader(vertexSource, fragmentSource);
		shader.Use();
	

		glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
		shader.SetMat4("projection", projection);
	}
	void RenderText(std::string text, glm::vec2 position, float scale, glm::vec3 color) {
		shader.Use();
		shader.SetVec3("textColor", color);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vao);
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			
			Character ch = Font::characters[*c];

			float xpos = position.x + ch.bearing.x * scale;
			float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, ch.textureId);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			position.x += (ch.advance >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};


class Text {
public:
	static TextRenderer renderer;
	static void Initialize() {
		renderer.Initialize(Shaders::textVertexSource, Shaders::textFragmentSource);
	}
	static void RenderText(std::string text, glm::vec2 position, float scale, glm::vec3 color) {
		renderer.RenderText(text, position, scale, color);
	}
};
#pragma once
#include <ft2build.h>
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include "Mesh.h"
#include "Text.h"

class GUIText {
public:
	glm::vec2 position;
	glm::vec3 color;
	float scale;
	std::string text;
	GUIAlignment alignment;

	GUIText(){}
	GUIText(std::string text, glm::vec2 position, float scale, glm::vec3 color, GUIAlignment alignment) {
		this->text = text;
		this->position = position;
		this->scale = scale;
		this->color = color;
		this->alignment = alignment;
	}
	glm::vec2 GetOffsetPosition() {
		switch (alignment)
		{
		case GUIAlignment::alignment_left: return glm::vec2(this->position.x - Font::GetStringLength(text, scale), this->position.y); break;
		case GUIAlignment::alignment_center: return glm::vec2(this->position.x - (Font::GetStringLength(text, scale) / 2), this->position.y); break;
		default:
			break;
		}
	}
	void Render() {
		Text::RenderText(text, GetOffsetPosition(), scale, color);
	}
};
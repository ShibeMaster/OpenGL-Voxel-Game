#pragma once
#include <ft2build.h>
#include <iostream>
#include <glm/glm.hpp>
#include <string>
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

		switch (alignment)
		{
		case GUIAlignment::alignment_left: this->position.x -= Font::GetStringLength(text, scale); break;
		case GUIAlignment::alignment_center: this->position.x -= (Font::GetStringLength(text, scale) / 2); break;
		default:
			break;
		}
	}

	void Render() {
		Text::RenderText(text, position, scale, color);
	}
};
#pragma once
#include "GUIButton.h"

class GUICheckbox : public GUIButton {
public:
	bool checked = false;
	Mesh selectedMesh;

	GUICheckbox(){}
	GUICheckbox(glm::vec2 position, glm::vec2 dimensions, glm::vec4 colour, glm::vec4 checkedColour, GUIAlignment alignment, std::string text, glm::vec3 textColour) : GUIButton(position, dimensions, colour, alignment, text, textColour) {
		selectedMesh = GUIUtils::GenerateMesh(this->position + (this->dimensions * 0.2f), this->dimensions - (this->dimensions * 0.4f), checkedColour, alignment);
		this->text = GUIText(text, glm::vec2(this->position.x - 20.0f, this->position.y + (this->dimensions.y / 2) - 5.0f), 0.5f, textColour, GUIAlignment::alignment_left);
	}

	void Render(Renderer& renderer) {
		renderer.shader.Use();
		if (checked)
			selectedMesh.DrawMesh();
		GUIButton::Render(renderer);
		text.Render();
	}
	void OnClicked() {
		checked = !checked;
	}
};
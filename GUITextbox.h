#pragma once
#include "GUIItem.h"
#include "GUIText.h"
class GUITextbox : public GUIItem {
public:
	std::string text;
	GUIText captionText;
	GUIText textGui;
	bool selected = false;
	int maxCharacters;

	GUITextbox(){}
	GUITextbox(glm::vec2 position, glm::vec2 dimensions, glm::vec4 color, glm::vec4 textColor, int maxcharacters, std::string startingText, std::string captionText, GUIAlignment alignment) : GUIItem(position, dimensions, color, alignment) {
		this->maxCharacters = maxcharacters;
		this->text = text;
		this->textGui = GUIText(text, glm::vec2(this->position.x + (this->dimensions.x / 2), this->position.y + (this->dimensions.y / 2) - 7.5f), 0.6f, textColor, GUIAlignment::alignment_center);
		this->captionText = GUIText(captionText, glm::vec2(this->position.x - 20.0f, this->position.y + (this->dimensions.y / 2) - 5.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), GUIAlignment::alignment_left);
	}
	void CheckForPress(GLFWwindow* window, int button, int action) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			std::cout << selected << std::endl;
			selected = InputManager::mouse.position.x > position.x && InputManager::mouse.position.x < position.x + dimensions.x && InputManager::mouse.position.y > position.y && InputManager::mouse.position.y < position.y + dimensions.y;
		}
	}
	void OnDelete() {
		if (selected && text.size() > 0) {
			text.pop_back();
			textGui.text = text;
		}
	}
	void CharCallback(GLFWwindow* window, int codepoint) {
		if (selected) {
			if (text.length() < maxCharacters) {
				text += (unsigned char)codepoint;
				textGui.text = text;
			}
		}
	}
	void Render(Renderer& renderer) {
		renderer.shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);
		mesh.DrawMesh();
		captionText.Render();
		textGui.Render();
	}
};
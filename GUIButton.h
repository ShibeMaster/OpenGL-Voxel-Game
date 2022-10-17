#pragma once
#include "GUIItem.h"
#include "InputManager.h"
#include "GUIText.h"
#include <string>

class GUIButton : public GUIItem {
protected:
	float cooldown = 0.5f;
	float lastPressedTime = 0.0f;
public:
	bool interactable = true;
	GUIText text;
	glm::vec2 textPosition;


	GUIButton(){}
	GUIButton(glm::vec2 position, glm::vec2 dimensions, glm::vec4 colour, GUIAlignment alignment, std::string text, glm::vec3 textColor) : GUIItem(position, dimensions, colour, alignment) {
		this->text = GUIText(text, glm::vec2(this->position.x + (this->dimensions.x / 2), this->position.y + (this->dimensions.y / 2)-5.0f), 0.5f, textColor, GUIAlignment::alignment_center);
	}

	virtual void CheckForPress(GLFWwindow* window, int button, int action) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS && interactable && glfwGetTime() - cooldown > lastPressedTime) {
			if (InputManager::mouse.position.x > position.x && InputManager::mouse.position.x < position.x + dimensions.x && InputManager::mouse.position.y > position.y && InputManager::mouse.position.y < position.y + dimensions.y) {
				lastPressedTime = glfwGetTime();
				OnClicked();
			}
		}
	}
	virtual void Render(Renderer& renderer) {
		renderer.shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		renderer.shader.SetMat4("model", model);
		mesh.DrawMesh();
		text.Render();
	}
	virtual void OnClicked(){}
};
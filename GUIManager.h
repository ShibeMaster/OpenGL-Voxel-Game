#pragma once
#include "GUISlider.h"
#include "GUIItem.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GUIButton.h"
#include "GUITextbox.h"
class GUIManager {
public:
	static std::vector<GUITextbox*> textboxs;
	static std::vector<GUIButton*> buttons;
	static std::vector<GUISlider*> sliders;
	static Renderer renderer;
	static void RegisterTextbox(GUITextbox* textbox) {
		textboxs.push_back(textbox);
	}
	static void RegisterButton(GUIButton* button) {
		buttons.push_back(button);
	}
	static void RegisterSlider(GUISlider* slider) {
		sliders.push_back(slider);
	}
	static void CharCallback(GLFWwindow* window, unsigned int codepoint) {
		for (GUITextbox* textbox : textboxs)
			textbox->CharCallback(window, codepoint);
	}
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
			for (GUITextbox* textbox : textboxs)
				textbox->OnDelete();
	}
	static void Initialize() {
		renderer.Initialize(Shaders::hudVertexSource, Shaders::fragmentSource);
		glfwSetCharCallback(WindowInfo::window, CharCallback);
		glfwSetMouseButtonCallback(WindowInfo::window, MouseCallback);
	}
	
	static void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
		for (GUIButton* guibutton : buttons) {
			guibutton->CheckForPress(window, button, action);
		}
		for (GUISlider* slider : sliders) {
			slider->UpdateSliderPercent(window, button, action);
		}
		for (GUITextbox* textbox : textboxs) {
			textbox->CheckForPress(window, button, action);
		}
	}
};
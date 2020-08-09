#include "lucipch.h"

#include "Luci/Core/Input/Input.h"
#include "Luci/Core/Input/KeyCodes.h"
#include "Luci/Core/Input/MouseCodes.h"
#include "Luci/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Luci {

	bool Input::IsKeyPressed(KeyCode keyCode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, static_cast<int>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX() {
		auto [xPos, _] = GetMousePosition();
		return xPos;
	}

	float Input::GetMouseY() {
		auto [_, yPos] = GetMousePosition();
		return yPos;
	}

}

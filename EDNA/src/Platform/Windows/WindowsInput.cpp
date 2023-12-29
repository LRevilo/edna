#include "ednapch.h"
#include "EDNA/Core/Input.h"

#include <GLFW/glfw3.h>
#include "EDNA/Core/Application.h"

namespace EDNA {


	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	std::pair<float, float> Input::GetMouseClientPosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int window_width = Application::Get().GetWindow().GetWidth();
		int window_height = Application::Get().GetWindow().GetHeight();

		xpos = std::clamp((float)xpos, 0.f, (float)window_width);
		ypos = std::clamp((float)ypos, 0.f, (float)window_height);


		return { (float)xpos, (float)ypos };
	}



	float Input::GetMouseX()
	{
		return GetMousePosition().first;
	}
	float Input::GetMouseY()
	{
		return GetMousePosition().second;
	}
}
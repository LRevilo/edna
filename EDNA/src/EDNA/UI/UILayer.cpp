#include "ednapch.h"
#include "UILayer.h"






#include "EDNA/Core/Application.h"
// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>



namespace EDNA {

	UILayer::UILayer()
		: Layer("UILayer")
	{
	}
	UILayer::~UILayer()
	{
	}

	void UILayer::OnAttach()
	{

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());


	}
	void UILayer::OnDetach()
	{

	}
	void UILayer::OnUpdate(Timestep ts)
	{
	}

	void UILayer::OnImGuiRender()
	{
	}

	void UILayer::OnEvent(Event& event)
	{
	}



	void UILayer::Begin()
	{

	}

	void UILayer::End()
	{

		Application& app = Application::Get();



	}




}
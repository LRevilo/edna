#include "ednapch.h"
#include "Application.h"


#include "EDNA/Core/Log.h"

#include "EDNA/Renderer/Renderer.h"
#include "EDNA/Core/Input.h"

//#include "EDNA/Audio/AudioEngine.h"


#include <GLFW/glfw3.h>


namespace EDNA {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	

	

	Application::Application()
	{
		EDNA_CORE_INFO("Application Constructor");
		EDNA_PROFILE_FUNCTION();

		EDNA_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//SoundDevice::Get();
		//SoundDevice::Get()->Init();

		Renderer::Init();
		
		Audio::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_UILayer = new UILayer();
		PushOverlay(m_UILayer);
		//----------



	}

	Application::~Application()
	{
	}



	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		//EDNA_CORE_TRACE("{0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));


		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			// if event is handled, it doesn't get sent to the lower layers
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		

		while (m_Running)
		{
			EDNA_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
		
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;



			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_UILayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnUIRender();
			m_UILayer->End();


			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}


	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}

#pragma once

#include "Core.h"

#include "Window.h"
#include "EDNA/LayerStack.h"
#include "Events/Event.h"
#include "EDNA/Events/ApplicationEvent.h"



namespace EDNA {

	class EDNA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}


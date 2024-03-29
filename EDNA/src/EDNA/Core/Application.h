#pragma once

#include "Core.h"

#include "Window.h"
#include "EDNA/Core/LayerStack.h"
#include "EDNA/Events/Event.h"
#include "EDNA/Events/ApplicationEvent.h"
#include "EDNA/ImGui/ImGuiLayer.h"
#include "EDNA/UI/UILayer.h"
#include "EDNA/Audio/AudioEngine.h"

#include "EDNA/Core/Timestep.h"


namespace EDNA {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);


		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


	
		inline Window& GetWindow() {return *m_Window;}
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


	public:


	private:

		std::unique_ptr<Window> m_Window;
		UILayer* m_UILayer;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		



		 

	private:
		static Application* s_Instance;
		
		
	};

	// To be defined in client
	Application* CreateApplication();
}


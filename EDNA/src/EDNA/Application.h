#pragma once

#include "Core.h"

#include "Window.h"
#include "EDNA/LayerStack.h"
#include "Events/Event.h"
#include "EDNA/Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"


//temp
#include "EDNA/Renderer/Shader.h"
#include "EDNA/Renderer/Buffer.h"
#include "EDNA/Renderer/VertexArray.h"

#include "Renderer/OrthographicCamera.h"

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

	
		inline Window& GetWindow() {return *m_Window;}
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;


		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		OrthographicCamera m_Camera;
		 

	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}


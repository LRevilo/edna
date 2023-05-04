#include <EDNA.h>
#include "imgui/imgui.h"


class ExampleLayer : public EDNA::Layer {

public:
	ExampleLayer()
		: Layer("ExampleLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		
		m_VertexArray.reset(EDNA::VertexArray::Create());

		float vertices[3 * 7] = {

			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};


		std::shared_ptr<EDNA::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(EDNA::VertexBuffer::Create(vertices, sizeof(vertices)));

		EDNA::BufferLayout layout = {
			{ EDNA::ShaderDataType::Float3, "a_Position"},
			{ EDNA::ShaderDataType::Float4, "a_Colour"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };

		std::shared_ptr<EDNA::IndexBuffer> indexBuffer;
		indexBuffer.reset(EDNA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc =
			R"(

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;

uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec4 v_Colour;

void main()
{
	v_Position = a_Position;
	v_Colour = a_Colour;
	gl_Position = u_ViewProjection * vec4(a_Position,1.0);
}

)";

		std::string fragmentSrc =
			R"(

#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Colour;

void main()
{
	color = v_Colour;
}

)";


		m_Shader.reset(new EDNA::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate(EDNA::Timestep ts) override
	{
		/*
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_TAB))
			EDNA_TRACE("Tab key is pressed (poll)!");
			*/
		EDNA_TRACE("Delta time: {0}", ts.GetSeconds());


		EDNA::RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 1.0 });
		EDNA::RenderCommand::Clear();

		m_Camera.SetRotation(45.0f);
		m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });

		EDNA::Renderer::BeginScene(m_Camera);

		EDNA::Renderer::Submit(m_Shader, m_VertexArray);

		EDNA::Renderer::EndScene();


	}

	virtual void OnImGuiRender() override
	{
		/*
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
		*/
	}

	void OnEvent(EDNA::Event& event) override
	{
		/*
		if (event.GetEventType() == EDNA::EventType::KeyPressed)
		{
			EDNA::KeyPressedEvent& e = (EDNA::KeyPressedEvent&)event;
			if (e.GetKeyCode() == EDNA_KEY_TAB)
				EDNA_TRACE("Tab key is pressed (event)!");
			EDNA_TRACE("{0}", (char)e.GetKeyCode());
		}
		*/
	}

private:
	std::shared_ptr<EDNA::Shader> m_Shader;
	std::shared_ptr<EDNA::VertexArray> m_VertexArray;
	EDNA::OrthographicCamera m_Camera;
};


class GamePrototype : public EDNA::Application
{
public:
	GamePrototype() 
	{
		PushLayer(new ExampleLayer());

	}
	~GamePrototype() 
	{

	}
};

EDNA::Application* EDNA::CreateApplication()
{
	return new GamePrototype();
}     
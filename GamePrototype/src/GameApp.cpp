#include <EDNA.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public EDNA::Layer {

public:
	ExampleLayer()
		: Layer("ExampleLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		
		m_VertexArray.reset(EDNA::VertexArray::Create());

		float vertices[4 * 5] = {

			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};


		EDNA::Ref<EDNA::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(EDNA::VertexBuffer::Create(vertices, sizeof(vertices)));

		EDNA::BufferLayout layout = {
			{ EDNA::ShaderDataType::Float3, "a_Position"},
			{ EDNA::ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0,1,2,2,3,0 };

		EDNA::Ref<EDNA::IndexBuffer> indexBuffer;
		indexBuffer.reset(EDNA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc =
			R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec4 v_Colour;

void main()
{
	v_Position = a_Position;
	v_Colour = a_Colour;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
}
)";

		std::string fragmentSrc =
			R"(
#version 330 core
layout(location = 0) out vec4 color;

uniform vec3 u_Colour;

in vec3 v_Position;
in vec3 v_Colour;

void main()
{
	color = vec4(u_Colour,1.0f);
}
)";


		m_Shader.reset(EDNA::Shader::Create(vertexSrc, fragmentSrc));


		std::string texVertexSrc =
			R"(

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
}
)";

		std::string texFragmentSrc =
			R"(

#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec3 u_Colour;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture,v_TexCoord);
}
)";

		m_TextureShader.reset(EDNA::Shader::Create(texVertexSrc, texFragmentSrc));


		m_Texture = EDNA::Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_ChernoTexture = EDNA::Texture2D::Create("Assets/Textures/ChernoLogo.png");

		std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(EDNA::Timestep ts) override
	{
		/*
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_TAB))
			EDNA_TRACE("Tab key is pressed (poll)!");
			*/

		//EDNA_TRACE("Delta time: {0}", ts.GetSeconds());

		if (EDNA::Input::IsKeyPressed(EDNA_KEY_W))
			m_ObjPosition.y += m_ObjMoveSpeed * ts;
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_S))
			m_ObjPosition.y -= m_ObjMoveSpeed * ts;
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_A))
			m_ObjPosition.x -= m_ObjMoveSpeed * ts;
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_D))
			m_ObjPosition.x += m_ObjMoveSpeed * ts;

		if (EDNA::Input::IsKeyPressed(EDNA_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (EDNA::Input::IsKeyPressed(EDNA_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;


		EDNA::RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 1.0 });
		EDNA::RenderCommand::Clear();

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);

		EDNA::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));


		std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Colour", m_Colour);

		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {

				glm::vec3 offset(x * 0.6f, y * 0.6f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ObjPosition + offset)*scale;

				m_Texture->Bind();
				EDNA::Renderer::Submit(m_TextureShader, m_VertexArray, transform);
				m_ChernoTexture->Bind();
				EDNA::Renderer::Submit(m_TextureShader, m_VertexArray, transform);

			}
		}
	

		//EDNA::Renderer::Submit(m_Shader, m_VertexArray, transform);
		EDNA::Renderer::EndScene();


	}

	virtual void OnImGuiRender() override
	{
		
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Shader Colour", glm::value_ptr(m_Colour));
		ImGui::End();
		
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
	EDNA::Ref<EDNA::Shader> m_Shader;
	EDNA::Ref<EDNA::VertexArray> m_VertexArray;

	EDNA::Ref<EDNA::Shader> m_TextureShader;

	EDNA::Ref<EDNA::Texture2D> m_Texture;
	EDNA::Ref<EDNA::Texture2D> m_ChernoTexture;


	EDNA::OrthographicCamera m_Camera;
	float m_ObjMoveSpeed = 5.0f;
	glm::vec3 m_ObjPosition = glm::vec3(0.0f);

	glm::vec3 m_CameraPosition = glm::vec3(0.0f);
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
	glm::vec3 m_Colour = glm::vec3(0.8f, 0.3f, 0.2f);

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
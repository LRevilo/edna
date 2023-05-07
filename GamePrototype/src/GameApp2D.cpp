#include "GameApp2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


GameApp2D::GameApp2D()
	: Layer("GameApp2D"), m_CameraController(1280.0f/720.0f)
{

}

void GameApp2D::OnAttach()
{
	
	m_SquareVA= EDNA::VertexArray::Create();

	float squareVertices[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};


	EDNA::Ref<EDNA::VertexBuffer> squareVB;
	squareVB = EDNA::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	EDNA::BufferLayout layout = {
		{ EDNA::ShaderDataType::Float3, "a_Position"},
	};

	squareVB->SetLayout(layout);
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0,1,2,2,3,0 };

	EDNA::Ref<EDNA::IndexBuffer> squareIB;
	squareIB = EDNA::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = EDNA::Shader::Create("Assets/Shaders/FlatColor.glsl");




}

void GameApp2D::OnDetach()
{
}

void GameApp2D::OnUpdate(EDNA::Timestep ts)
{
	//update
	m_CameraController.OnUpdate(ts);

	EDNA::RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 1.0 });
	EDNA::RenderCommand::Clear();


	EDNA::Renderer::BeginScene(m_CameraController.GetCamera());


	std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);


	EDNA::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));

	EDNA::Renderer::EndScene();

}

void GameApp2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void GameApp2D::OnEvent(EDNA::Event& e)
{
	m_CameraController.OnEvent(e);
}

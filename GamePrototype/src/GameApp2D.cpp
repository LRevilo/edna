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
	
	m_CheckerBoardTexture = EDNA::Texture2D::Create("Assets/Textures/Checkerboard.png");




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


	EDNA::Renderer2D::BeginScene(m_CameraController.GetCamera());

	EDNA::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	EDNA::Renderer2D::DrawQuad({ 0.5f, -0.5f }, {0.5f, 0.5f }, { 0.3f, 0.2f, 0.8f, 1.0f });
	EDNA::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerBoardTexture);

	EDNA::Renderer2D::EndScene();

	//EDNA::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));
	//std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
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

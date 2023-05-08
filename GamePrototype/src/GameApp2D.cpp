#include "ednapch.h"
#include "GameApp2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <chrono>



GameApp2D::GameApp2D()
	: Layer("GameApp2D"), m_CameraController(1280.0f/720.0f)
{

}

void GameApp2D::OnAttach()
{
	EDNA_PROFILE_FUNCTION();
	m_CheckerBoardTexture = EDNA::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_OverworldTilesTexture = EDNA::Texture2D::Create("Assets/Textures/OverworldTiles.png");
	m_ShrubTile = EDNA::SubTexture2D::CreateFromCoords(m_OverworldTilesTexture, { 7,4 }, { 16,16 }, { 1,1 });

	EDNA::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = EDNA::Framebuffer::Create(fbSpec);

}

void GameApp2D::OnDetach()
{
}

void GameApp2D::OnUpdate(EDNA::Timestep ts)
{
	EDNA_PROFILE_FUNCTION();
	//update
	m_Framebuffer->Bind();
	m_CameraController.OnUpdate(ts);

	EDNA::Renderer2D::ResetStats();

		EDNA::RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 1.0 });
		EDNA::RenderCommand::Clear();


	{
		EDNA_PROFILE_SCOPE("Renderer Draw");

		EDNA::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//EDNA::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f },3.141592f/6.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		EDNA::Renderer2D::DrawQuad({ 0.5f, 0.0f, 0.0f }, { 0.5f, 0.5f }, { 0.8f, 0.3f, 0.2f, 1.0f });

		EDNA::Renderer2D::DrawQuad({ -1.0f, -0.5f, 0.0f }, { 0.5f, 0.5f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		EDNA::Renderer2D::DrawQuad({ -0.0f, -0.0f, -0.9f }, { 10.0f, 10.0f }, m_CheckerBoardTexture, 5.0f);
		EDNA::Renderer2D::DrawRotatedQuad({ -0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f },3.141592f/4.0f, m_CheckerBoardTexture, 10.0f, glm::vec4(1.0f,1.0f,0.5f,1.0f));
		EDNA::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 0.5f, 0.5f }, m_ShrubTile, 1.0f);
		EDNA::Renderer2D::EndScene();

	}


	m_Framebuffer->Unbind();


	//EDNA::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));
	//std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<EDNA::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void GameApp2D::OnImGuiRender()
{
	EDNA_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
	

	//uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	//ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });


	auto stats = EDNA::Renderer2D::GetStats();
	ImGui::SeparatorText("Renderer2D Stats");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indicies: %d", stats.GetTotalIndexCount());

	//for (auto& result : m_ProfileResults)
	//{
	//	char label[50];
	//	strcpy(label, "%.3fms ");
	//	strcat(label, result.Name);
	//	ImGui::Text(label, result.Time);
	//}
	//m_ProfileResults.clear();

	ImGui::End();

}

void GameApp2D::OnEvent(EDNA::Event& e)
{
	m_CameraController.OnEvent(e);
}

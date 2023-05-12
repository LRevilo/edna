#include "ednapch.h"
#include "MainLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <chrono>

namespace EDNA {

	MainLayer::MainLayer()
		: Layer("MainLayer"), m_CameraController(1280.0f / 720.0f)
	{

	}


	void MainLayer::OnAttach()
	{
		EDNA_PROFILE_FUNCTION();
		m_CheckerBoardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_OverworldTilesTexture = Texture2D::Create("Assets/Textures/OverworldTiles.png");
		m_ShrubTile = SubTexture2D::CreateFromCoords(m_OverworldTilesTexture, { 7,4 }, { 16,16 }, { 1,1 });

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_Scene = CreateRef<Scene>();

		m_CameraEntity = m_Scene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,9.90f)));
		m_CameraEntity.AddComponent<CameraComponent>(glm::perspective(90.0f,16.0f/9.0f, 1.0f, 10.0f));

		m_SquareEntity = m_Scene->CreateEntity("Square");
		m_SquareEntity.AddComponent<TransformComponent>();
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f,0.0f,1.0f});
	
		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				
			}
			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.f;

				if (Input::IsKeyPressed(EDNA_KEY_A))
					transform[3][0] -= speed * ts;

				if (Input::IsKeyPressed(EDNA_KEY_D))
					transform[3][0] += speed * ts;

				if (Input::IsKeyPressed(EDNA_KEY_W))
					transform[3][1] += speed * ts;

				if (Input::IsKeyPressed(EDNA_KEY_S))
					transform[3][1] -= speed * ts;

			}
		};


		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void MainLayer::OnDetach()
	{
	}

	void MainLayer::OnUpdate(Timestep ts)
	{
		EDNA_PROFILE_FUNCTION();
		
		//resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			EDNA_CORE_INFO("Resized window: {0},{1}", m_ViewportSize.x, m_ViewportSize.y);
		}

		m_CameraController.OnUpdate(ts);

		// Udate scene


		Renderer2D::ResetStats();
		RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 1.0 });
		RenderCommand::Clear();


		


		m_Scene->OnUpdate(ts);

		//Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f },3.141592f/6.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Renderer2D::EndScene();



		//m_Framebuffer->Unbind();


	}

	void MainLayer::OnImGuiRender()
	{
		EDNA_PROFILE_FUNCTION();
		ImGui::Begin("Settings");


		auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
		ImGui::ColorEdit4("SquareColor", glm::value_ptr(squareColor));


		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(1);
		ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });


		auto stats = Renderer2D::GetStats();
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

	void MainLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent event = (WindowResizeEvent&)e;
			m_ViewportSize.x = (float)event.GetWidth();
			m_ViewportSize.y = (float)event.GetHeight();
		}
	}
}
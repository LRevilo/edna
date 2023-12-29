#include "ednapch.h"
#include "MainLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "EDNA/Audio/AudioEngine.h"
#include <chrono>

#include "Scripts/CameraController.h"
#include "Scripts/RandomMovementScript.h"
#include "Scripts/PlayerMovementScript.h"




namespace EDNA {

	MainLayer::MainLayer()
		: Layer("MainLayer")//, m_CameraController(1280.0f / 720.0f)
	{
		EDNA_CORE_INFO("MainLayer Constructor");
	}


	void MainLayer::OnAttach()
	{
		EDNA_PROFILE_FUNCTION();
		//EDNA::Audio::LoadAndPlayFromFile("Assets/Audio/Drone 1.wav");




		m_CheckerBoardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_OverworldTilesTexture = Texture2D::Create("Assets/Textures/OverworldTiles.png");
		m_ShrubTile = SubTexture2D::CreateFromCoords(m_OverworldTilesTexture, { 7,4 }, { 16,16 }, { 1,1 });
		Renderer::Shaders()->Load("GlyphShader", "Assets/Shaders/Glyph.glsl");
		m_GlyphMaterial = CreateRef<GlyphMaterial>();
		m_GlyphMaterial->m_ShaderName = "GlyphShader";


		// main frame buffer
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, /*FramebufferTextureFormat::RED_INTEGER, */ FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		m_TextureFramebuffer = Framebuffer::Create(fbSpec);

		m_Scene = CreateRef<Scene>();
		m_Scene->OnViewportResize((uint32_t)1280, (uint32_t)720);
		// 
		m_NoiseTexture = Texture2D::Create(NOISE_SIZE, NOISE_SIZE);
		GenerateNoiseTexture(1, 42069);


		m_CameraEntity = m_Scene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 20.f)));
		//m_CameraEntity.AddComponent<CameraComponent>(glm::perspective(120.0f,16.0f/9.0f, 1.0f, 40.0f));
		auto& cam = m_CameraEntity.AddComponent<CameraComponent>();
		cam.Camera.SetPerspective(120.0f, 16.0f / 9.0f, 1.0f, 40.0f);


		m_SquareEntity = m_Scene->CreateEntity("Player");
		m_SquareEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0)));
		m_SquareEntity.AddComponent<PlayerInputComponent>();
		//m_SquareEntity.AddComponent<SpriteRendererComponent>(m_CheckerBoardTexture, glm::vec4{ 0.0f, 1.0f,0.0f,1.0f });
		m_SquareEntity.AddComponent<CameraAttachmentComponent>(0.5f, glm::vec3{ 0.f, 0.f, 20.f });
		m_SquareEntity.AddComponent<RenderableComponent>();
		m_SquareEntity.AddComponent<NativeScriptComponent>().Bind<PlayerMovementScript>();
		auto& m = m_SquareEntity.AddComponent<MeshComponent>();
		m.MeshData.CubeMesh();
		m.MeshData.m_Material = m_GlyphMaterial;

		//m_SquareEntity.AddComponent<NativeScriptComponent>().Bind<RandomMovementScript>();




		m_OtherSquareEntity = m_Scene->CreateEntity("OtherSquare");
		m_OtherSquareEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.f)));
		m_OtherSquareEntity.AddComponent<RenderableComponent>();
		m_OtherSquareEntity.AddComponent<ShadowCasterComponent>();
		m_OtherSquareEntity.AddComponent<MeshComponent>();

		auto& mesh = m_OtherSquareEntity.GetComponent<MeshComponent>();
		mesh.MeshData.PlaneMesh(5.f,16);
		mesh.MeshData.m_Material = m_GlyphMaterial;



		m_PlaneEntity = m_Scene->CreateEntity("FloorPlane");
		m_PlaneEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(-5.f, -5.f, 0.f)));
		m_PlaneEntity.AddComponent<RenderableComponent>();
		m_PlaneEntity.AddComponent<MeshComponent>();
		auto& plane = m_PlaneEntity.GetComponent<MeshComponent>();
		plane.MeshData.PlaneMesh(0.f);

		plane.MeshData.m_Material = m_GlyphMaterial;;
		//plane.MeshData.m_Material->m_MaterialName = "Material";
		//plane.MeshData.m_Material->m_ShaderName = "3DShader";


		int imax = 10;
		for (int i = 0; i < imax; i++)
		{
			std::string index = std::to_string(i);
			m_AnotherSquareEntity = m_Scene->CreateEntity("AnotherSquare (" + index + ")");
			float rand = (float)i / imax;
			glm::mat4 transform = glm::mat4(1.0f);

			transform[0][0] = ((101 * i) % 17) / 6.f;
			transform[1][0] = ((37 * i) % 55) / 11.f;
			transform[2][0] = ((57 * i) % 5) / 3.f;

			transform[0][1] = ((11 * i) % 7) / 6.f;
			transform[1][1] = ((19 * i) % 5) / 11.f;
			transform[2][1] = ((31 * i) % 73) / 7.f;

			transform[0][2] = ((41 * i) % 13) / 3.f;
			transform[1][2] = ((23 * i) % 7) / 7.f;
			transform[2][2] = ((19 * i) % 11) / 7.f;

			m_AnotherSquareEntity.AddComponent<TransformComponent>(transform);
			m_AnotherSquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, rand,1.f - rand,1.0f });
			m_AnotherSquareEntity.AddComponent<NativeScriptComponent>().Bind<RandomMovementScript>();
		}

		// m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		//SoundDevice::Get()->LoadSound("Assets/Audio/Drone 1.wav");

		auto c1 = glm::vec4(1, 1, 1, 1);

		auto c2 = glm::vec4(0, 1, 1, 1);
		auto c3 = glm::vec4(1, 0, 1, 1);
		auto c4 = glm::vec4(1, 1, 0, 1);

		auto c5 = glm::vec4(1, 0, 0, 1);
		auto c6 = glm::vec4(0, 1, 0, 1);
		auto c7 = glm::vec4(0, 0, 1, 1);

		CreateCube({ 1.f,1.f,1.f }, c1);
		CreateCube({ 3.f,1.f,3.f }, c2);
		CreateCube({ -8.f, 3.f, 0.f }, c3);
		CreateCube({ -8.f, -3.f, 4.f }, c4);
		CreateCube({ -1.f, -1.f, 0.f }, c5);
		CreateCube({ -5.f, 1.f, 0.f }, c6);
		CreateCube({ 5.f, 3.f, 0.f }, c7);
		CreateCube({ 5.f, -2.f, -1.f });



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
			//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			EDNA_CORE_INFO("Resized window: {0},{1}", m_ViewportSize.x, m_ViewportSize.y);
		}
		Renderer2D::ResetStats();
		RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 1.0 });



		m_Scene->OnUpdate(ts);


		auto& trans = m_OtherSquareEntity.GetComponent<TransformComponent>();
		glm::vec3 ray_dir = m_Scene->GetRayDirection();

		float radius = 22.f;
		auto& cam_trans = m_CameraEntity.GetComponent<TransformComponent>();

		//EDNA_CORE_INFO("{0}, {1}, {2}", trans.Transform[3][0], trans.Transform[3][1], trans.Transform[3][2]);

		trans.Transform[3][0] = cam_trans.Transform[3][0] + ray_dir.x * radius;
		trans.Transform[3][1] = cam_trans.Transform[3][1] + ray_dir.y * radius;
		trans.Transform[3][2] = cam_trans.Transform[3][2] + ray_dir.z * radius;



	}

	void MainLayer::OnImGuiRender()
	{
		EDNA_PROFILE_FUNCTION();
		ImGui::Begin("Settings");


		glm::vec3 ray_dir = m_Scene->GetRayDirection();
		ImGui::Text("Ray: %f , %f, %f", ray_dir.x, ray_dir.y, ray_dir.z);


		//  auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
		//  ImGui::ColorEdit4("SquareColor", glm::value_ptr(squareColor));
		//  
		//  auto& OtherSquareColor = m_OtherSquareEntity.GetComponent<SpriteRendererComponent>().Color;
		//  ImGui::ColorEdit4("OtherSquareColor", glm::value_ptr(OtherSquareColor));
		//  
		//  auto& AnotherSquareColor = m_AnotherSquareEntity.GetComponent<SpriteRendererComponent>().Color;
		//  ImGui::ColorEdit4("AnotherSquareColor", glm::value_ptr(AnotherSquareColor));


		//uint64_t textureID = m_Scene->GetShadowBuffer()->GetDepthAttachmentRendererID();


		static int seed = 2;
		ImGui::SliderInt("Seed", &seed, 0, 64);
		static float scale = 2;
		ImGui::SliderFloat("Scale", &scale, 0, 64);

		static float x = 0.f;
		static float y = 0.f;
		ImGui::SliderFloat("X", &x, -64.f, 64.f);
		ImGui::SliderFloat("Y", &y, -64.f, 64.f);


		GenerateNoiseTexture(scale, seed, x,y);

		uint64_t textureID = m_NoiseTexture->GetID();
		static int v = 16;
		ImGui::SliderInt("zoom", &v, 1, 64);
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ (float)m_NoiseTexture->GetWidth() * v, (float)m_NoiseTexture->GetHeight() * v }, ImVec2{ 0,1 }, ImVec2{ 1,0 });


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
		//m_CameraController.OnEvent(e);

		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent event = (WindowResizeEvent&)e;
			m_ViewportSize.x = (float)event.GetWidth();
			m_ViewportSize.y = (float)event.GetHeight();
		}
	}


	Entity MainLayer::CreateCube(glm::vec3 pos, glm::vec4 color)
	{
		static int id = 0;

		auto cubeEntity = m_Scene->CreateEntity("Cube " + id);
		cubeEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), pos));
		cubeEntity.AddComponent<RenderableComponent>();
		cubeEntity.AddComponent<ShadowCasterComponent>();
		cubeEntity.AddComponent<MeshComponent>();

		auto& mesh = cubeEntity.GetComponent<MeshComponent>();
		mesh.MeshData.CubeMesh(color);

		//m_Material = CreateRef<GlyphMaterial>();
		mesh.MeshData.m_Material = m_GlyphMaterial;
		//mesh.MeshData.m_Material->m_MaterialName = "Material";
		//mesh.MeshData.m_Material->m_ShaderName = "GlyphShader";

		id++;

		return cubeEntity;
	}
	void MainLayer::GenerateNoiseTexture(float scale, int seed, float x_in, float y_in)
	{
		float isize = 1.f / (NOISE_SIZE - 1);
		for (int y = 0; y < NOISE_SIZE; y++)
		{
			for (int x = 0; x < NOISE_SIZE; x++)
			{
				auto& pixel = NOISE_DATA[x + NOISE_SIZE * y];
				float X = 2 * (x) * isize - 1 ;
				float Y = 2 * (y) * isize - 1 ;

				float scalar = scale*0.398942280401f; // 1/sqrt(2*pi);
				float n1 = Noise::ValueNoise(x_in - X*scalar, y_in + Y * scalar, 1 + seed);




				//float n2 = Noise::ValueNoise(x_in - 2* scalar * X, y_in + 2 * scalar * Y, 2 + seed);
				//float n3 = Noise::ValueNoise(x_in - 4 * scalar * X, y_in + 4*scalar * Y, 3 + seed);


				float n = n1;// +0.3 * n2 + 0.1 * n3;

				//n = floor(16 * n) / 16.f;
				//n = (n > 0.5f);


				pixel.SetRGBA(n, n, n, n);
			}
		}

		m_NoiseTexture->SetData(NOISE_DATA, NOISE_SIZE * NOISE_SIZE * 4);
	}
}
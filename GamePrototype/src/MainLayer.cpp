#include "ednapch.h"
#include "MainLayer.h"


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
		//EDNA::Audio::LoadAndPlayFromFile("Assets/Audio/piano_intro.wav");
		LoadShaders();

		m_CheckerBoardTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");
		m_OverworldTilesTexture = Texture2D::Create("Assets/Textures/OverworldTiles.png");
		m_ShrubTile = SubTexture2D::CreateFromCoords(m_OverworldTilesTexture, { 7,4 }, { 16,16 }, { 1,1 });

		m_GlyphMaterial = CreateRef<GlyphMaterial>();
		m_GlyphMaterial->m_ShaderName = "GlyphShader";


		m_DebugMaterial = CreateRef<DebugMaterial>();
		m_DebugMaterial->m_ShaderName = "VegetationShader";
		m_DebugMaterial->Bits = 0b1111111110000001100000011000000110000001100000011000000111111111;


		// main frame buffer
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, /*FramebufferTextureFormat::RED_INTEGER, */ FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		m_TextureFramebuffer = Framebuffer::Create(fbSpec);


		// 
		m_NoiseTexture = Texture2D::Create(NOISE_SIZE, NOISE_SIZE);
		GenerateNoiseTexture(1, 42069);


		m_Scene = CreateRef<Scene>();
		m_Scene->OnViewportResize((uint32_t)1280, (uint32_t)720);


		m_CameraEntity = m_Scene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 20.f)));
		//m_CameraEntity.AddComponent<CameraComponent>(glm::perspective(120.0f,16.0f/9.0f, 1.0f, 40.0f));
		auto& cam = m_CameraEntity.AddComponent<CameraComponent>();
		cam.Camera.SetPerspective(120.0f, 16.0f / 9.0f, 1.0f, 40.0f);


		m_SquareEntity = m_Scene->CreateEntity("Player");
		m_SquareEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0)));
		m_SquareEntity.AddComponent<PlayerInputComponent>();
		//m_SquareEntity.AddComponent<SpriteRendererComponent>(m_CheckerBoardTexture, glm::vec4{ 0.0f, 1.0f,0.0f,1.0f });
		auto &cameraAttach = m_SquareEntity.AddComponent<CameraAttachmentComponent>(0.0f, glm::vec3{ 0.f, 0.f, 0.f });
		cameraAttach.Direction = 15.0f*glm::vec3(0.f, 0.5f, -1.f);

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
		mesh.MeshData.PlaneMesh(0.2f,4);
		mesh.MeshData.m_Material = m_GlyphMaterial;



		float scale = 8.0;
		CreateFloorPlane(0, 0, scale);
		CreateFloorPlane(1, 0, scale);
		CreateFloorPlane(0, 1, scale);
		CreateFloorPlane(1, 1, scale);
		//plane.MeshData.m_Material->m_MaterialName = "Material";
		//plane.MeshData.m_Material->m_ShaderName = "3DShader";


		int imax = 10;
		for (int i = 0; i < imax; i++)
		{
			std::string index = std::to_string(i);
			m_AnotherSquareEntity = m_Scene->CreateEntity("AnotherSquare (" + index + ")");
			float rand = (float)i / imax;
			glm::mat4 transform = glm::mat4(1.0f);

			//transform[0][0] = ((101 * i) % 17) / 6.f;
			//transform[1][0] = ((37 * i) % 55) / 11.f;
			//transform[2][0] = ((57 * i) % 5) / 3.f;
			//
			//transform[0][1] = ((11 * i) % 7) / 6.f;
			//transform[1][1] = ((19 * i) % 5) / 11.f;
			//transform[2][1] = ((31 * i) % 73) / 7.f;
			//
			//transform[0][2] = ((41 * i) % 13) / 3.f;
			//transform[1][2] = ((23 * i) % 7) / 7.f;
			//transform[2][2] = ((19 * i) % 11) / 7.f;

			m_AnotherSquareEntity.AddComponent<RenderableComponent>();
			m_AnotherSquareEntity.AddComponent<TransformComponent>(transform);
			m_AnotherSquareEntity.AddComponent<SpriteRendererComponent>(m_CheckerBoardTexture, glm::vec4{ 0.0f, 1.0f,0.0f,1.0f });
			m_AnotherSquareEntity.AddComponent<NativeScriptComponent>().Bind<RandomMovementScript>();
		}


		// Create cubes
		{
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


		RenderCommand::SetClearColour({ 0.1, 0.1, 0.1, 0.0f });


		m_DebugMaterial->m_Time += ts;

		m_Scene->OnUpdate(ts);


		auto& trans = m_OtherSquareEntity.GetComponent<TransformComponent>();
		glm::vec3 ray_dir = m_Scene->GetRayDirection();

		float radius = 22.f;
		auto& cam_trans = m_CameraEntity.GetComponent<TransformComponent>();

		//EDNA_CORE_INFO("{0}, {1}, {2}", trans.Transform[3][0], trans.Transform[3][1], trans.Transform[3][2]);

		trans.Transform[3][0] = cam_trans.Transform[3][0] + ray_dir.x * radius;
		trans.Transform[3][1] = cam_trans.Transform[3][1] + ray_dir.y * radius;
		trans.Transform[3][2] = cam_trans.Transform[3][2] + ray_dir.z * radius;

		auto& player_trans = m_SquareEntity.GetComponent<TransformComponent>();
		


		// finding intersection of raycast and z = 0 plane
		auto v = ray_dir;
		auto p = glm::vec3(cam_trans.Transform[3][0], cam_trans.Transform[3][1], cam_trans.Transform[3][2]);
		float xi = p.x - v.x * p.z / v.z;
		float yi = p.y - v.y * p.z / v.z;



		glm::vec3 position = glm::vec3(player_trans.Transform[3][0], player_trans.Transform[3][1], player_trans.Transform[3][2]);  // Camera position
		glm::vec3 target = glm::vec3(xi, yi,0);    // Target position (direction to face)
		glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);         // Up vector

		glm::mat4 viewMatrix = glm::lookAt(position, target, up);

		player_trans.Transform = glm::inverse(viewMatrix);//glm::rotate(player_trans.Transform, 0.01f,glm::vec3(0.f,0.f,1.f));
		// blows up when mouse is at origin

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

		ImTextureID my_tex_id = reinterpret_cast<void*>(textureID);

		//UIStyleColorsTransparent();
		//ImGui::SetNextWindowContentSize();
		ImGui::SetNextWindowSize(ImVec2(300,600));
		ImGui::Begin("Inventory", NULL, 
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar | 
			ImGuiWindowFlags_NoSavedSettings 
			/*
			ImGuiWindowFlags_NoInputs 
			ImGuiWindowFlags_NoTitleBar 
			ImGuiWindowFlags_NoMove
			*/
		);

		
		const char* buttonText = "Click me pls";
		// Render the texture using Image
		ImVec2 buttonSize = ImVec2(200, 100);

		ImGui::ImageButton(my_tex_id, buttonSize);
		ImVec2 buttonMin = ImGui::GetItemRectMin();
		ImVec2 buttonMax = ImGui::GetItemRectMax();

		// Center the text under the button
		ImVec2 textSize = ImGui::CalcTextSize(buttonText);
		ImVec2 textPosition = ImVec2(ImGui::GetItemRectMin().x + (ImGui::GetItemRectSize().x - textSize.x) * 0.5f,
			ImGui::GetItemRectMax().y + 5.0f - ImGui::GetItemRectSize().y/2.f); // Adjust spacing

		ImGui::SetCursorScreenPos(textPosition);
		ImGui::Text("%s", buttonText);
		float y_margin = 10.f;
		ImGui::SetCursorScreenPos(ImVec2(buttonMin.x, buttonMax.y + y_margin));

		// Check if the button is pressed
		if (ImGui::IsItemClicked())
		{
			// Handle button click
			// Add your logic here
		}

	

		struct ButtonData {
			const char* Name = "Default name";
			ImTextureID my_tex_id = 0;

			ImVec2 size = ImVec2(32.0f, 32.0f);
			ImVec2 uv0 = ImVec2(0.0f, 1.0f);
			ImVec2 uv1 = ImVec2(1.f, 0.f);    // UV coordinates for (32
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		};



		static const char* names[9] =
		{
			"Bobby", "Beatrice", "Betty",
			"Brianna", "Barry", "Bernard",
			"Bibi", "Blaine", "Bryn"
		};


		static bool t = true;
		static std::vector<ButtonData> buttons = { ButtonData(), ButtonData(),  ButtonData(),  ButtonData(),  ButtonData(),  ButtonData(),  ButtonData(),  ButtonData(),  ButtonData() };
		if (t)
		{

		for (int k = 0; k < IM_ARRAYSIZE(names); k++)
		{
			ButtonData data;
			data.my_tex_id = reinterpret_cast<void*>(textureID);
			data.Name = "Button " + k;
			data.tint_col = ImVec4(1.0f, 1.0f, k/9.f, 1.0f);
			buttons[k] = data;
		}
		t = false;
		}




		static int pressed_count = 0;
		for (int i = 0; i < 8; i++)
		{
			// UV coordinates are often (0.0f, 0.0f) and (1.0f, 1.0f) to display an entire textures.
			// Here are trying to display only a 32x32 pixels area of the texture, hence the UV computation.
			// Read about UV coordinates here: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
			ImGui::PushID(i);
			if (i > 0)
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(i - 1.0f, i - 1.0f));

			if (ImGui::ImageButton(buttons[i].Name, buttons[i].my_tex_id, buttons[i].size, buttons[i].uv0, buttons[i].uv1, buttons[i].bg_col, buttons[i].tint_col))
				pressed_count += 1;
			if (i > 0)
				ImGui::PopStyleVar();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::NewLine();



			enum Mode
			{
				Mode_Copy,
				Mode_Move,
				Mode_Swap
			};
			static int mode = 0;
			if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
			if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
			if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

			for (int n = 0; n < IM_ARRAYSIZE(names); n++)
			{
				ImGui::PushID(n);
				if ((n % 3) != 0)
					ImGui::SameLine();
				//ImGui::Button(names[n], ImVec2(60, 60));
				ImGui::ImageButton(buttons[n].Name, buttons[n].my_tex_id, buttons[n].size, buttons[n].uv0, buttons[n].uv1, buttons[n].bg_col, buttons[n].tint_col);

				// Our buttons are both drag sources and drag targets here!
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Set payload to carry the index of our item (could be anything)
					ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

					// Display preview (could be anything, e.g. when dragging an image we could decide to display
					// the filename and a small preview of the image, etc.)
					if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }
					if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
					if (mode == Mode_Swap) { ImGui::Text("Swap %s", buttons[n].Name); }
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
					{
						IM_ASSERT(payload->DataSize == sizeof(int));
						int payload_n = *(const int*)payload->Data;
						if (mode == Mode_Copy)
						{
							names[n] = names[payload_n];
							
						}
						if (mode == Mode_Move)
						{
							names[n] = names[payload_n];
							names[payload_n] = "";
						}
						if (mode == Mode_Swap)
						{
							ButtonData tmp = buttons[n];
							buttons[n] = buttons[payload_n];
							buttons[payload_n] = tmp;
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopID();
			}
			//ImGui::TreePop();
		
		ImGui::End();
		//ImGui::StyleColorsDark();
	
	

		ImGui::ShowDemoWindow();

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





	void MainLayer::LoadShaders()
	{
		Renderer::Shaders()->Load("ShrubShader", "Assets/Shaders/ShrubShader.glsl");
		Renderer::Shaders()->Load("GrassShader", "Assets/Shaders/GrassShader.glsl");
		Renderer::Shaders()->Load("VegetationShader", "Assets/Shaders/VegetationShader.glsl");
		Renderer::Shaders()->Load("GlyphShader", "Assets/Shaders/Glyph.glsl");
	}





	void MainLayer::UIStyleColorsTransparent()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.00f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 0.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 0.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 0.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 0.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 0.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 0.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 0.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 0.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 0.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 0.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 0.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 0.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 0.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 0.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.00f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.00f);
	}

	Entity MainLayer::CreateCube(glm::vec3 pos, glm::vec4 color)
	{
		static int id = 0;

		auto cubeEntity = m_Scene->CreateEntity("Cube " + id);
		cubeEntity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), pos));
		cubeEntity.AddComponent<RenderableComponent>();
		cubeEntity.AddComponent<ShadowCasterComponent>();
		cubeEntity.AddComponent<MeshComponent>();
		//cubeEntity.AddComponent<NativeScriptComponent>().Bind<RandomMovementScript>();
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

	void MainLayer::CreateFloorPlane(int x, int y, float scale)
	{

		auto entity = m_Scene->CreateEntity("FloorPlane");
		entity.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(scale*x, scale*y, 0.f)));
		entity.AddComponent<RenderableComponent>();
		auto& mesh = entity.AddComponent<MeshComponent>();
		//m_PlaneEntity.AddComponent<ShadowCasterComponent>();
		mesh.MeshData.FloorPlaneMesh(glm::vec2(x,y), scale,8);
		mesh.MeshData.m_Material = m_DebugMaterial;
	}


}
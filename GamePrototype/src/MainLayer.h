#pragma once
#include "EDNA.h"


namespace EDNA {

	struct PlayerData
	{
		std::string Name = "Player";

		// Stats
		int BaseHP = 5;
		int HP = 5;

		int BaseEnergy = 5;
		int Energy = 5;

		int BaseStr = 1;
		int Str = 1;

		int BaseDex = 1;
		int Dex = 1;

		int BaseInt = 1;
		int Int = 1;

		int BaseLuk = 1;
		int Luk = 1;
	};

	struct Pixel
	{
		uint8_t R = 255;
		uint8_t G = 255;
		uint8_t B = 0;
		uint8_t A = 255;
		void SetRGBA(float r, float g, float b, float a)
		{
			R = (uint8_t)floor(255 * r);
			G = (uint8_t)floor(255 * g);
			B = (uint8_t)floor(255 * b);
			A = (uint8_t)floor(255 * a);
		}
	};

	class MainLayer : public Layer
	{

	public:
		MainLayer();
		virtual ~MainLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;																					


		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event&) override;

		void LoadShaders();



		void UIStyleColorsTransparent();
		Entity CreateCube(glm::vec3 pos, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
		void GenerateNoiseTexture(float scale, int seed, float x = 0, float y = 0 );
		void CreateFloorPlane(int x, int y, float scale);



	private:

		//OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_CheckerBoardTexture;
		Ref<Texture2D> m_OverworldTilesTexture;
		Ref<SubTexture2D> m_ShrubTile;

		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_ShadowFramebuffer;

		Ref<Framebuffer> m_TextureFramebuffer;



		const int NOISE_SIZE = 64;
		Pixel* NOISE_DATA = new Pixel[NOISE_SIZE * NOISE_SIZE];
		Ref<Texture2D> m_NoiseTexture;
		
		Ref<Scene> m_Scene;

		Entity m_SquareEntity;
		Entity m_OtherSquareEntity;
		Entity m_AnotherSquareEntity;
		Entity m_CameraEntity;
		Entity m_PlaneEntity;

		Ref<Material> m_Material;
		Ref<GlyphMaterial> m_GlyphMaterial;
		Ref<Material> m_TextureMaterial;

		Ref<DebugMaterial> m_DebugMaterial;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		uint32_t test = 1;

	};

}
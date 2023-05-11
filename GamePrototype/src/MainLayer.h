#pragma once
#include "EDNA.h"

namespace EDNA {
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

	private:
		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_CheckerBoardTexture;
		Ref<Texture2D> m_OverworldTilesTexture;
		Ref<SubTexture2D> m_ShrubTile;

		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;

		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_Scene;

		Entity m_SquareEntity;
		Entity m_CameraEntity;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	};

}
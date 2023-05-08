#pragma once
#include "EDNA.h"

class GameApp2D : public EDNA::Layer
{

public:
	GameApp2D();
	virtual ~GameApp2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;


	void OnUpdate(EDNA::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(EDNA::Event&) override;

private:
	EDNA::OrthographicCameraController m_CameraController;

	EDNA::Ref<EDNA::Texture2D> m_CheckerBoardTexture;

	EDNA::Ref<EDNA::VertexArray> m_SquareVA;
	EDNA::Ref<EDNA::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };



};
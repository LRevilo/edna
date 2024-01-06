#pragma once

#include "EDNA/Renderer/RendererAPI.h"

namespace EDNA {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void Clear() override;
		virtual void ClearDepth() override;
		virtual void EnableDepth(bool depth) override;
		virtual void BindDepthMap(uint32_t id) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

	};

}
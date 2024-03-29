#pragma once

#include "glm/glm.hpp"

#include "VertexArray.h"

namespace EDNA {


	class RendererAPI 
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};


	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void Clear() = 0;
		virtual void ClearDepth() = 0;
		virtual void EnableDepth(bool depth) = 0;
		virtual void BindDepthMap(uint32_t id) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		 
		inline static API GetAPI() { return s_API; }



	private:
		static API s_API;




	};

 }
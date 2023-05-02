#include "ednapch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace EDNA {


	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		EDNA_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::OpenGl:	return new OpenGLVertexBuffer(vertices, size);
		}
		EDNA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		EDNA_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::OpenGl:	return new OpenGLIndexBuffer(indices, size);
		}
		EDNA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
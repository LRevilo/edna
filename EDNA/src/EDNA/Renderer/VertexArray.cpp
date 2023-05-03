#include "ednapch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace EDNA {


	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		EDNA_CORE_ASSERT(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}
		EDNA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
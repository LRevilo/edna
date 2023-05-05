#include "ednapch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace EDNA {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}
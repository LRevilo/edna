#include "ednapch.h"

#include "OpenGLRendererAPI.h"
#include <glad/glad.h>


namespace EDNA {
	void OpenGLRendererAPI::Init()
	{

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_GEOMETRY_SHADER);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);

	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableDepth(bool depth)
	{
		if (depth) { glEnable(GL_DEPTH_TEST); }
		else {glDisable(GL_DEPTH_TEST);}
	}

	void OpenGLRendererAPI::BindDepthMap(uint32_t id)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		//EDNA_CORE_INFO("Index count: {0}", count);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			EDNA_CORE_ERROR("OpenGL Error: {0}", error);
		}


		glBindTexture(GL_TEXTURE_2D, 0);

		//EDNA_CORE_INFO("RenderAPI: end draw indexed");

	}
}
